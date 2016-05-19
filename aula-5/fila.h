#ifndef __fila_h__
#define __fila_h__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
 *	Estrutura Nó para cada arquivo
 *	implementação da fila via lista duplamente encadeada
 *	@prox : próximo
 *	@ant : anterior
 *	@arq : arquivo deste nó
 */
struct NoArquivo;
typedef struct NoArquivo {
	struct NoArquivo *prox;
	struct NoArquivo *ant;
	char *arq;
} NoArquivo;

/*
 *  Estrutura da Fila em si
 *  prim: primeiro
 *  ult: último
 *  qtd: número de nós atualmente na fila
 */
typedef struct Fila {
	int qtd;
	NoArquivo *prim;
	NoArquivo *ult;
} Fila;

/*Cria uma nova fila*/
Fila *Fila_criar();

/*Destrói os elementos da fila, liberando-os*/
void Fila_destruir(Fila * fila);

/*Mantém os nós, mas libera os arquivos em cada um deles*/
void Fila_limpar(Fila * fila); 

/*Libera os arquivos e destrói os nós*/
void Fila_limpar_destruir(Fila * fila);

/*Retorna o tamanho da fila*/
#define Fila_qtd(A) ((A)->qtd)

/*Retorna o primeiro arquivo na fila*/
#define Fila_primeiro(A) ((A)->prim != NULL ? (A)->prim->arq : NULL)

/*Retorna o último da fila*/
#define Fila_ultimo(A) ((A)->ult != NULL ? (A)->ult->arq : NULL)

/*Enfileirar : Insere um novo arquivo de caminho @nome_arquivo no começo da @fila*/
void Fila_enflr(Fila * fila, char *nome_arquivo);

/* Desenfileirar : Remove o último arquivo da @fila*/
FILE *Fila_desnflr(Fila * fila);

/* Remove @arq da fila*/
FILE *Fila_remover(Fila * fila, NoArquivo * arq);

/*Itera a fila*/
#define FILA_ITERAR(L, S, M, V) NoArquivo *_no = NULL;\
				NoArquivo *V = NULL;\
for(V = _no = L->S; _no != NULL; V = _no = _no->M)

#endif
