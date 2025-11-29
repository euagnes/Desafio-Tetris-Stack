#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Configurações e Constantes ---
#define MAX_FILA 5   // Tamanho fixo da fila
#define MAX_PILHA 3  // Tamanho fixo da reserva (pilha)

typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// --- Variáveis Globais ---

// Estrutura da Fila (Circular)
Peca fila[MAX_FILA];
int inicio = 0;
int fim = 0;
int total_fila = 0;

// Estrutura da Pilha (Linear)
Peca pilha[MAX_PILHA];
int topo = 0; // Indica a próxima posição livre na pilha (0 = vazia)

int id_contador = 0; // Controle de IDs únicos

// --- Protótipos ---
Peca gerarPeca();
void inicializarJogo();
void reabastecerFila(); // Auxiliar para manter a fila cheia
void jogarPeca();       // Ação 1
void reservarPeca();    // Ação 2
void usarReserva();     // Ação 3
void exibirEstado();
void limparBuffer();

int main() {
    srand(time(NULL));
    
    inicializarJogo(); // Já começa com a fila cheia

    int opcao;
    do {
        exibirEstado();

        printf("\n=== Opcoes de Acao ===\n");
        printf("1 - Jogar peca (Da frente da fila)\n");
        printf("2 - Reservar peca (Da fila para a pilha)\n");
        printf("3 - Usar peca reservada (Do topo da pilha)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        
        scanf("%d", &opcao);
        limparBuffer();

        printf("\n------------------------------------------------\n");

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarReserva();
                break;
            case 0:
                printf("Encerrando Tetris Stack...\n");
                break;
            default:
                printf("[!] Opcao invalida.\n");
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

// Enche a fila completamente no início
void inicializarJogo() {
    printf("Iniciando sistema... Gerando pecas iniciais.\n");
    for (int i = 0; i < MAX_FILA; i++) {
        Peca nova = gerarPeca();
        fila[fim] = nova;
        fim = (fim + 1) % MAX_FILA; // Lógica circular
        total_fila++;
    }
}

// Função auxiliar interna: Adiciona uma peça nova no final da fila automaticamente
void reabastecerFila() {
    if (total_fila < MAX_FILA) {
        Peca nova = gerarPeca();
        fila[fim] = nova;
        fim = (fim + 1) % MAX_FILA;
        total_fila++;
        printf("   -> Nova peca [%c %d] entrou na fila (reposicao).\n", nova.nome, nova.id);
    }
}

// Ação 1: Joga a peça da frente e repõe
void jogarPeca() {
    // Como a fila é reabastecida sempre, ela teoricamente nunca está vazia,
    // mas é boa prática verificar.
    if (total_fila == 0) return; 

    Peca p = fila[inicio];
    inicio = (inicio + 1) % MAX_FILA;
    total_fila--;

    printf(">> Voce JOGOU a peca: [%c %d]\n", p.nome, p.id);
    
    // Regra do desafio: Manter a fila sempre cheia
    reabastecerFila();
}

// Ação 2: Move da frente da Fila para o topo da Pilha
void reservarPeca() {
    if (topo >= MAX_PILHA) {
        printf("[!] A reserva (Pilha) esta CHEIA! Use uma peca reservada antes.\n");
        return;
    }
    
    // 1. Pega a peça da fila (Dequeue)
    Peca p = fila[inicio];
    inicio = (inicio + 1) % MAX_FILA;
    total_fila--;

    // 2. Coloca na pilha (Push)
    pilha[topo] = p;
    topo++; // Incrementa o topo

    printf(">> Peca [%c %d] movida para a RESERVA.\n", p.nome, p.id);

    // 3. A fila precisa ser completada
    reabastecerFila();
}

// Ação 3: Usa a peça do topo da Pilha
void usarReserva() {
    if (topo == 0) {
        printf("[!] A reserva (Pilha) esta VAZIA!\n");
        return;
    }

    // Pop: Pega a peça do topo (topo - 1)
    topo--; 
    Peca p = pilha[topo];

    printf(">> Voce USOU a peca da RESERVA: [%c %d]\n", p.nome, p.id);
    
    // Nota: Aqui NÃO chamamos reabastecerFila(), pois a peça saiu da pilha,
    // e a fila não foi afetada.
}

void exibirEstado() {
    printf("\nEstado atual:\n");
    
    // 1. Exibir Fila
    printf("Fila de pecas: ");
    int idx = inicio;
    for (int i = 0; i < total_fila; i++) {
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");

    // 2. Exibir Pilha
    // Mostramos do Topo (último adicionado) para a Base (índice 0)
    printf("Pilha de reserva (Topo -> Base): ");
    if (topo == 0) {
        printf("[ Vazia ]");
    } else {
        // O índice 'topo' aponta para o espaço vazio, então começamos de topo-1
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