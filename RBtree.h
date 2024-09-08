#ifndef RB_H
#define RB_H

#include <stdio.h>
#include <stdlib.h>

enum cor
{
    VERMELHO,
    PRETO,
    DUPLO_PRETO
};

enum rotacao
{
    SIMPLES_ESQUERDA,
    SIMPLES_DIREITA,
    DUPLA_ESQUERDA,
    DUPLA_DIREITA
};

enum filhos
{
    NENHUM,
    ESQUERDO,
    DIREITO,
    AMBOS
};

typedef struct no
{
    int chave;
    struct no *esq, *dir, *pai;
    enum cor cor;

} no;

typedef no *arvore;

void inicializar(arvore *raiz);

void inserir(arvore *raiz, int valor);
void ajustar(arvore *raiz, arvore elemento);

void remover(arvore *raiz, int valor);
void reajustar(arvore *raiz, arvore elemento);

void resolverDuploPreto(arvore elemento);

void rotacaoSimplesDireita(arvore *raiz, arvore pivot);
void rotacaoSimplesEsquerda(arvore *raiz, arvore pivot);
void rotacaoDuplaDireita(arvore *raiz, arvore pivot);
void rotacaoDuplaEsquerda(arvore *raiz, arvore pivot);

int ehRaiz(arvore elemento);
int ehFilhoEsquerdo(arvore elemento);
int maiorValor(arvore elemento);

arvore pai(arvore elemento);
arvore irmao(arvore elemento);
arvore tio(arvore elemento);
arvore avo(arvore elemento);

arvore maior(arvore elemento);

enum cor cor(arvore elemento);
enum filhos filhos(arvore elemento);
enum rotacao rotacao(arvore elemento);

void run();

void preorder(arvore raiz);
void propriedades(arvore raiz, int valor);

#endif