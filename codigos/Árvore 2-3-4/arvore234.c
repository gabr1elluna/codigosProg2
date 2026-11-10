#include <stdio.h>
#include <stdlib.h>

typedef struct No234 {
    int numChaves;
    int chaves[3];
    struct No234* filhos[4];
    int folha;
} No234;

No234* criarNo(int folha) {
    No234* novo = (No234*)malloc(sizeof(No234));
    if (novo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    novo->numChaves = 0;
    novo->folha = folha;
    for (int i = 0; i < 4; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

int buscar(No234* raiz, int valor) {
    if (raiz == NULL) return 0;
    
    int i = 0;
    while (i < raiz->numChaves && valor > raiz->chaves[i]) {
        i++;
    }
    
    if (i < raiz->numChaves && valor == raiz->chaves[i]) {
        return 1;
    }
    
    if (raiz->folha) {
        return 0;
    }
    
    return buscar(raiz->filhos[i], valor);
}

void dividirFilho(No234* pai, int i, No234* filhoCheio) {
    No234* novoNo = criarNo(filhoCheio->folha);
    novoNo->numChaves = 1;

    novoNo->chaves[0] = filhoCheio->chaves[2];
    
    if (!filhoCheio->folha) {
        novoNo->filhos[0] = filhoCheio->filhos[2];
        novoNo->filhos[1] = filhoCheio->filhos[3];
    }

    filhoCheio->numChaves = 1;

    for (int j = pai->numChaves; j > i; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[i + 1] = novoNo;
    

    for (int j = pai->numChaves - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }
    pai->chaves[i] = filhoCheio->chaves[1];
    pai->numChaves++;
}

void inserirNaoCheio(No234* no, int valor) {
    int i = no->numChaves - 1;
    
    if (no->folha) {
        for (int k = 0; k < no->numChaves; k++) {
            if (no->chaves[k] == valor) {
                printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
                return;
            }
        }
        
        while (i >= 0 && valor < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        if (i >= 0 && valor == no->chaves[i]) {
            printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
            return;
        }
        no->chaves[i + 1] = valor;
        no->numChaves++;
    } else {
        while (i >= 0 && valor < no->chaves[i]) {
            i--;
        }
        i++;

        if (i - 1 >= 0 && no->chaves[i - 1] == valor) {
            printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
            return;
        }

        if (no->filhos[i]->numChaves == 3) {
            dividirFilho(no, i, no->filhos[i]);
            if (valor > no->chaves[i]) {
                i++;
            } else if (valor == no->chaves[i]) {
                printf("O valor %d ja existe na arvore. Insercao ignorada.\n", valor);
                return;
            }
        }
        inserirNaoCheio(no->filhos[i], valor);
    }
}

No234* inserir(No234* raiz, int valor) {
    if (raiz == NULL) {
        No234* novaRaiz = criarNo(1);
        novaRaiz->chaves[0] = valor;
        novaRaiz->numChaves = 1;
        printf("Valor %d inserido com sucesso.\n", valor);
        return novaRaiz;
    }
    
    if (raiz->numChaves == 3) {
        No234* novaRaiz = criarNo(0);
        novaRaiz->filhos[0] = raiz;
        dividirFilho(novaRaiz, 0, raiz);
        
        int i = 0;
        if (novaRaiz->chaves[0] < valor) {
            i++;
        }
        inserirNaoCheio(novaRaiz->filhos[i], valor);
        printf("Valor %d inserido com sucesso.\n", valor);
        return novaRaiz;
    } else {
        inserirNaoCheio(raiz, valor);
        printf("Valor %d inserido (ou checado) com sucesso.\n", valor);
        return raiz;
    }
}

int encontrarChave(No234* no, int k) {
    int idx = 0;
    while (idx < no->numChaves && no->chaves[idx] < k)
        idx++;
    return idx;
}

int pegarPredecessor(No234* no, int idx) {
    No234* atual = no->filhos[idx];
    while (!atual->folha)
        atual = atual->filhos[atual->numChaves];
    return atual->chaves[atual->numChaves - 1];
}

int pegarSucessor(No234* no, int idx) {
    No234* atual = no->filhos[idx + 1];
    while (!atual->folha)
        atual = atual->filhos[0];
    return atual->chaves[0];
}

void fundir(No234* no, int idx) {
    No234* filho = no->filhos[idx];
    No234* irmao = no->filhos[idx + 1];

    filho->chaves[1] = no->chaves[idx];

    filho->chaves[2] = irmao->chaves[0];
    if (!filho->folha) {
        filho->filhos[2] = irmao->filhos[0];
        filho->filhos[3] = irmao->filhos[1];
    }
    filho->numChaves = 3;
    
    for (int i = idx + 1; i < no->numChaves; i++)
        no->chaves[i - 1] = no->chaves[i];
    for (int i = idx + 2; i <= no->numChaves; i++)
        no->filhos[i - 1] = no->filhos[i];
    no->numChaves--;
    
    free(irmao);
}

void preencher(No234* no, int idx) {
    if (idx != 0 && no->filhos[idx - 1]->numChaves >= 2) {
        No234* filho = no->filhos[idx];
        No234* irmao = no->filhos[idx - 1];
        
        for (int i = filho->numChaves - 1; i >= 0; i--)
            filho->chaves[i + 1] = filho->chaves[i];
        if (!filho->folha) {
            for (int i = filho->numChaves; i >= 0; i--)
                filho->filhos[i + 1] = filho->filhos[i];
        }
        
        filho->chaves[0] = no->chaves[idx - 1];
        if (!filho->folha)
            filho->filhos[0] = irmao->filhos[irmao->numChaves];
        
        no->chaves[idx - 1] = irmao->chaves[irmao->numChaves - 1];
        filho->numChaves++;
        irmao->numChaves--;
    }
    else if (idx != no->numChaves && no->filhos[idx + 1]->numChaves >= 2) {
        No234* filho = no->filhos[idx];
        No234* irmao = no->filhos[idx + 1];
        
        filho->chaves[filho->numChaves] = no->chaves[idx];
        if (!filho->folha)
            filho->filhos[filho->numChaves + 1] = irmao->filhos[0];
        
        no->chaves[idx] = irmao->chaves[0];
        
        for (int i = 1; i < irmao->numChaves; i++)
            irmao->chaves[i - 1] = irmao->chaves[i];
        if (!irmao->folha) {
            for (int i = 1; i <= irmao->numChaves; i++)
                irmao->filhos[i - 1] = irmao->filhos[i];
        }
        
        filho->numChaves++;
        irmao->numChaves--;
    }
    else {
        if (idx != no->numChaves) {
            fundir(no, idx);
        } else {
            fundir(no, idx - 1);
        }
    }
}

void removerNo(No234* no, int valor) {
    int idx = encontrarChave(no, valor);
    
    if (idx < no->numChaves && no->chaves[idx] == valor) {
        if (no->folha) {
            for (int i = idx + 1; i < no->numChaves; i++)
                no->chaves[i - 1] = no->chaves[i];
            no->numChaves--;
        } else {
            if (no->filhos[idx]->numChaves >= 2) {
                int pred = pegarPredecessor(no, idx);
                no->chaves[idx] = pred;
                removerNo(no->filhos[idx], pred);
            } else if (no->filhos[idx + 1]->numChaves >= 2) {
                int suc = pegarSucessor(no, idx);
                no->chaves[idx] = suc;
                removerNo(no->filhos[idx + 1], suc);
            } else {
                fundir(no, idx);
                removerNo(no->filhos[idx], valor);
            }
        }
    } 
    else {
        if (no->folha) {
            printf("O valor %d nao esta na arvore.\n", valor);
            return;
        }
        
        int flag = ((idx == no->numChaves) ? 1 : 0);
        
        if (no->filhos[idx]->numChaves < 2) {
            preencher(no, idx);
        }
        
        if (flag && idx > no->numChaves) {
            removerNo(no->filhos[idx - 1], valor);
        } else {
            removerNo(no->filhos[idx], valor);
        }
    }
}

No234* remover(No234* raiz, int valor) {
    if (!raiz) {
        printf("A arvore esta vazia!\n");
        return raiz;
    }
    
    removerNo(raiz, valor);
    
    if (raiz->numChaves == 0) {
        No234* tmp = raiz;
        if (raiz->folha) {
            raiz = NULL;
        } else {
            raiz = raiz->filhos[0];
        }
        free(tmp);
    }
    return raiz;
}

void percorrerEmOrdem(No234* raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->numChaves; i++) {
            if (!raiz->folha) {
                percorrerEmOrdem(raiz->filhos[i]);
            }
            printf("%d ", raiz->chaves[i]);
        }
        if (!raiz->folha) {
            percorrerEmOrdem(raiz->filhos[i]);
        }
    }
}

void exibirPorNiveis(No234* raiz) {
    if (raiz == NULL) {
        printf("Arvore vazia.\n");
        return;
    }

    No234* fila[1000];
    int niveis[1000];
    int inicio = 0, fim = 0;

    fila[fim] = raiz;
    niveis[fim] = 0;
    fim++;

    int nivelAtual = 0;
    printf("Nivel 0: ");

    while (inicio < fim) {
        No234* atual = fila[inicio];
        int nivel = niveis[inicio];
        inicio++;

        if (nivel > nivelAtual) {
            nivelAtual = nivel;
            printf("\nNivel %d: ", nivelAtual);
        }

        printf("[");
        for (int i = 0; i < atual->numChaves; i++) {
            printf("%d", atual->chaves[i]);
            if (i < atual->numChaves - 1) printf(" ");
        }
        printf("] ");

        if (!atual->folha) {
            for (int i = 0; i <= atual->numChaves; i++) {
                if (atual->filhos[i] != NULL) {
                    fila[fim] = atual->filhos[i];
                    niveis[fim] = nivel + 1;
                    fim++;
                }
            }
        }
    }
    printf("\n");
}


void liberarArvore(No234* raiz) {
    if (raiz != NULL) {
        if (!raiz->folha) {
            for (int i = 0; i <= raiz->numChaves; i++) {
                liberarArvore(raiz->filhos[i]);
            }
        }
        free(raiz);
    }
}

int main() {
    No234* raiz = NULL;
    int opcao, valor;

    do {
        printf("\n======= MENU =======\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer em ordem\n");
        printf("5 - Exibir arvore por niveis\n");
        printf("0 - Sair\n");
        printf("====================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
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
                printf("\n--- Percurso Em Ordem ---\n");
                if (raiz == NULL) {
                    printf("A arvore esta vazia.");
                } else {
                    percorrerEmOrdem(raiz);
                }
                printf("\n-------------------------\n");
                break;

            case 5:
                printf("\n--- Arvore por Niveis ---\n");
                exibirPorNiveis(raiz);
                printf("-------------------------\n");
                break;

            case 0:
                printf("Encerrando programa e liberando memoria...\n");
                liberarArvore(raiz);
                raiz = NULL;
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}