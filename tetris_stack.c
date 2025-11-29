#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Configurações ---
#define MAX_FILA 5
#define MAX_PILHA 3

typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;
} Peca;

// --- Globais ---
Peca fila[MAX_FILA];
int inicio = 0;
int fim = 0;
int total_fila = 0;

Peca pilha[MAX_PILHA];
int topo = 0; // Aponta para o próximo espaço VAZIO

int id_contador = 0;

// --- Protótipos ---
Peca gerarPeca();
void inicializarJogo();
void reabastecerFila();
void jogarPeca();         // Ação 1
void reservarPeca();      // Ação 2
void usarReserva();       // Ação 3
void trocarPecaUnica();   // Ação 4 (Novo)
void trocaMultipla();     // Ação 5 (Novo - Desafio Master)
void exibirEstado();
void limparBuffer();

int main() {
    srand(time(NULL));
    inicializarJogo();

    int opcao;
    do {
        exibirEstado();

        printf("\n=== Opcoes disponiveis ===\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao escolhida: ");
        
        scanf("%d", &opcao);
        limparBuffer();

        printf("\n------------------------------------------------\n");

        switch (opcao) {
            case 1: jogarPeca(); break;
            case 2: reservarPeca(); break;
            case 3: usarReserva(); break;
            case 4: trocarPecaUnica(); break;
            case 5: trocaMultipla(); break;
            case 0: printf("Encerrando Tetris Stack...\n"); break;
            default: printf("[!] Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação ---

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id_contador++;
    return p;
}

void inicializarJogo() {
    for (int i = 0; i < MAX_FILA; i++) {
        fila[fim] = gerarPeca();
        fim = (fim + 1) % MAX_FILA;
        total_fila++;
    }
}

void reabastecerFila() {
    // Garante que a fila esteja cheia se houver espaço
    while (total_fila < MAX_FILA) {
        Peca nova = gerarPeca();
        fila[fim] = nova;
        fim = (fim + 1) % MAX_FILA;
        total_fila++;
        printf("   [Sistema] Reposicao automatica: Peca [%c %d] entrou na fila.\n", nova.nome, nova.id);
    }
}

// 1. Remove da frente
void jogarPeca() {
    if (total_fila == 0) return;
    Peca p = fila[inicio];
    inicio = (inicio + 1) % MAX_FILA;
    total_fila--;
    printf(">> Acao: JOGOU a peca [%c %d]\n", p.nome, p.id);
    reabastecerFila();
}

// 2. Move Fila -> Pilha
void reservarPeca() {
    if (topo >= MAX_PILHA) {
        printf("[!] Erro: A pilha de reserva esta CHEIA.\n");
        return;
    }
    
    Peca p = fila[inicio];
    inicio = (inicio + 1) % MAX_FILA;
    total_fila--;

    pilha[topo] = p;
    topo++;
    printf(">> Acao: RESERVOU a peca [%c %d]\n", p.nome, p.id);
    reabastecerFila();
}

// 3. Remove da Pilha (Usa)
void usarReserva() {
    if (topo == 0) {
        printf("[!] Erro: A pilha de reserva esta VAZIA.\n");
        return;
    }
    topo--;
    Peca p = pilha[topo];
    printf(">> Acao: USOU a peca da reserva [%c %d]\n", p.nome, p.id);
    // Nota: Usar da reserva não afeta a fila, então não precisa reabastecer
}

// 4. Troca simples (Frente da Fila <-> Topo da Pilha)
void trocarPecaUnica() {
    if (total_fila == 0 || topo == 0) {
        printf("[!] Erro: Precisa de pecas na fila E na pilha para trocar.\n");
        return;
    }

    // Identifica os índices
    int idx_fila = inicio;
    int idx_pilha = topo - 1; // O topo real é topo-1

    // Realiza a troca (Swap)
    Peca temp = fila[idx_fila];
    fila[idx_fila] = pilha[idx_pilha];
    pilha[idx_pilha] = temp;

    printf(">> Acao: TROCA realizada ([%c %d] <-> [%c %d])\n", 
           fila[idx_fila].nome, fila[idx_fila].id, 
           pilha[idx_pilha].nome, pilha[idx_pilha].id);
}

// 5. Troca Múltipla (3 da Fila <-> 3 da Pilha)
void trocaMultipla() {
    // Validação: Pilha precisa estar cheia (3 itens) e Fila ter pelo menos 3
    if (topo < 3 || total_fila < 3) {
        printf("[!] Erro: Para troca multipla, a pilha deve ter 3 pecas.\n");
        return;
    }

    printf(">> Acao: Realizando TROCA EM BLOCO (3 pecas)...\n");

    // Loop para trocar 3 peças
    for (int i = 0; i < 3; i++) {
        // Cálculo do índice na FILA CIRCULAR:
        // Precisamos do elemento 'inicio', 'inicio+1', 'inicio+2'
        int idx_fila = (inicio + i) % MAX_FILA;

        // Cálculo do índice na PILHA:
        // Topo (topo-1), depois (topo-2), depois (topo-3)
        int idx_pilha = (topo - 1) - i;

        // Swap
        Peca temp = fila[idx_fila];
        fila[idx_fila] = pilha[idx_pilha];
        pilha[idx_pilha] = temp;
    }
    printf(">> Sucesso! As 3 primeiras pecas foram trocadas.\n");
}

void exibirEstado() {
    printf("\nEstado atual:\n");
    
    // Fila
    printf("Fila de pecas: ");
    int idx = inicio;
    for (int i = 0; i < total_fila; i++) {
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");

    // Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (topo == 0) printf("[ Vazia ]");
    else {
        for (int i = topo - 1; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
    }
    printf("\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}