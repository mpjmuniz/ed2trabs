#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include <stdlib.h>
#include <stdlib.h>

#include "arvore_b.h"
#include "metadados.h"
#include "no.h"

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados, int *pont, int *encontrou)
{
	*encontrou = 0;
	//TODO: Inserir aqui o codigo do algoritmo
	//Abre arquivo de metadados e lê a raiz
	Metadados *meta = le_arq_metadados(nome_arquivo_metadados);
	FILE *fnos = fopen(nome_arquivo_dados, "r+b");
	fseek(fnos, meta->pont_raiz, SEEK_SET);
	*pont = meta->pont_raiz;
	No *no = le_no(fnos);
	int i;
	for(i = 0; i<no->m && cod_cli<no->clientes[i]->cod_cliente; i++);
	if(cod_cli == no->clientes[i]->cod_cliente) *encontrou = 1;
	//Libera variáveis
	fclose(fnos);
	free(meta);
	free(no);
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
