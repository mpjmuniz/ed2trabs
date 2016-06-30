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

/*
 *	TODO:
 *		Exclusão:
 *			- Concatenação
 *				- Concluir sub-operações 
 *				- Gerenciar memória
 *			- Redistribuição
 *				- Implementar
 *
 * */


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

inline Cliente *_remover_cliente(NoFolha* folha, int posicao_inicial){
	int i = posicao_inicial;
	Cliente *removido = folha->clientes[posicao_inicial];
	do{
		folha->clientes[i] = folha->clientes[i + 1];
	} while(++i < folha->m);
	folha->m--;
	folha->clientes[folha->m] = NULL;
	return removido;
}

inline void _adicionar_cliente(NoInterno *pai, NoFolha *folha, NoFolha *irmao, Cliente *novo){
	int i = folha->m - 1,
		j = 0,
		chave = novo->cod_cliente;

	while(i >= 0 && folha->clientes[i]->cod_cliente > novo->cod_cliente){
		folha->clientes[i + 1] = folha->clientes[i];
		i--;
	}

	folha->clientes[i + 1] = novo;
	folha->m++;

	if(i == 0){
		for(j = 0; j < pai->m; j++){
			if(pai->chaves[j] == chave){
				pai->chaves[j] = irmao->clientes[0]->cod_cliente;
			}
		}
	}
}

inline void _redistribuir(NoInterno *pai, NoFolha *folha, NoFolha *irmao){
	/*
	 *	Estado: irmao está à direita da folha, pai é pai de ambos
	 * */
	int i = 0;

	for(i = 0; i < irmao->m - D; i++){
		_adicionar_cliente(pai, folha, irmao, _remover_cliente(irmao, i));
	}
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
		char *nome_arquivo_dados) {
	int pont_folha = 0,
		encontrou = 0,
		rc = -1,
		posicao = 0,
		i = 0;
	FILE *dados,
		 *indices;
	NoFolha *folha,
			*irmao;
	NoInterno *pai;
	
	/*
	 *	Estado: posicao guarda a posicao onde o registro está na página
	 *			pont_folha guarda o ponteiro para a folha em que o registro está
	 *			encontrou diz se o registro está na árvore ou não
	 *
	 * */
	posicao = busca(cod_cli, nome_arquivo_metadados, nome_arquivo_indice,
			nome_arquivo_dados, &pont_folha, &encontrou);

	if(encontrou){
		/*
		 *	Se o registro está na árvore, só é necessário removẽ-lo da folha
		 * */
		dados = fopen(nome_arquivo_dados, "r+b");

		//Seek para leitura
		rc = fseek(dados, pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		folha = le_no_folha(dados);
		
		free(_remover_cliente(folha, posicao));

		if(folha->m < D){

			rc = fseek(dados, folha->pont_prox, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			irmao = le_no_folha(dados);

			indices = fopen(nome_arquivo_indice, "r+b");

			rc = fseek(indices, folha->pont_pai, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			pai = le_no_interno(indices);

			if(folha->m + irmao->m < 2 * D){
				//Concatenação
			} else {
				//Usar redistribuição
				_redistribuir(pai, folha, irmao);
			}

			rc = fseek(indices, folha->pont_pai, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			salva_no_interno(pai, indices);

			rc = fseek(dados, folha->pont_prox, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			salva_no_folha(irmao, dados);

			if(pai != NULL) libera_no_interno(pai);
			if(irmao != NULL) libera_no_folha(irmao);
			fclose(indices);
		}

		rc = fseek(dados, pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");
		salva_no_folha(folha, dados);

		if(folha != NULL) libera_no_folha(folha);
		fclose(dados);
	} 

    return pont_folha;
}
