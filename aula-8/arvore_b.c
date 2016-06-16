#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include <stdlib.h>
#include <assert.h>

#include "arvore_b.h"
#include "metadados.h"
#include "no.h"

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados, int *pont, int *encontrou)
{
	int *ponteiro_interno = malloc(sizeof(int)),
		rc = -1,
		i,
		pos;
	FILE *fnos = fopen(nome_arquivo_dados, "r+b");
	No *no;
	Metadados *meta;

	assert(fnos != NULL && "Falha na abertura do arquivo.\n");
	
	//Abre arquivo de metadados e lê a raiz
	meta = le_arq_metadados(nome_arquivo_metadados);
	rc = fseek(fnos, meta->pont_raiz, SEEK_SET);
	assert(rc == 0 && "Falha no seek.\n");

	pont = NULL;
	*encontrou = 0;
	*ponteiro_interno = meta->pont_raiz;

	while(ponteiro_interno != NULL){
		i = 1;
		pos = 1;
		pont = ponteiro_interno;
		
		no = le_no(fnos);
		if(no == NULL) break;

		while(i <= no->m){
			if(cod_cli > no->clientes[i]->cod_cliente){
				i++;
				pos = i + 1;
			} else {
				if(cod_cli == no->clientes[i]->cod_cliente){
					ponteiro_interno = NULL;
					*encontrou = 1;
				} else {
					*ponteiro_interno = no->p[i - 1];
				}
				i = no->m + 2;

			}
		}

		if(i == no->m + 1){
			*ponteiro_interno = no->p[no->m];
		}
	}

	//Libera variáveis
	fclose(fnos);
	if(ponteiro_interno != NULL) free(ponteiro_interno);
	free(meta);
	libera_no(no);
    return i;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados)
{
	//TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados)
{
	//TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
