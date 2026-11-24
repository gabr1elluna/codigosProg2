
#include <stdio.h>
#include <stdlib.h>

#define TAM 10

typedef struct No {
    int valor;
    struct No *prox;
} No;

No *tabela[TAM];

// Funcao hash
int funcaoHash(int valor) {
    return valor % TAM;
}

// Inicializar tabela
void inicializar() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

// Inserir elemento
void inserir(int valor) {
    int indice = funcaoHash(valor);

    No *atual = tabela[indice];

    // Evita elementos duplicados
    while (atual != NULL) {
        if (atual->valor == valor) {
            printf("Elemento ja existe!\n");
            return;
        }

        atual = atual->prox;
    }

    No *novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    novo->valor = valor;
    novo->prox = tabela[indice];

    tabela[indice] = novo;

    printf("Elemento inserido!\n");
}

// Buscar elemento
int buscar(int valor) {
    int indice = funcaoHash(valor);

    No *atual = tabela[indice];

    while (atual != NULL) {
        if (atual->valor == valor) {
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

// Remover elemento
void remover(int valor) {
    int indice = funcaoHash(valor);

    No *atual = tabela[indice];
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->valor == valor) {

            if (anterior == NULL) {
                tabela[indice] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);

            printf("Elemento removido!\n");
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Elemento nao encontrado!\n");
}

// Exibir tabela
void exibir() {
    printf("\n=== TABELA HASH ===\n");

    for (int i = 0; i < TAM; i++) {
        printf("[%d] -> ", i);

        No *atual = tabela[i];

        while (atual != NULL) {
            printf("%d -> ", atual->valor);
            atual = atual->prox;
        }

        printf("NULL\n");
    }
}

// Liberar memoria
void liberar() {
    for (int i = 0; i < TAM; i++) {
        No *atual = tabela[i];

        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }

        tabela[i] = NULL;
    }
}

int main() {
    int opcao;
    int valor;

    inicializar();

    do {
        printf("\n=== TABELA HASH ===\n");
        printf("1 - Inserir elemento\n");
        printf("2 - Buscar elemento\n");
        printf("3 - Remover elemento\n");
        printf("4 - Exibir tabela\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                inserir(valor);
                break;

            case 2:
                printf("Digite o valor: ");
                scanf("%d", &valor);

                if (buscar(valor)) {
                    printf("Elemento encontrado!\n");
                } else {
                    printf("Elemento nao encontrado!\n");
                }
                break;

            case 3:
                printf("Digite o valor: ");
                scanf("%d", &valor);
                remover(valor);
                break;

            case 4:
                exibir();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberar();

    return 0;
}
