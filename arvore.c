#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

// Aloca um novo nó da Árvore Rubro-Negra
RBTree* alocarArvore() {
    // Aloca memória para um novo nó
    RBTree *novoNo = (RBTree*)malloc(sizeof(RBTree));
    if (novoNo) {
        // Inicializa ponteiros e cor do novo nó
        novoNo->pai = novoNo->esquerda = novoNo->direita = NULL;
        novoNo->cor = VERMELHO; // Novos nós são vermelhos por padrão
    }
    return novoNo; // Retorna o novo nó alocado
}

// Função para leitura e inicialização da árvore
void leArvore(RBTree *arvore) {
    // Configura cor e ponteiros do nó para o padrão
    arvore->cor = VERMELHO; 
    arvore->esquerda = arvore->direita = arvore->pai = NULL;
}

// Desaloca um nó da Árvore Rubro-Negra
void desalocarArvore(RBTree* arvore) {
    if (arvore != NULL) {
        // Desaloca recursivamente os nós filhos
        desalocarArvore(arvore->esquerda);
        desalocarArvore(arvore->direita);
        free(arvore); // Libera a memória do nó atual
    }
}

// Função para realizar rotação à esquerda
void rotacaoEsquerda(RBTree **raiz, RBTree *x) {
    RBTree *y = x->direita; // Define y como filho direito de x
    x->direita = y->esquerda; // Move a subárvore esquerda de y para a direita de x
    if (y->esquerda != NULL) {
        y->esquerda->pai = x; // Ajusta o pai da subárvore esquerda de y
    }
    y->pai = x->pai; // Liga y ao pai de x
    // Atualiza as ligações do pai de x
    if (x->pai == NULL) { // Caso x seja a raiz
        *raiz = y; 
    } else if (x == x->pai->esquerda) { // Caso x seja filho esquerdo
        x->pai->esquerda = y;
    } else { // Caso x seja filho direito
        x->pai->direita = y;
    }
    y->esquerda = x; // Coloca x à esquerda de y
    x->pai = y; // Atualiza o pai de x
}

// Função para realizar rotação à direita
void rotacaoDireita(RBTree **raiz, RBTree *y) {
    RBTree *x = y->esquerda; // Define x como filho esquerdo de y
    y->esquerda = x->direita; // Move a subárvore direita de x para a esquerda de y
    if (x->direita != NULL) {
        x->direita->pai = y; // Ajusta o pai da subárvore direita de x
    }
    x->pai = y->pai; // Liga x ao pai de y
    // Atualiza as ligações do pai de y
    if (y->pai == NULL) { // Caso y seja a raiz
        *raiz = x;
    } else if (y == y->pai->direita) { // Caso y seja filho direito
        y->pai->direita = x;
    } else { // Caso y seja filho esquerdo
        y->pai->esquerda = x;
    }
    x->direita = y; // Coloca y à direita de x
    y->pai = x; // Atualiza o pai de y
}

// Insere um novo nó na árvore rubro-negra
void insercao(RBTree **raiz, RBTree *novoNo) {
    RBTree *y = NULL; // Inicializa o ponteiro y como nulo
    RBTree *x = *raiz; // Inicializa x com a raiz da árvore
    // Percorre a árvore para encontrar a posição de inserção
    while (x != NULL) {
        y = x; // Mantém o rastreamento do nó pai
        // Decide se vai para a subárvore esquerda ou direita
        if (novoNo->dado.idade < x->dado.idade) {
            x = x->esquerda; // Vai para a esquerda
        } else {
            x = x->direita; // Vai para a direita
        }
    }
    // Define o pai do novo nó
    novoNo->pai = y;
    // Insere o novo nó na posição correta
    if (y == NULL) {
        *raiz = novoNo; // Caso a árvore esteja vazia
    } else if (novoNo->dado.idade < y->dado.idade) {
        y->esquerda = novoNo; // Insere à esquerda
    } else {
        y->direita = novoNo; // Insere à direita
    }
    // Define a cor do novo nó como vermelho
    novoNo->cor = VERMELHO;
    // Chama a função de balanceamento para ajustar a árvore
    balanceamento(raiz, novoNo);
}

// Função para balancear a árvore após a inserção
void balanceamento(RBTree **raiz, RBTree *novoNo) {
    // Continua enquanto o novo nó não é a raiz e ambos, novo nó e seu pai, são vermelhos
    while (novoNo != *raiz && novoNo->cor == VERMELHO && novoNo->pai->cor == VERMELHO) {
        RBTree *pai = novoNo->pai;
        RBTree *avo = pai->pai;
        // Se o pai do novo nó é filho esquerdo do avô
        if (pai == avo->esquerda) {
            RBTree *tio = avo->direita;
            // Caso 1: Tio é vermelho
            if (tio != NULL && tio->cor == VERMELHO) {
                tio->cor = PRETO;
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                novoNo = avo;
            } else {
                // Caso 2: Novo nó é filho direito
                if (novoNo == pai->direita) {
                    novoNo = pai;
                    rotacaoEsquerda(raiz, novoNo);
                }
                // Caso 3: Novo nó é filho esquerdo
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoDireita(raiz, avo);
            }
        } else {
            // Caso simétrico: Pai é filho direito do avô
            RBTree *tio = avo->esquerda;
            // Caso 1: Tio é vermelho (simétrico)
            if (tio != NULL && tio->cor == VERMELHO) {
                tio->cor = PRETO;
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                novoNo = avo;
            } else {
                // Caso 2: Novo nó é filho esquerdo (simétrico)
                if (novoNo == pai->esquerda) {
                    novoNo = pai;
                    rotacaoDireita(raiz, novoNo);
                }
                // Caso 3: Novo nó é filho direito (simétrico)
                pai->cor = PRETO;
                avo->cor = VERMELHO;
                rotacaoEsquerda(raiz, avo);
            }
        }
    }
    // A raiz da árvore sempre deve ser preta
    (*raiz)->cor = PRETO;
}

//Imprimir a arvore em ordem
void printInOrder(RBTree *raiz) {
    if (raiz != NULL) {
        printInOrder(raiz->esquerda); // Primeiro, visita a subárvore esquerda recursivamente
        printf("Nome: %s\n", raiz->dado.nome); // Imprime o nome do nó atual
        printf("Idade: %d\n", raiz->dado.idade); // Imprime a idade do nó atual
        printInOrder(raiz->direita); // Depois, visita a subárvore direita recursivamente
    }
}


