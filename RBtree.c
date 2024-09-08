#include <stdio.h>
#include <stdlib.h>
#include "RBtree.h"

arvore no_null; // Variável global para nó duplo preto

/** Inicializa a raiz da árvore e define o nó duplo preto
 * @param raiz ponteiro duplo para a raiz da árvore
 */
void inicializar(arvore *raiz)
{
    *raiz = NULL;

    no_null = (arvore)malloc(sizeof(struct no));
    no_null->cor = DUPLO_PRETO;
    no_null->chave = 0;
    no_null->esq = NULL;
    no_null->dir = NULL;
}

/** Insere um elemento na árvore
 * @param raiz ponteiro duplo para a raiz da árvore
 * @param valor valor a ser inserido
 */
void inserir(arvore *raiz, int valor)
{
    arvore posicao, pai, novo;

    posicao = *raiz;
    pai = NULL;

    while (posicao != NULL) // Busca a posição a inserir o elemento
    {
        pai = posicao;

        if (valor > posicao->chave)
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }

    // Achou
    novo = (arvore)malloc(sizeof(struct no));
    novo->chave = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    if (ehRaiz(novo)) // Árvore vazia
        *raiz = novo;

    else
    {
        if (valor > pai->chave)
            pai->dir = novo;
        else
            pai->esq = novo;
    }

    ajustar(raiz, novo);
}

/** Ajuste (pós inserção) do balanceamento da árvore
 * @param raiz ponteiro duplo para a raiz absoluta da árvore
 * @param elemento ponteiro para o elemento recém inserido na árvore
 */
void ajustar(arvore *raiz, arvore elemento)
{
    while (cor(pai(elemento)) == VERMELHO && cor(elemento) == VERMELHO)
    {
        if (cor(tio(elemento)) == VERMELHO) // Recolorir
        {
            tio(elemento)->cor = PRETO;
            pai(elemento)->cor = PRETO;
            avo(elemento)->cor = VERMELHO;
            elemento = avo(elemento);
            continue;
        }

        switch (rotacao(elemento)) // Verifica caso de rotação
        {
        case SIMPLES_DIREITA:
            rotacaoSimplesDireita(raiz, avo(elemento));
            pai(elemento)->cor = PRETO;
            irmao(elemento)->cor = VERMELHO;
            continue;
            break;

        case SIMPLES_ESQUERDA:
            rotacaoSimplesEsquerda(raiz, avo(elemento));
            pai(elemento)->cor = PRETO;
            irmao(elemento)->cor = VERMELHO;
            continue;
            break;

        case DUPLA_DIREITA:
            rotacaoDuplaDireita(raiz, avo(elemento));
            elemento->cor = PRETO;
            elemento->esq->cor = VERMELHO;
            elemento->dir->cor = VERMELHO;
            continue;
            break;

        case DUPLA_ESQUERDA:
            rotacaoDuplaEsquerda(raiz, avo(elemento));
            elemento->cor = PRETO;
            elemento->esq->cor = VERMELHO;
            elemento->dir->cor = VERMELHO;
            continue;
            break;
        }
    }

    (*raiz)->cor = PRETO; // Atualiza a nova raiz para a cor preta
}

/** Remove um elemento da árvore
 * @param raiz ponteiro duplo para a raiz absoluta da árvore
 * @param valor valor a ser removido
 */
void remover(arvore *raiz, int valor)
{
    arvore posicao;
    posicao = *raiz;

    int encerrarWhile = 0;

    while (posicao != NULL) // Busca pelo elemento
    {
        if (valor == posicao->chave) // Achou o elemento
        {
            int valor;

            switch (filhos(posicao))
            {

            case NENHUM:
                if (ehRaiz(posicao))
                    *raiz = NULL;

                else if (cor(posicao) == VERMELHO)
                {
                    arvore *filhoPai = ehFilhoEsquerdo(posicao) ? &(posicao->pai->esq) : &(posicao->pai->dir);
                    *filhoPai = NULL;
                }

                else // Gera nó duplo preto
                {
                    no_null->pai = posicao->pai;

                    arvore *filhoPai = ehFilhoEsquerdo(posicao) ? &(posicao->pai->esq) : &(posicao->pai->dir);
                    *filhoPai = no_null;

                    reajustar(raiz, no_null);
                }

                break;

            case ESQUERDO:
                posicao->esq->cor = PRETO;
                posicao->esq->pai = posicao->pai;

                if (ehRaiz(posicao))
                    *raiz = posicao->esq;

                else
                {
                    arvore *filhoPai = ehFilhoEsquerdo(posicao) ? &(posicao->pai->esq) : &(posicao->pai->dir);
                    *filhoPai = posicao->esq;
                }

                break;

            case DIREITO:

                posicao->dir->cor = PRETO;
                posicao->dir->pai = posicao->pai;

                if (ehRaiz(posicao))
                    *raiz = posicao->dir;
                else
                {
                    arvore *filhoPai = ehFilhoEsquerdo(posicao) ? &(posicao->pai->esq) : &(posicao->pai->dir);
                    *filhoPai = posicao->dir;
                }

                break;

            case AMBOS:

                valor = maiorValor(posicao->esq);

                remover(raiz, valor);

                posicao->chave = valor;

                break;
            }

            break;
        }

        // Busca
        posicao = valor > posicao->chave ? posicao->dir : posicao->esq;
    }
};

/** Ajuste (pós remoção) do balanceamento da árvore em caso de duplo preto
 * @param raiz ponteiro duplo para a raiz absoluta da árvore
 * @param elemento ponteiro para o elemento duplo preto
 */
void reajustar(arvore *raiz, arvore elemento)
{
    // caso 1 (resolve duplo preto)
    if (ehRaiz(elemento))
    {
        resolverDuploPreto(elemento);
        return;
    }

    // caso 2
    else if (
        cor(pai(elemento)) == PRETO &&
        cor(irmao(elemento)) == VERMELHO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO)
    {
        irmao(elemento)->cor = PRETO;
        pai(elemento)->cor = VERMELHO;

        if (ehFilhoEsquerdo(elemento))
        {
            rotacaoSimplesEsquerda(raiz, pai(elemento));
        }
        else
            rotacaoSimplesDireita(raiz, pai(elemento));

        reajustar(raiz, elemento);
        return;
    }

    // caso 3
    else if (
        cor(pai(elemento)) == PRETO &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO)
    {

        pai(elemento)->cor = DUPLO_PRETO;
        irmao(elemento)->cor = VERMELHO;

        resolverDuploPreto(elemento);

        reajustar(raiz, elemento->pai);

        return;
    }

    // caso 4 (resolve duplo preto)
    else if (
        cor(pai(elemento)) == VERMELHO &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == PRETO)
    {
        pai(elemento)->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;
        resolverDuploPreto(elemento);

        return;
    }

    // caso 5 (filho esquerdo)
    else if (
        ehFilhoEsquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == VERMELHO &&
        cor(irmao(elemento)->dir) == PRETO)
    {

        irmao(elemento)->esq->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;

        rotacaoSimplesDireita(raiz, irmao(elemento));

        reajustar(raiz, elemento);

        printf("%d", (*raiz)->chave);

        return;
    }

    // caso 5 (filho direito)
    else if (
        !ehFilhoEsquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == PRETO &&
        cor(irmao(elemento)->dir) == VERMELHO)
    {
        irmao(elemento)->dir->cor = PRETO;
        irmao(elemento)->cor = VERMELHO;

        rotacaoSimplesEsquerda(raiz, irmao(elemento));

        reajustar(raiz, elemento);
        return;
    }

    // caso 6 (filho esquerdo) - resolve duplo preto
    else if (
        ehFilhoEsquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->dir) == VERMELHO)
    {

        irmao(elemento)->cor = cor(pai(elemento));
        irmao(elemento)->dir->cor = PRETO;
        pai(elemento)->cor = PRETO;

        rotacaoSimplesEsquerda(raiz, elemento->pai);

        resolverDuploPreto(elemento);

        return;
    }
    // caso 6b (filho direito) - resolve duplo preto
    else if (
        !ehFilhoEsquerdo(elemento) &&
        cor(irmao(elemento)) == PRETO &&
        cor(irmao(elemento)->esq) == VERMELHO)
    {

        irmao(elemento)->cor = cor(pai(elemento));
        irmao(elemento)->esq->cor = PRETO;
        pai(elemento)->cor = PRETO;

        rotacaoSimplesDireita(raiz, elemento->pai);

        resolverDuploPreto(elemento);

        return;
    }
}

/** Retira a ocorrência de duplo preto da árvore
 * @param elemento ponteiro para o nó duplo preto
 */
void resolverDuploPreto(arvore elemento)
{
    if (elemento == no_null)
    {
        arvore *filhoPai = ehFilhoEsquerdo(elemento) ? &(elemento->pai->esq) : &(elemento->pai->dir);
        *filhoPai = NULL;
    }
    else
        elemento->cor = PRETO;
}

/** Atualiza ponteiros para uma rotação simples esquerda
 * @param raiz endereço para a raiz absoluta da árvore
 * @param pivot ponteiro para o nó pivot da rotação
 */
void rotacaoSimplesEsquerda(arvore *raiz, arvore pivot)
{
    arvore u, t1;

    // Definição
    u = pivot->dir;
    t1 = u->esq;

    arvore *filhoPai = ehFilhoEsquerdo(pivot) ? &(pivot->pai->esq) : &(pivot->pai->dir);

    // Reposicionamento
    u->pai = pivot->pai;
    u->esq = pivot;

    pivot->pai = u;
    pivot->dir = t1;

    if (t1)
        t1->pai = pivot;

    if (ehRaiz(u))
        *raiz = u;
    else
        *filhoPai = u;
}

/** Atualiza ponteiros para uma rotação simples direita
 * @param raiz endereço para a raiz absoluta da árvore
 * @param pivot ponteiro para o nó pivot da rotação
 */
void rotacaoSimplesDireita(arvore *raiz, arvore pivot)
{
    arvore u, t1;

    // Definição
    u = pivot->esq;
    t1 = u->dir;

    int pivotEhFilhoEsquerdo = ehFilhoEsquerdo(pivot);

    // Reposicionamento
    u->dir = pivot;
    u->pai = pivot->pai;

    pivot->esq = t1;
    pivot->pai = u;

    if (t1)
        t1->pai = pivot;

    if (ehRaiz(u))
        *raiz = u;
    else
    {
        if (pivotEhFilhoEsquerdo)
            pai(u)->esq = u;
        else
            pai(u)->dir = u;
    }
}

/** Atualiza ponteiros para uma rotação dupla esquerda
 * @param raiz endereço para a raiz absoluta da árvore
 * @param pivot ponteiro para o nó pivot da rotação
 */
void rotacaoDuplaEsquerda(arvore *raiz, arvore pivot)
{
    rotacaoSimplesDireita(raiz, pivot->dir);
    rotacaoSimplesEsquerda(raiz, pivot);
}

/** Atualiza ponteiros para uma rotação dupla direita
 * @param raiz endereço para a raiz absoluta da árvore
 * @param pivot ponteiro para o nó pivot da rotação
 */
void rotacaoDuplaDireita(arvore *raiz, arvore pivot)
{
    rotacaoSimplesEsquerda(raiz, pivot->esq);
    rotacaoSimplesDireita(raiz, pivot);
}

/** Verifica se o elemento é raiz absoluta da árvore
 * @param elemento ponteiro para o elemento
 * @return 0 para falso, 1 para verdadeiro
 */
int ehRaiz(arvore elemento)
{
    return elemento->pai == NULL;
}

/** Verifica se o elemento é filho esquerdo de seu pai
 * @param elemento ponteiro para o elemento
 * @return 0 para falso, 1 para verdadeiro
 */
int ehFilhoEsquerdo(arvore elemento)
{
    return !ehRaiz(elemento) && elemento == pai(elemento)->esq;
}

/** Obtém o maior valor de uma árvore relativa
 * @param elemento ponteiro para a raiz relativa
 * @return O maior valor a partir de uma raiz relativa
 */
int maiorValor(arvore elemento)
{
    if (elemento == NULL)
        return -1;

    if (elemento->dir == NULL)
        return elemento->chave;

    return maiorValor(elemento->dir);
}

/** Obtém o pai do elemento
 * @param elemento ponteiro para o elemento
 * @return Ponteiro para o pai do elemento
 */
arvore pai(arvore elemento)
{
    return elemento->pai;
}

/** Obtém o avô do elemento
 * @param elemento ponteiro para o elemento
 * @return Ponteiro para o avô do elemento
 */
arvore avo(arvore elemento)
{
    return pai(pai(elemento));
}

/** Obtém o irmão do elemento
 * @param elemento ponteiro para o elemento
 * @return Ponteiro para o irmão do elemento
 */
arvore irmao(arvore elemento)
{
    return ehFilhoEsquerdo(elemento) ? pai(elemento)->dir : pai(elemento)->esq;
}

/** Obtém o tio do elemento
 * @param elemento ponteiro para o elemento
 * @return Ponteiro para o tio do elemento
 */
arvore tio(arvore elemento)
{
    return irmao(pai(elemento));
}

/** Obtém a cor do elemento
 * @param elemento ponteiro para o elemento
 * @return Enum relativo a cor do elemento
 */
enum cor cor(arvore elemento)
{
    if (elemento == NULL || elemento->cor == PRETO)
        return PRETO;

    else
        return VERMELHO;
}

/** Obtém quais filhos o elemento possui
 * @param elemento ponteiro para o elemento
 * @return Enum relativo ao caso de filhos do elemento
 */
enum filhos filhos(arvore elemento)
{
    if (elemento == NULL)
        return -1;

    if (elemento->esq == NULL && elemento->dir == NULL)
        return NENHUM;

    if (elemento->esq != NULL && elemento->dir == NULL)
        return ESQUERDO;

    if (elemento->esq == NULL && elemento->dir != NULL)
        return DIREITO;

    if (elemento->esq != NULL && elemento->dir != NULL)
        return AMBOS;
}

/** Obtém qual o tipo de rotação necessária para o ajuste do elemento
 * @param elemento ponteiro para o elemento
 * @return Enum relativo ao caso de rotação do elemento
 */
enum rotacao rotacao(arvore elemento)
{
    if (elemento == NULL)
        return -1;

    if (ehFilhoEsquerdo(elemento) && ehFilhoEsquerdo(pai(elemento)))
        return SIMPLES_DIREITA;

    if (!ehFilhoEsquerdo(elemento) && !ehFilhoEsquerdo(pai(elemento)))
        return SIMPLES_ESQUERDA;

    if (!ehFilhoEsquerdo(elemento) && ehFilhoEsquerdo(pai(elemento)))
        return DUPLA_DIREITA;

    if (ehFilhoEsquerdo(elemento) && !ehFilhoEsquerdo(pai(elemento)))
        return DUPLA_ESQUERDA;
}

/** Imprime as chaves e o tipo de cor na ordem preorder */
void preorder(arvore raiz)
{
    if (raiz == NULL)
        return;

    // Cor do Texto
    const char *corTexto;
    const char *corTextoPadrao = "\033[0m";
    const char *tipo;

    if (cor(raiz) == VERMELHO)
    {
        corTexto = "\033[1;31m";
        tipo = "R";
    }

    else if (cor(raiz) == PRETO)
    {
        corTexto = "\033[0m";
        tipo = "B";
    }

    printf("%s[%d %s]%s", corTexto, raiz->chave, tipo, corTextoPadrao);
    preorder(raiz->esq);
    preorder(raiz->dir);
}

/** Imprime as propriedades do elemento */
void propriedades(arvore raiz, int valor)
{
    if (raiz == NULL) // Árvore vazia
        return;

    else if (raiz->chave == valor) // Achou o elemento
    {
        int chave = raiz->chave;
        int chaveEsq = raiz->esq == NULL ? -1 : raiz->esq->chave;
        int chaveDir = raiz->dir == NULL ? -1 : raiz->dir->chave;
        enum cor cor = raiz->cor;

        const char *corTexto;
        corTexto = cor == VERMELHO ? "\e[0;31m" : "\033[0m";

        if (ehRaiz(raiz))
            printf("Elemento \e[0;36m%d\033[0m | \e[0;36mRaiz\033[0m | esq \e[0;36m%d\033[0m | dir \e[0;36m%d\033[0m | cor \e%s%d\033[0m\n", chave, chaveEsq, chaveDir, corTexto, cor);

        else
        {
            int pai = raiz->pai->chave;
            printf("Elemento \e[0;36m%d\033[0m | pai \e[0;36m%d\033[0m | esq \e[0;36m%d\033[0m | dir \e[0;36m%d\033[0m | cor %s%d\033[0m\n", chave, pai, chaveEsq, chaveDir, corTexto, cor);
        }
    }

    else // Busca
    {
        propriedades(raiz->esq, valor);
        propriedades(raiz->dir, valor);
    }
}

/** Executa o loop de interações na main */
void run()
{
    arvore raiz;

    inicializar(&raiz);

    while (1)
    {
        printf("\n");
        printf("(1) Inserir\n");
        printf("(2) Preorder\n");
        printf("(3) Remover\n");
        printf("(4) Propriedades\n");
        printf("(99) Sair\n");

        int operacao, input;
        printf("\e[0;32m> ");
        scanf("%d", &operacao);
        printf("\033[0m");

        switch (operacao)
        {
        case 1:
            scanf("%d", &input);
            inserir(&raiz, input);
            break;

        case 2:
            preorder(raiz);
            printf("\n");
            break;

        case 3:
            scanf("%d", &input);
            remover(&raiz, input);
            break;

        case 4:
            scanf("%d", &input);
            propriedades(raiz, input);
            break;

        case 99:
            return;
            break;

        default:
            printf("\n");
            printf("\033[1;33mOpcao invalida, tente novamente\n\033[0m");
            printf("\n");
            continue;
        }
    }
}
