#include <stdio.h>
#include <stdlib.h>

typedef struct NoAVL {
    int valor;
    int altura;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
} NoAVL;

int altura(NoAVL* no) {
    if (no == NULL) return 0;
    return no->altura;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int fatorBalanceamento(NoAVL* no) {
    if (no == NULL) return 0;
    return altura(no->esquerda) - altura(no->direita);
}

NoAVL* rotacaoDireita(NoAVL* y) {
    NoAVL* x = y->esquerda;
    NoAVL* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = maior(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

NoAVL* rotacaoEsquerda(NoAVL* x) {
    NoAVL* y = x->direita;
    NoAVL* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = maior(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

NoAVL* criarNoAVL(int valor) {
    NoAVL* novo = (NoAVL*)malloc(sizeof(NoAVL));
    if (novo != NULL) {
        novo->valor = valor;
        novo->esquerda = NULL;
        novo->direita = NULL;
        novo->altura = 1;
    }
    return novo;
}

NoAVL* inserirAVL(NoAVL* raiz, int valor) {
    if (raiz == NULL) {
        printf("Valor %d inserido com sucesso.\n", valor);
        return criarNoAVL(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserirAVL(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserirAVL(raiz->direita, valor);
    } else {
        printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
        return raiz; 
    }

    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));

    int fb = fatorBalanceamento(raiz);


    if (fb > 1 && valor < raiz->esquerda->valor)
        return rotacaoDireita(raiz);

    if (fb < -1 && valor > raiz->direita->valor)
        return rotacaoEsquerda(raiz);

    if (fb > 1 && valor > raiz->esquerda->valor) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && valor < raiz->direita->valor) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

NoAVL* encontrarMinimoAVL(NoAVL* raiz) {
    NoAVL* atual = raiz;
    while (atual && atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

NoAVL* removerAVL(NoAVL* raiz, int valor) {
    if (raiz == NULL) {
        printf("Valor %d nao encontrado para remocao.\n", valor);
        return raiz;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = removerAVL(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = removerAVL(raiz->direita, valor);
    } else {
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            NoAVL* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
            printf("Valor removido com sucesso.\n");
        } else {
            NoAVL* temp = encontrarMinimoAVL(raiz->direita);
            raiz->valor = temp->valor;
            raiz->direita = removerAVL(raiz->direita, temp->valor);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

int buscarAVL(NoAVL* raiz, int valor) {
    if (raiz == NULL) return 0;
    if (raiz->valor == valor) return 1;
    if (valor < raiz->valor) return buscarAVL(raiz->esquerda, valor);
    return buscarAVL(raiz->direita, valor);
}

void percorrerPreOrdem(NoAVL* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        percorrerPreOrdem(raiz->esquerda);
        percorrerPreOrdem(raiz->direita);
    }
}

void percorrerEmOrdem(NoAVL* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerEmOrdem(raiz->direita);
    }
}

void percorrerPosOrdem(NoAVL* raiz) {
    if (raiz != NULL) {
        percorrerPosOrdem(raiz->esquerda);
        percorrerPosOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

void exibirBalanceamento(NoAVL* raiz) {
    if (raiz != NULL) {
        exibirBalanceamento(raiz->esquerda);
        printf("No: %d | Altura: %d | FB: %d\n", raiz->valor, raiz->altura, fatorBalanceamento(raiz));
        exibirBalanceamento(raiz->direita);
    }
}

void liberarArvore(NoAVL* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    NoAVL* raiz = NULL;
    int opcaoMenuPrincipal, opcaoSubmenu, valor;

    do {
        printf("\n========================\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("5 - Exibir altura e fator de balanceamento\n");
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
                raiz = inserirAVL(raiz, valor);
                break;
            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                if (buscarAVL(raiz, valor))
                    printf("Resultado: O valor %d ESTA presente na arvore.\n", valor);
                else
                    printf("Resultado: O valor %d NAO ESTA presente na arvore.\n", valor);
                break;
            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                raiz = removerAVL(raiz, valor);
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
                    case 1: percorrerPreOrdem(raiz); break;
                    case 2: percorrerEmOrdem(raiz); break;
                    case 3: percorrerPosOrdem(raiz); break;
                    default: printf("Opcao de percurso invalida!");
                }
                printf("\n");
                break;
            case 5:
                if (raiz == NULL) {
                    printf("A arvore esta vazia! Altura: 0\n");
                } else {
                    printf("\nAltura total da arvore: %d\n", raiz->altura);
                    printf("Fatores de balanceamento (exibicao Em Ordem):\n");
                    exibirBalanceamento(raiz);
                }
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