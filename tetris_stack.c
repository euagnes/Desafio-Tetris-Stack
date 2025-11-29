#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definições de constantes
#define MAX_FILA 5 // Tamanho fixo da fila

// Definição da estrutura da Peça
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// Variáveis Globais para controle da Fila
Peca fila[MAX_FILA];
int inicio = 0;
int fim = 0;
int total_elementos = 0;
int id_contador = 0; // Para garantir IDs únicos sempre crescentes

// --- Protótipos das Funções ---
Peca gerarPeca();
void inicializarFila();
void inserirPeca(); // Enqueue
void jogarPeca();   // Dequeue
void exibirFila();
void limparBuffer();

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // 1. Inicializar a fila com peças automáticas
    inicializarFila();

    int opcao;

    // Loop do Menu Principal
    do {
        // Exibe o estado atual antes de pedir a ação
        exibirFila();

        printf("\n=== Tabela: Comandos disponiveis ===\n");
        printf("1 - Jogar peca (Dequeue)\n");
        printf("2 - Inserir nova peca (Enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o "enter" pendente no buffer do teclado

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                inserirPeca();
                break;
            case 0:
                printf("Saindo do Tetris Stack...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
        
        printf("\n------------------------------------------------\n");

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// Gera uma peça com tipo aleatório e ID único
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Sorteia um índice de 0 a 3
    p.nome = tipos[rand() % 4];
    p.id = id_contador++; // Atribui e depois incrementa
    
    return p;
}

// Preenche a fila completamente no início do jogo
void inicializarFila() {
    printf("Inicializando o sistema...\n");
    for (int i = 0; i < MAX_FILA; i++) {
        Peca nova = gerarPeca();
        fila[fim] = nova;
        fim = (fim + 1) % MAX_FILA; // Lógica circular
        total_elementos++;
    }
}

// Adiciona uma peça ao final (Enqueue)
void inserirPeca() {
    if (total_elementos == MAX_FILA) {
        printf("[AVISO] A fila esta cheia! Jogue uma peca antes de inserir outra.\n");
        return;
    }

    Peca nova = gerarPeca();
    
    // Insere na posição 'fim'
    fila[fim] = nova;
    
    // Atualiza o índice 'fim' usando aritmética modular para circularidade
    // Se fim for 4 e MAX for 5: (4+1) % 5 = 0. Volta pro início!
    fim = (fim + 1) % MAX_FILA;
    total_elementos++;
    
    printf(">> Peca [%c %d] inserida com sucesso.\n", nova.nome, nova.id);
}

// Remove a peça da frente (Dequeue)
void jogarPeca() {
    if (total_elementos == 0) {
        printf("[ERRO] A fila esta vazia! Nao ha pecas para jogar.\n");
        return;
    }

    Peca removida = fila[inicio];
    
    // Atualiza o índice 'inicio' circularmente
    inicio = (inicio + 1) % MAX_FILA;
    total_elementos--;

    printf(">> Voce jogou a peca [%c %d]!\n", removida.nome, removida.id);
}

// Mostra a fila formatada
void exibirFila() {
    printf("\nEstado atual da Fila (%d/%d):\n", total_elementos, MAX_FILA);
    
    if (total_elementos == 0) {
        printf("Fila Vazia\n");
        return;
    }

    // Variável auxiliar para percorrer sem alterar o 'inicio' original
    int idx = inicio;
    
    for (int i = 0; i < total_elementos; i++) {
        printf("[%c %d] ", fila[idx].nome, fila[idx].id);
        
        // Avança o índice circularmente apenas para visualização
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// Função utilitária para limpar buffer (evita bugs no scanf)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}