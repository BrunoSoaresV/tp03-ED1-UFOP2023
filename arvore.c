#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

// Aloca um novo nó da Árvore Rubro-Negra
RBTree* alocarArvore() {
    return (RBTree*)malloc(sizeof(RBTree));
}

// Função para leitura e inicialização da árvore
RBTree* leArvore() {
    RBTree *novoNo = alocarArvore();
    if (novoNo) {
        // Define a cor do novo nó como VERMELHO. Na Árvore Rubro-Negra, novos nós são
        // inseridos como vermelhos para manter as propriedades de balanceamento da árvore,
        // já que adicionar um nó vermelho causa menos violações das regras da árvore.
        novoNo->cor = VERMELHO; 
        // Inicializa os ponteiros para os filhos esquerdo e direito e o pai como NULL.
        // Isso é feito porque o novo nó ainda não está conectado na árvore, então seus
        // ponteiros para os filhos e o pai ainda não têm referências válidas.
        novoNo->esquerda = novoNo->direita = novoNo->pai = NULL;
    }
    return novoNo; // Retorna o nó alocado e inicializado
}

// Desaloca a Árvore
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

// Função auxiliar para encontrar o nó mínimo em uma subárvore
RBTree* encontrarMinimo(RBTree* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;  // Move-se para o nó mais à esquerda na subárvore
    }
    return no;  // Retorna o nó mínimo encontrado
}

// Função auxiliar para substituir um nó por outro
void substituirNo(RBTree **raiz, RBTree *alvo, RBTree *substituto) {
    if (alvo->pai == NULL) {
        *raiz = substituto;  // Se o nó a ser substituído é a raiz, substitui a raiz pela nova raiz (substituto)
    } else if (alvo == alvo->pai->esquerda) {
        alvo->pai->esquerda = substituto;  // Se o nó a ser substituído é um filho esquerdo, substitui o pai->esquerda pelo substituto
    } else {
        alvo->pai->direita = substituto;  // Se o nó a ser substituído é um filho direito, substitui o pai->direita pelo substituto
    }
    if (substituto != NULL) {
        substituto->pai = alvo->pai;  // Atualiza o pai do substituto para o pai do nó a ser substituído
    }
}

// Função para realizar a remoção de um nó na árvore rubro-negra
void remocao(RBTree **raiz, int idade) {
    RBTree *no = *raiz;
    // Procura o nó a ser removido
    while (no != NULL) {
        if (idade < no->dado.idade) {
            no = no->esquerda;
        } else if (idade > no->dado.idade) {
            no = no->direita;
        } else {
            break; // Nó encontrado
        }
    }

    if (no == NULL) {
        printf("Idade %d não encontrada na árvore.\n", idade);
        return;
    }

    RBTree *y = no;
    CorNo yOriginalCor = y->cor;
    RBTree *x;

    if (no->esquerda == NULL) {
        // Se o nó tem pelo menos um filho, atualiza o ponteiro para o filho não nulo
        x = no->direita;
        substituirNo(raiz, no, no->direita);
    } else if (no->direita == NULL) {
        // Se o nó tem apenas um filho, atualiza o ponteiro para o filho não nulo
        x = no->esquerda;
        substituirNo(raiz, no, no->esquerda);
    } else {
        // Se o nó tem ambos os filhos, encontra o sucessor (y) e ajusta os ponteiros
        y = encontrarMinimo(no->direita);
        yOriginalCor = y->cor;
        x = y->direita;
        if (y->pai == no) {
            if (x != NULL) {
                x->pai = y;
            }
        } else {
            // Remove o sucessor (y) da posição original e ajusta os ponteiros
            substituirNo(raiz, y, y->direita);
            y->direita = no->direita;
            y->direita->pai = y;
        }
        // Substitui o nó a ser removido (no) pelo seu sucessor (y) e ajusta os ponteiros
        substituirNo(raiz, no, y);
        y->esquerda = no->esquerda;
        y->esquerda->pai = y;
        y->cor = no->cor;
    }

    free(no);

    // Caso a cor original de y seja preta e o filho substituto (x) não seja nulo,
    // ajusta a árvore para manter as propriedades de árvore rubro-negra
    if (yOriginalCor == PRETO && x != NULL) {
        balanceamentoRemocao(raiz, x);
    }
}

// Função para balancear a árvore após a remoção
void balanceamentoRemocao(RBTree **raiz, RBTree *x) {
    while (x != *raiz && (x == NULL || x->cor == PRETO)) {
        // Continua enquanto não atingir a raiz e o nó atual (x) é nulo ou preto

        if (x == x->pai->esquerda) {
            // Se x é filho esquerdo do pai
            RBTree *irmao = x->pai->direita;

            if (irmao->cor == VERMELHO) {
                // Caso 1: O irmão é vermelho, troca as cores com o pai e rotaciona à esquerda
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                irmao = x->pai->direita;
            }

            if ((irmao->esquerda == NULL || irmao->esquerda->cor == PRETO) &&
                (irmao->direita == NULL || irmao->direita->cor == PRETO)) {
                // Caso 2: O irmão e os sobrinhos são pretos, colore o irmão de vermelho e sobe um nível
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->direita == NULL || irmao->direita->cor == PRETO) {
                    // Caso 3: O irmão tem um sobrinho vermelho à esquerda, ajusta as cores e rotaciona à direita no irmão
                    irmao->esquerda->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoDireita(raiz, irmao);
                    irmao = x->pai->direita;
                }

                // Caso 4: O sobrinho direito do irmão é vermelho, ajusta as cores e rotaciona à esquerda no pai
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->direita->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
                // Finaliza o loop
            }
        } else {
            // Caso simétrico: x é filho direito
            RBTree *irmao = x->pai->esquerda;

            if (irmao->cor == VERMELHO) {
                // Caso 1 (simétrico): O irmão é vermelho, troca as cores com o pai e rotaciona à direita
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                irmao = x->pai->esquerda;
            }

            if ((irmao->direita == NULL || irmao->direita->cor == PRETO) &&
                (irmao->esquerda == NULL || irmao->esquerda->cor == PRETO)) {
                // Caso 2 (simétrico): O irmão e os sobrinhos são pretos, colore o irmão de vermelho e sobe um nível
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esquerda == NULL || irmao->esquerda->cor == PRETO) {
                    // Caso 3 (simétrico): O irmão tem um sobrinho vermelho à direita, ajusta as cores e rotaciona à esquerda no irmão
                    irmao->direita->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(raiz, irmao);
                    irmao = x->pai->esquerda;
                }

                // Caso 4 (simétrico): O sobrinho esquerdo do irmão é vermelho, ajusta as cores e rotaciona à direita no pai
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->esquerda->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
                // Finaliza o loop
            }
        }
    }
    if (x != NULL) {
        x->cor = PRETO;  // Garante que a raiz da árvore seja preta
    }
}
