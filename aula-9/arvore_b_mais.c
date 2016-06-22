#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <assert.h>
#include <stdlib.h>

#include "arvore_b_mais.h"
#include "lista_clientes.h"
#include "metadados.h"
#include "no_folha.h"


int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int *pont_folha, int *encontrou)
{
	*encontrou = 0;
	*pont_folha = 0;
	
	int rc,
		posicao;
	FILE *metadados,
		 *indice,
		 *dados;
	Metadados *meta;
	NoFolha *folha;

	meta = le_arq_metadados(nome_arquivo_metadados);
	assert(meta != NULL && "Arquivo de metadados inválido, ou função errada.\n");

	if(meta->raiz_folha){
		dados = fopen(nome_arquivo_dados, "rb");

		*pont_folha = meta->pont_raiz;
		
		rc = fseek(dados, meta->pont_raiz, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		folha = le_no_folha(dados);
		assert(folha != NULL && "Folha é nula.\n");
		
		*encontrou = buscar_cliente(folha->clientes, folha->m, cod_cli, &posicao);

		libera_no_folha(folha);
		
		fclose(dados);
	} else {
		indice = fopen(nome_arquivo_indice, "rb");

		fclose(indice);
	}

	if(meta != NULL) free(meta);
	return posicao;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
	//TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
	//TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
