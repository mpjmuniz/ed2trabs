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
#include "no_interno.h"

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
		char *nome_arquivo_dados, int *pont_folha, int *encontrou) {

	*encontrou = 0;
	*pont_folha = 0;
	
	int rc,
		posicao,
		i,
		m,
		p_interno,
		//proximo é nó folha
		prox_folha = 0;
	FILE *indice,
		 *dados;
	Metadados *meta;
	NoFolha *folha;
	NoInterno *no_Atual;

	meta = le_arq_metadados(nome_arquivo_metadados);
	assert(meta != NULL && "Arquivo de metadados inválido, ou função errada.\n");

	*pont_folha = meta->pont_raiz;
	prox_folha = meta->raiz_folha;

	indice = fopen(nome_arquivo_indice, "rb");
	
	while(!prox_folha){
		i = 0;

		rc = fseek(indice, *pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		no_Atual = le_no_interno(indice);
		if(no_Atual == NULL) break;

		m = no_Atual->m;

		while(i < m){
			if(cod_cli >= no_Atual->chaves[i]){
				i++;
			}else{
				*pont_folha = no_Atual->p[i];
				i = m + 1;
			}
		}

		if(i == m) {
			*pont_folha = no_Atual->p[m];
		}

		prox_folha = no_Atual->aponta_folha;
		libera_no_interno(no_Atual);
	}

	//Busca na folha
	dados = fopen(nome_arquivo_dados, "rb");

	rc = fseek(dados, *pont_folha, SEEK_SET);
	assert(rc == 0 && "Falha no seek.\n");

	folha = le_no_folha(dados);
	assert(folha != NULL && "Folha é nula.\n");
	
	*encontrou = buscar_cliente(folha->clientes, folha->m, cod_cli, &posicao);

	libera_no_folha(folha);
	
	fclose(dados);
	fclose(indice);
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
