#ifndef ARVORE_H
#define ARVORE_H

typedef enum { VERMELHO, PRETO } CorNo;

// Definição do TAD Pessoa
typedef struct Pessoa {
    char nome[100];
    int idade;
} Pessoa;

// Definição do TAD RBTree
typedef struct RBTree {
    Pessoa dado;
    CorNo cor;
    struct RBTree *pai, *esquerda, *direita;
} RBTree;

// Protótipos das funções para a Árvore Rubro-Negra
RBTree* alocarArvore();
void desalocarArvore(RBTree* arvore);
RBTree* leArvore();
void rotacaoDireita(RBTree** raiz, RBTree* x);
void rotacaoEsquerda(RBTree** raiz, RBTree* x);
void insercao(RBTree **raiz, RBTree *novoNo);
void balanceamento(RBTree **raiz, RBTree *novoNo);
void printInOrder(RBTree* raiz);

#endif // ARVORE_H
