#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No* anterior;
    struct No* proximo;
} No;

No* criarNo(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo) {
        novo->valor = valor;
        novo->anterior = NULL;
        novo->proximo = NULL;
    }
    return novo;
}

void inserirInicio(No** cabeca, int valor) {
    No* novo = criarNo(valor);
    if (*cabeca != NULL) {
        novo->proximo = *cabeca;
        (*cabeca)->anterior = novo;
    }
    *cabeca = novo;
    printf("Valor %d inserido no inicio.\n", valor);
}

void inserirFinal(No** cabeca, int valor) {
    No* novo = criarNo(valor);
    if (*cabeca == NULL) {
        *cabeca = novo;
    } else {
        No* atual = *cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
        novo->anterior = atual;
    }
    printf("Valor %d inserido no final.\n", valor);
}

void inserirPosicao(No** cabeca, int valor, int posicao) {
    if (posicao <= 1) {
        inserirInicio(cabeca, valor);
        return;
    }

    No* novo = criarNo(valor);
    No* atual = *cabeca;
    int posAtual = 1;

    while (atual != NULL && posAtual < posicao - 1) {
        atual = atual->proximo;
        posAtual++;
    }

    if (atual == NULL) {
        printf("Posicao %d fora dos limites. Inserindo no final.\n", posicao);
        inserirFinal(cabeca, valor);
        free(novo);
    } else {
        novo->proximo = atual->proximo;
        novo->anterior = atual;
        if (atual->proximo != NULL) {
            atual->proximo->anterior = novo;
        }
        atual->proximo = novo;
        printf("Valor %d inserido na posicao %d.\n", valor, posicao);
    }
}

void removerPosicao(No** cabeca, int posicao) {
    if (*cabeca == NULL) {
        printf("A lista esta vazia!\n");
        return;
    }

    No* atual = *cabeca;
    int posAtual = 1;

    while (atual != NULL && posAtual < posicao) {
        atual = atual->proximo;
        posAtual++;
    }

    if (atual == NULL) {
        printf("Posicao %d nao encontrada.\n", posicao);
        return;
    }

    if (atual->anterior == NULL) {
        *cabeca = atual->proximo;
        if (*cabeca != NULL) {
            (*cabeca)->anterior = NULL;
        }
    } else {
        atual->anterior->proximo = atual->proximo;
    }

    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    }

    printf("No da posicao %d (Valor: %d) removido com sucesso.\n", posicao, atual->valor);
    free(atual);
}

void buscar(No* cabeca, int valor) {
    No* atual = cabeca;
    int posicao = 1;
    while (atual != NULL) {
        if (atual->valor == valor) {
            printf("Valor %d encontrado na posicao %d.\n", valor, posicao);
            return;
        }
        atual = atual->proximo;
        posicao++;
    }
    printf("Valor %d nao encontrado na lista.\n", valor);
}

void listar(No* cabeca) {
    if (cabeca == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }

    No* atual = cabeca;
    int posicao = 1;
    printf("\n--- Conteudo da Lista ---\n");
    while (atual != NULL) {
        printf("Posicao %d | Valor: %d | Endereco: %p | Ant: %p | Prox: %p\n", posicao, atual->valor, (void*)atual, (void*)atual->anterior, (void*)atual->proximo);
        atual = atual->proximo;
        posicao++;
    }
    printf("-------------------------\n");
}

int main() {
    No* lista = NULL;
    int opcao, valor, posicao;

    do {
        printf("===============================================\n");
        printf("1. Inserir no inicio\n");
        printf("2. Inserir em posicao especifica\n");
        printf("3. Inserir no final\n");
        printf("4. Remover de posicao especifica\n");
        printf("5. Buscar valor\n");
        printf("6. Listar elementos\n");
        printf("0. Sair\n");
        printf("===============================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir no inicio: ");
                scanf("%d", &valor);
                inserirInicio(&lista, valor);
                break;
            case 2:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                printf("Digite a posicao (inicial e 1): ");
                scanf("%d", &posicao);
                inserirPosicao(&lista, valor, posicao);
                break;
            case 3:
                printf("Digite o valor para inserir no final: ");
                scanf("%d", &valor);
                inserirFinal(&lista, valor);
                break;
            case 4:
                printf("Digite a posicao do no a ser removido: ");
                scanf("%d", &posicao);
                removerPosicao(&lista, posicao);
                break;
            case 5:
                printf("Digite o valor que deseja buscar: ");
                scanf("%d", &valor);
                buscar(lista, valor);
                break;
            case 6:
                listar(lista);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
