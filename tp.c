#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    RBTree *raiz = NULL; // Inicializa a raiz da árvore como NULL
    int operacao; // Variável para armazenar a operação escolhida
    int qtd, i; // Variáveis para quantidade e loop
    Pessoa pessoa; // Variável para armazenar dados de uma pessoa
    do {
        scanf("%d", &operacao); // Lê a operação do usuário
        if (operacao == 1) { // Se a operação for 1, insere pessoas na árvore
            scanf("%d", &qtd); // Lê a quantidade de pessoas a serem inseridas
            for (i = 0; i < qtd; i++) {
                scanf("%s %d", pessoa.nome, &pessoa.idade); // Lê nome e idade da pessoa
                RBTree *novoNo = leArvore(); // Lê um novo nó da árvore
                novoNo->dado = pessoa; // Atribui os dados da pessoa ao nó
                insercao(&raiz, novoNo); // Insere o nó na árvore
            }
        } else if (operacao == 2) { // Se a operação for 2, imprime a árvore in-order
            printf("Dados inOrder:\n");
            printInOrder(raiz); // Chama a função para imprimir a árvore
        }
    } while (operacao != 0); // Repete até que a operação seja 0
    // Após o loop, verifica se a árvore não está vazia e a imprime
    if (raiz != NULL) {
        printf("Dados inOrder:\n");
        printInOrder(raiz);
        desalocarArvore(raiz); // Desaloca a árvore
    }
    return 0; 
}
