#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RUBRO,
    NEGRO
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


/* Inicializa a árvore */
ArvoreRN *criarArvore()
{
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

    arvore->NIL->cor = NEGRO;
    arvore->NIL->esquerda = arvore->NIL;
    arvore->NIL->direita = arvore->NIL;
    arvore->NIL->pai = arvore->NIL;

    arvore->raiz = arvore->NIL;

    return arvore;
}


/* Cria um novo nó */
No *criarNo(ArvoreRN *arvore, int valor)
{
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo->valor = valor;
    novo->cor = RUBRO;
    novo->esquerda = arvore->NIL;
    novo->direita = arvore->NIL;
    novo->pai = arvore->NIL;

    return novo;
}


/* Busca um valor na árvore */
No *buscar(No *no, No *NIL, int valor)
{
    if (no == NIL || no->valor == valor) {
        return no;
    }

    if (valor < no->valor) {
        return buscar(no->esquerda, NIL, valor);
    }

    return buscar(no->direita, NIL, valor);
}
