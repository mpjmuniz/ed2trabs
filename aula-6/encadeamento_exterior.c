#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <string.h>
#include <errno.h>

#include "encadeamento_exterior.h"
#include "lista_compartimentos.h"
#include "lista_clientes.h"
#include <assert.h>
#include <stdlib.h>

void cria_hash(char *nome_arquivo_hash, int tam)
{
	int i = 0;
	ListaCompartimentos *lc = cria_compartimentos(0);
	
	for(i = 0; i < tam; i++){
		adcn_compartimento(lc, -1);
	}

	salva_compartimentos(nome_arquivo_hash, lc);
	libera_compartimentos(lc);
}

int busca(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados) {
	/*
	 *	Busca por um registro de chave x:
	 *	Calcular o endereço aplicando a função h(x)
	 *	Percorrer a lista encadeada associada ao endereço
	 *	Comparar a chave de cada nó da lista encadeada com a chave
	 *	x, até encontrar o nó desejado
	 *	Se final da lista for atingido, registro não está lá
	 *
	 * */

	Cliente *cl;

	int end = cod_cli % 7, 
		rc = -1,
		seguinte = 0;
	
	FILE *arq_hash = fopen(nome_arquivo_hash, "rb");

	rc = fseek(arq_hash, (end) * tamanho_compartimento(), SEEK_SET);
	assert(rc == 0 && "Falha no seek\n");

	FILE *arq_clientes = fopen(nome_arquivo_dados, "rb");
	
	CompartimentoHash *lido = le_compartimento(arq_hash);

	//endereço no arquivo = lido->prox
	seguinte = lido->prox;

	while(seguinte != -1){
		rc = fseek(arq_clientes, seguinte * tamanho_cliente(), SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		cl = le_cliente(arq_clientes);
		if(cl->cod_cliente == cod_cli){
			if(cl->flag == OCUPADO){
				fclose(arq_hash);
				fclose(arq_clientes);
				free(cl);
				free(lido);
				return seguinte;
			} 
		}
		seguinte = cl->prox;
		free(cl);
	} 

	free(lido);
    fclose(arq_hash);
	fclose(arq_clientes);
	return -1;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int num_registros) {

	Cliente *cl = NULL, *atual = NULL;

	//	1. Calcular o endereço aplicando a função h(x)
	int end = cod_cli % 7, 
		end_escrita = num_registros * tamanho_cliente(),
		rc = -1,
		seguinte = 0,
		ultimo = 0;
	
	FILE *arq_hash = fopen(nome_arquivo_hash, "r+b");
	FILE *arq_clientes = fopen(nome_arquivo_dados, "r+b");

	CompartimentoHash *lido;
	
	/* 1ª tentativa: Obtenção direta do registro via compartimento hash */

	//cursor na tabela de dispersão posicionado
	rc = fseek(arq_hash, end * tamanho_compartimento(), SEEK_SET);
	assert(rc == 0 && "Falha no seek\n");

	lido = le_compartimento(arq_hash);

	//endereço no arquivo de clientes = lido->prox
	seguinte = lido->prox;
	
	/* 	
	 *  1ª Iteração: fim da 1ª tentativa
	 * 	2ª iteração em diante: 2ª tentativa, 
	 * 		que é a busca do registro na lista de tratamento das colisões
	 *
	 * */

	while(seguinte != -1){
		rc = fseek(arq_clientes, seguinte * tamanho_cliente(), SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		atual = le_cliente(arq_clientes);
				
		if(atual->cod_cliente == cod_cli){
			if(atual->flag == OCUPADO){
				fclose(arq_clientes);
				fclose(arq_hash);
				free(atual);
				free(lido);
				//Já existe, erro.
				return -1;
			}
		}

		if(atual->flag == LIBERADO){
			//Se um dos registros com chave congruente % 7 está liberado,
			//esta é a posição para inserir
			end_escrita = seguinte * tamanho_cliente();
			seguinte = atual->prox;
			free(atual);
			break;
		}

		ultimo = seguinte;
		seguinte = atual->prox;

		free(atual);
	}
	
	/* Escrita do registro inserido*/

	if(atual == NULL){
		/*Atualização do campo próximo no compartimento hash, caso a lista não exista*/
		rc = fseek(arq_clientes, end_escrita, SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		cl = cliente(cod_cli, nome_cli, -1, OCUPADO);
		salva_cliente(cl, arq_clientes);

		lido->prox = end_escrita / tamanho_cliente();
		
		rc = fseek(arq_hash, end * tamanho_compartimento(), SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		salva_compartimento(lido, arq_hash);
	} else {
		/*Escrita do registro sendo inserido*/
		rc = fseek(arq_clientes, end_escrita, SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		cl = cliente(cod_cli, nome_cli, seguinte , OCUPADO);
		salva_cliente(cl, arq_clientes);

		/*Escrita do registro anterior, com seu prox atualizado*/
		rc = fseek(arq_clientes, ultimo * tamanho_cliente(), SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");

		atual = le_cliente(arq_clientes);
		atual->prox = end_escrita / tamanho_cliente();
		atual->flag = OCUPADO;

		rc = fseek(arq_clientes, ultimo * tamanho_cliente(), SEEK_SET);
		assert(rc == 0 && "Falha no seek\n");
		
		salva_cliente(atual, arq_clientes);
		free(atual);
	}

	fclose(arq_hash);
	fclose(arq_clientes);
	
	free(cl);
	free(lido);

    return end_escrita / tamanho_cliente();
}

int exclui(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados)
{
    int end = busca(cod_cli, nome_arquivo_hash, nome_arquivo_dados),
    	rc = -1;
    FILE *arq_clientes;
	Cliente *cl;
		
    if(end != -1){
		arq_clientes = fopen(nome_arquivo_dados, "r+b");

    	rc = fseek(arq_clientes, end * tamanho_cliente(), SEEK_SET);
    	assert(rc == 0 && "Falha no seek\n");
		
		cl = le_cliente(arq_clientes);
		cl->flag = LIBERADO;

    	rc = fseek(arq_clientes, end * tamanho_cliente(), SEEK_SET);
    	assert(rc == 0 && "Falha no seek\n");

    	salva_cliente(cl, arq_clientes);

		fclose(arq_clientes);
    	free(cl);
	}

	return end;
}
