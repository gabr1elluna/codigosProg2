#include <stdio.h>
#include <stdlib.h>

typedef enum {
    VERMELHO,
    PRETO
} Cor;

typedef struct No {
    int valor;
    Cor cor;
    struct No *esquerda;
    struct No *direita;
    struct No *pai;
} No;

typedef struct {
    No *raiz;
    No *NIL;
} ArvoreRN;

ArvoreRN *criarArvore() {
    ArvoreRN *arvore = malloc(sizeof(ArvoreRN));
    if (arvore == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    arvore->NIL = malloc(sizeof(No));
    if (arvore->NIL == NULL) {
        free(arvore);
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    arvore->NIL->cor = PRETO;
    arvore->NIL->esquerda = arvore->NIL;
    arvore->NIL->direita = arvore->NIL;
    arvore->NIL->pai = arvore->NIL;

    arvore->raiz = arvore->NIL;
    return arvore;
}

No *criarNo(ArvoreRN *arvore, int valor) {
    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo->valor = valor;
    novo->cor = VERMELHO; 
    novo->esquerda = arvore->NIL;
    novo->direita = arvore->NIL;
    novo->pai = arvore->NIL;
    return novo;
}


No *buscar(No *no, No *NIL, int valor) {
    if (no == NIL || no->valor == valor) {
        return no;
    }
    if (valor < no->valor) {
        return buscar(no->esquerda, NIL, valor);
    }
    return buscar(no->direita, NIL, valor);
}

No *minimo(ArvoreRN *arvore, No *no) {
    while (no->esquerda != arvore->NIL) {
        no = no->esquerda;
    }
    return no;
}

void transplantar(ArvoreRN *arvore, No *u, No *v) {
    if (u->pai == arvore->NIL) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

void rotacaoEsquerda(ArvoreRN *arvore, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->NIL) y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == arvore->NIL) arvore->raiz = y;
    else if (x == x->pai->esquerda) x->pai->esquerda = y;
    else x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

void rotacaoDireita(ArvoreRN *arvore, No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != arvore->NIL) x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == arvore->NIL) arvore->raiz = x;
    else if (y == y->pai->direita) y->pai->direita = x;
    else y->pai->esquerda = x;
    x->direita = y;
    y->pai = x;
}


void balancearInsercao(ArvoreRN *arvore, No *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            No *y = z->pai->pai->direita;
            
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacaoEsquerda(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(arvore, z->pai->pai);
            }
        } else {
            No *y = z->pai->pai->esquerda;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacaoDireita(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = PRETO;
}

void inserir(ArvoreRN *arvore, int valor) {
    No *z = criarNo(arvore, valor);
    No *y = arvore->NIL;
    No *x = arvore->raiz;

    while (x != arvore->NIL) {
        y = x;
        if (z->valor < x->valor) x = x->esquerda;
        else if (z->valor > x->valor) x = x->direita;
        else {
            printf("Valor %d ja existe. Insercao ignorada.\n", valor);
            free(z);
            return;
        }
    }

    z->pai = y;
    if (y == arvore->NIL) arvore->raiz = z;
    else if (z->valor < y->valor) y->esquerda = z;
    else y->direita = z;

    balancearInsercao(arvore, z);
    printf("Valor %d inserido com sucesso.\n", valor);
}


void balancearRemocao(ArvoreRN *arvore, No *x) {
    while (x != arvore->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            No *irmao = x->pai->direita;
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, x->pai);
                irmao = x->pai->direita;
            }
            if (irmao->esquerda->cor == PRETO && irmao->direita->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->direita->cor == PRETO) {
                    irmao->esquerda->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoDireita(arvore, irmao);
                    irmao = x->pai->direita;
                }
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->direita->cor = PRETO;
                rotacaoEsquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else {
            No *irmao = x->pai->esquerda;
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(arvore, x->pai);
                irmao = x->pai->esquerda;
            }
            if (irmao->direita->cor == PRETO && irmao->esquerda->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esquerda->cor == PRETO) {
                    irmao->direita->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(arvore, irmao);
                    irmao = x->pai->esquerda;
                }
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->esquerda->cor = PRETO;
                rotacaoDireita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = PRETO;
}

void remover(ArvoreRN *arvore, int valor) {
    No *z = buscar(arvore->raiz, arvore->NIL, valor);
    if (z == arvore->NIL) {
        printf("Valor %d nao encontrado para remocao.\n", valor);
        return;
    }

    No *y = z;
    No *x;
    Cor corOriginalY = y->cor;

    if (z->esquerda == arvore->NIL) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->NIL) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        y = minimo(arvore, z->direita);
        corOriginalY = y->cor;
        x = y->direita;

        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplantar(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (corOriginalY == PRETO) {
        balancearRemocao(arvore, x);
    }
    printf("Valor %d removido com sucesso.\n", valor);
}


void imprimirArvore(ArvoreRN *arvore, No *raiz, int espaco) {
    if (raiz == arvore->NIL) return;
    
    espaco += 5;
    imprimirArvore(arvore, raiz->direita, espaco);
    
    printf("\n");
    for (int i = 5; i < espaco; i++) printf(" ");
    
    char siglaCor = (raiz->cor == VERMELHO) ? 'V' : 'P';
    printf("%d(%c)\n", raiz->valor, siglaCor);
    
    imprimirArvore(arvore, raiz->esquerda, espaco);
}

void liberarNos(ArvoreRN *arvore, No *no) {
    if (no != arvore->NIL) {
        liberarNos(arvore, no->esquerda);
        liberarNos(arvore, no->direita);
        free(no);
    }
}

void liberarArvore(ArvoreRN *arvore) {
    liberarNos(arvore, arvore->raiz);
    free(arvore->NIL);
    free(arvore);
}


int main() {
    ArvoreRN *arvore = criarArvore();
    int opcao, valor;

    do {
        printf("\n======= MENU ARVORE RUBRO-NEGRA =======\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Imprimir arvore\n");
        printf("0 - Sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                inserir(arvore, valor);
                break;
                
            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                No* busca = buscar(arvore->raiz, arvore->NIL, valor);
                if (busca != arvore->NIL) {
                    printf("Resultado: O valor %d ESTA na arvore. Cor: %s\n", 
                           valor, busca->cor == VERMELHO ? "VERMELHO" : "PRETO");
                } else {
                    printf("Resultado: O valor %d NAO ESTA na arvore.\n", valor);
                }
                break;
                
            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                remover(arvore, valor);
                break;
                
            case 4:
                if (arvore->raiz == arvore->NIL) {
                    printf("A arvore esta vazia!\n");
                } else {
                    printf("\n--- Estrutura da Arvore ---\n");
                    printf("Legenda: (V) = VERMELHO, (P) = PRETO\n");
                    imprimirArvore(arvore, arvore->raiz, 0);
                    printf("\n---------------------------\n");
                }
                break;
                
            case 0:
                printf("Encerrando e liberando memoria...\n");
                liberarArvore(arvore);
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}