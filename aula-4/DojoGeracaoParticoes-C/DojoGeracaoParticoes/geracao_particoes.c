#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "geracao_particoes.h"
#include "nomes.h"
#include "lista_clientes.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

int existe_nao_congelado(int congelados[]){
	unsigned int i = 0;
	for(i = 0; i < sizeof(congelados)/sizeof(int); i++){
		if(!congelados[i]) return 1;
	}
	return 0;
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	FILE *in = fopen(nome_arquivo_entrada, "rb"),
		 *out;
	int i = 0, m = 0, gravado = 0, j = 0;
	ListaClientes *list = NULL;
	int congelados[M];
	
	//	1. Ler M registros do arquivo para a memória
	ler_clientes(in, &list, M, congelados);
	
	for(j = 0; j < conta_nomes(nome_arquivos_saida); j++){
		out = fopen(nome_arquivos_saida->nome, "wb");
		while(existe_nao_congelado(congelados)){
			//	2. Selecionar, no array em memória, o registro r com menor chave
			Cliente *menor = list->lista[0];
			for(i = 1; i < M; i++){
				if(congelados[i] || list->lista[i] == NULL) continue;
				if(menor == NULL || list->lista[i]->cod_cliente < menor->cod_cliente){
					menor = list->lista[i];
					m = i;
				}
			}

			//	3. Gravar o registro r na partição de saída
			if(menor != NULL){
				salva_cliente(menor, out);
			} else{
				continue;
			}
			
			//	4. Substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada
			if(list->lista[m] != NULL){
				gravado = list->lista[m]->cod_cliente;
				free(list->lista[m]);
				list->lista[m] = le_cliente(in);
				if(list->lista[m] != NULL){
					//	5. Caso a chave deste último seja menor do que a chave recém gravada, 
					//	considerá-lo congelado e ignorá-lo no restante do processamento
					if(list->lista[m]->cod_cliente < gravado){
						congelados[m] = 1;
					}

					//	6. Caso existam em memória registros não congelados, voltar ao passo 2
				} else{
					congelados[m] = 1;
				}
			} else {
				congelados[m] = 1;
			}
		}
		/*	7. Caso contrário:
			- fechar a partição de saída*/
		fclose(out);
		//	- descongelar os registros congelados
		for(i = 0; i < M; i++) congelados[i] = 0;

		//	- abrir nova partição de saída
		nome_arquivos_saida = nome_arquivos_saida->prox;
	}

	libera_clientes(list);	
	fclose(in);
	
}

void selecao_natural(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M, int n)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

