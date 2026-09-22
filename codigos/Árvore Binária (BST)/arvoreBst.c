#include <stdio.h>
#include <stdlib.h>

typedef struct NoBST {
    int valor;
    struct NoBST* esquerda;
    struct NoBST* direita;
} NoBST;

NoBST* criarNo(int valor) {
    NoBST* novo = (NoBST*)malloc(sizeof(NoBST));
    if (novo != NULL) {
        novo->valor = valor;
        novo->esquerda = NULL;
        novo->direita = NULL;
    }
    return novo;
}

NoBST* inserir(NoBST* raiz, int valor) {
    if (raiz == NULL) {
        printf("Valor %d inserido com sucesso.\n", valor);
        return criarNo(valor);
    }
    
    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    } else {
        printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
    }
    
    return raiz;
}

int buscar(NoBST* raiz, int valor) {
    if (raiz == NULL) {
        return 0;
    }
    if (raiz->valor == valor) {
        return 1;
    }
    
    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    } else {
        return buscar(raiz->direita, valor);
    }
}

NoBST* encontrarMinimo(NoBST* raiz) {
    NoBST* atual = raiz;
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

NoBST* remover(NoBST* raiz, int valor) {
    if (raiz == NULL) {
        printf("Valor %d nao encontrado para remocao.\n", valor);
        return raiz;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    } else {

        if (raiz->esquerda == NULL) {
            NoBST* temp = raiz->direita;
            free(raiz);
            printf("Valor removido com sucesso.\n");
            return temp;
        } else if (raiz->direita == NULL) {
            NoBST* temp = raiz->esquerda;
            free(raiz);
            printf("Valor removido com sucesso.\n");
            return temp;
        }

        NoBST* temp = encontrarMinimo(raiz->direita);

        raiz->valor = temp->valor;

        raiz->direita = remover(raiz->direita, temp->valor);
    }
    return raiz;
}

void percorrerPreOrdem(NoBST* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        percorrerPreOrdem(raiz->esquerda);
        percorrerPreOrdem(raiz->direita);
    }
}

void percorrerEmOrdem(NoBST* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerEmOrdem(raiz->direita);
    }
}

void percorrerPosOrdem(NoBST* raiz) {
    if (raiz != NULL) {
        percorrerPosOrdem(raiz->esquerda);
        percorrerPosOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

void liberarArvore(NoBST* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    NoBST* raiz = NULL;
    int opcaoMenuPrincipal, opcaoSubmenu, valor;

    do {
        printf("\n========================\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("0 - Sair\n");
        printf("========================\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcaoMenuPrincipal) != 1) {
            while (getchar() != '\n');
            opcaoMenuPrincipal = -1;
        }

        switch (opcaoMenuPrincipal) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;

            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor)) {
                    printf("Resultado: O valor %d ESTA presente na arvore.\n", valor);
                } else {
                    printf("Resultado: O valor %d NAO ESTA presente na arvore.\n", valor);
                }
                break;

            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;

            case 4:
                printf("\n--- Submenu: Percorrer Arvore ---\n");
                printf("1 - Pre-ordem\n");
                printf("2 - Em ordem\n");
                printf("3 - Pos-ordem\n");
                printf("Escolha o tipo de percurso: ");
                scanf("%d", &opcaoSubmenu);

                if (raiz == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }

                printf("Resultado: ");
                switch (opcaoSubmenu) {
                    case 1:
                        percorrerPreOrdem(raiz);
                        break;
                    case 2:
                        percorrerEmOrdem(raiz);
                        break;
                    case 3:
                        percorrerPosOrdem(raiz);
                        break;
                    default:
                        printf("Opcao de percurso invalida!");
                }
                printf("\n");
                break;

            case 0:
                printf("Encerrando programa e liberando memoria...\n");
                liberarArvore(raiz);
                raiz = NULL;
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcaoMenuPrincipal != 0);

    return 0;
}