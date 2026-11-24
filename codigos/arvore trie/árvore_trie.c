
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM 26

typedef struct No {
    struct No *filhos[TAM];
    int fimPalavra;
} No;

No* criarNo() {
    No *novo = (No*) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro de memoria!\n");
        exit(1);
    }

    for (int i = 0; i < TAM; i++) {
        novo->filhos[i] = NULL;
    }

    novo->fimPalavra = 0;

    return novo;
}

void inserir(No *raiz, char palavra[]) {
    No *atual = raiz;

    for (int i = 0; palavra[i] != '\0'; i++) {
        char c = tolower((unsigned char)palavra[i]);

        if (c < 'a' || c > 'z') {
            printf("Use apenas letras de A a Z.\n");
            return;
        }

        int indice = c - 'a';

        if (atual->filhos[indice] == NULL) {
            atual->filhos[indice] = criarNo();
        }

        atual = atual->filhos[indice];
    }

    atual->fimPalavra = 1;

    printf("Palavra inserida!\n");
}

int buscar(No *raiz, char palavra[]) {
    No *atual = raiz;

    for (int i = 0; palavra[i] != '\0'; i++) {
        char c = tolower((unsigned char)palavra[i]);

        if (c < 'a' || c > 'z') {
            return 0;
        }

        int indice = c - 'a';

        if (atual->filhos[indice] == NULL) {
            return 0;
        }

        atual = atual->filhos[indice];
    }

    return atual->fimPalavra;
}

int verificarPrefixo(No *raiz, char prefixo[]) {
    No *atual = raiz;

    for (int i = 0; prefixo[i] != '\0'; i++) {
        char c = tolower((unsigned char)prefixo[i]);

        if (c < 'a' || c > 'z') {
            return 0;
        }

        int indice = c - 'a';

        if (atual->filhos[indice] == NULL) {
            return 0;
        }

        atual = atual->filhos[indice];
    }

    return 1;
}

void exibirPalavras(No *no, char palavra[], int nivel) {
    if (no == NULL) {
        return;
    }

    if (no->fimPalavra) {
        palavra[nivel] = '\0';
        printf("%s\n", palavra);
    }

    for (int i = 0; i < TAM; i++) {
        if (no->filhos[i] != NULL) {
            palavra[nivel] = 'a' + i;
            exibirPalavras(no->filhos[i], palavra, nivel + 1);
        }
    }
}

int main() {
    No *raiz = criarNo();

    int opcao;
    char palavra[100];
    char auxiliar[100];

    do {
        printf("\n=== ARVORE TRIE ===\n");
        printf("1 - Inserir palavra\n");
        printf("2 - Buscar palavra\n");
        printf("3 - Verificar prefixo\n");
        printf("4 - Exibir palavras\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a palavra: ");
                scanf("%99s", palavra);
                inserir(raiz, palavra);
                break;

            case 2:
                printf("Digite a palavra: ");
                scanf("%99s", palavra);

                if (buscar(raiz, palavra)) {
                    printf("Palavra encontrada!\n");
                } else {
                    printf("Palavra nao encontrada!\n");
                }
                break;

            case 3:
                printf("Digite o prefixo: ");
                scanf("%99s", palavra);

                if (verificarPrefixo(raiz, palavra)) {
                    printf("Prefixo encontrado!\n");
                } else {
                    printf("Prefixo nao encontrado!\n");
                }
                break;

            case 4:
                printf("\nPalavras armazenadas:\n");
                exibirPalavras(raiz, auxiliar, 0);
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
