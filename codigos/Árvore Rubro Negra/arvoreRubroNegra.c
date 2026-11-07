#include <stdio.h>
#include <stdlib.h>

// Propriedade 1: Todo nó é VERMELHO ou PRETO
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
    No *NIL; // Sentinela que representa as folhas pretas
} ArvoreRN;

// --- FUNÇÕES DE INICIALIZAÇÃO ---

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

    // Propriedade 3: Toda folha (NIL) é PRETA
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
    novo->cor = VERMELHO; // Inserção com correção: o nó novo é pintado de vermelho
    novo->esquerda = arvore->NIL;
    novo->direita = arvore->NIL;
    novo->pai = arvore->NIL;
    return novo;
}

// --- FUNÇÕES AUXILIARES E DE BUSCA ---

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

// --- ROTAÇÕES ---

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

// --- INSERÇÃO E CORREÇÃO ---

void balancearInsercao(ArvoreRN *arvore, No *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) { // Pai é filho à esquerda
            No *y = z->pai->pai->direita; // Tio
            
            if (y->cor == VERMELHO) { 
                // Tio Vermelho: Recoloração
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else { 
                // Tio Preto
                if (z == z->pai->direita) { // Esquerda-Direita
                    z = z->pai;
                    rotacaoEsquerda(arvore, z);
                }
                // Esquerda-Esquerda: Rotação seguida de recoloração
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(arvore, z->pai->pai);
            }
        } else { // Pai é filho à direita
            No *y = z->pai->pai->esquerda; // Tio
            
            if (y->cor == VERMELHO) {
                // Tio Vermelho: Recoloração
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                // Tio Preto
                if (z == z->pai->esquerda) { // Direita-Esquerda
                    z = z->pai;
                    rotacaoDireita(arvore, z);
                }
                // Direita-Direita: Rotação seguida de recoloração
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = PRETO; // A raiz deve ser preta ao final
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

// --- REMOÇÃO E CORREÇÃO ---

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

    // Caso 1 e 2: O nó a ser removido tem no máximo 1 filho
    if (z->esquerda == arvore->NIL) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->NIL) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        // Caso 3: O nó tem dois filhos. Substituir pelo sucessor.
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

    // Corrigir violações se o nó removido da sua posição original for preto
    if (corOriginalY == PRETO) {
        balancearRemocao(arvore, x);
    }
    printf("Valor %d removido com sucesso.\n", valor);
}

// --- PERCURSOS ---

void percorrerPreOrdem(ArvoreRN *arvore, No *raiz) {
    if (raiz != arvore->NIL) {
        printf("%d ", raiz->valor);
        percorrerPreOrdem(arvore, raiz->esquerda);
        percorrerPreOrdem(arvore, raiz->direita);
    }
}

void percorrerEmOrdem(ArvoreRN *arvore, No *raiz) {
    if (raiz != arvore->NIL) {
        percorrerEmOrdem(arvore, raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerEmOrdem(arvore, raiz->direita);
    }
}

void percorrerPosOrdem(ArvoreRN *arvore, No *raiz) {
    if (raiz != arvore->NIL) {
        percorrerPosOrdem(arvore, raiz->esquerda);
        percorrerPosOrdem(arvore, raiz->direita);
        printf("%d ", raiz->valor);
    }
}

// --- EXIBIÇÃO E CÁLCULOS ---

int calcularAlturaPreta(ArvoreRN *arvore, No *no) {
    int altura = 0;
    // Percorre um caminho qualquer (vamos pela esquerda) até chegar ao sentinela NIL
    while (no != arvore->NIL) {
        if (no->cor == PRETO) {
            altura++;
        }
        no = no->esquerda; 
    }
    return altura + 1; // Soma 1 porque o próprio NIL é considerado preto
}

void imprimirArvoreCores(ArvoreRN *arvore, No *raiz, int espaco) {
    if (raiz == arvore->NIL) return;
    
    espaco += 5;
    imprimirArvoreCores(arvore, raiz->direita, espaco);
    
    printf("\n");
    for (int i = 5; i < espaco; i++) printf(" ");
    
    printf("%d(%s)\n", raiz->valor, raiz->cor == VERMELHO ? "VERMELHO" : "PRETO");
    
    imprimirArvoreCores(arvore, raiz->esquerda, espaco);
}

void exibirCoresEAltura(ArvoreRN *arvore) {
    if (arvore->raiz == arvore->NIL) {
        printf("A arvore esta vazia! Altura preta: 0\n");
        return;
    }
    
    int alturaPreta = calcularAlturaPreta(arvore, arvore->raiz);
    printf("\n--- Altura Preta da Arvore: %d ---\n", alturaPreta);
    printf("Estrutura e Cores:\n");
    imprimirArvoreCores(arvore, arvore->raiz, 0);
    printf("\n----------------------------------\n");
}

// --- LIMPEZA DE MEMÓRIA ---

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

// --- FUNÇÃO PRINCIPAL (MENUS) ---

int main() {
    ArvoreRN *arvore = criarArvore();
    int opcaoMenuPrincipal, opcaoSubmenu, valor;

    do {
        printf("\n======= MENU ARVORE RUBRO-NEGRA =======\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("5 - Exibir cores e altura preta\n");
        printf("0 - Sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcaoMenuPrincipal) != 1) {
            while (getchar() != '\n');
            opcaoMenuPrincipal = -1;
        }

        switch (opcaoMenuPrincipal) {
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
                    printf("Resultado: O valor %d ESTA presente na arvore.\n", valor);
                } else {
                    printf("Resultado: O valor %d NAO ESTA presente na arvore.\n", valor);
                }
                break;
                
            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                remover(arvore, valor);
                break;
                
            case 4:
                printf("\n--- Submenu: Percorrer Arvore ---\n");
                printf("1 - Pre-ordem\n");
                printf("2 - Em ordem\n");
                printf("3 - Pos-ordem\n");
                printf("Escolha o tipo de percurso: ");
                scanf("%d", &opcaoSubmenu);

                if (arvore->raiz == arvore->NIL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }

                printf("Resultado: ");
                switch (opcaoSubmenu) {
                    case 1: percorrerPreOrdem(arvore, arvore->raiz); break;
                    case 2: percorrerEmOrdem(arvore, arvore->raiz); break;
                    case 3: percorrerPosOrdem(arvore, arvore->raiz); break;
                    default: printf("Opcao de percurso invalida!");
                }
                printf("\n");
                break;
                
            case 5:
                exibirCoresEAltura(arvore);
                break;
                
            case 0:
                printf("Encerrando programa e liberando memoria...\n");
                liberarArvore(arvore);
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcaoMenuPrincipal != 0);

    return 0;
}