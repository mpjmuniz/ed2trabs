#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "geracao_particoes.h"
#include "nomes.h"
#include "lista_clientes.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <io.h>

void classificacao_interna(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	//TODO: Inserir aqui o codigo do algoritmo de geracao de particoes
}

void mostrar_congelados(int congs[]){
	int i = 0;
	for(i = 0; i < sizeof(congs)/sizeof(int); i++) printf("%d ", congs[i]);
	printf("\n");
}

int existe_nao_congelado(int congelados[], int M){
	int i = 0;
	for(i = 0; i < M; i++){
		if(!congelados[i]) return 1;
	}
	return 0;
}

/*int substituirCliente(ListaClientes *cls, int pos, FILE *fnt){
	Cliente *cl = le_cliente(fnt);

}*/

void substituirRegistro(Cliente *dest, Cliente *fnt){
	strcpy(dest->nome, fnt->nome);
	dest->cod_cliente = fnt->cod_cliente;
}

void selecao_com_substituicao(char *nome_arquivo_entrada, Nomes *nome_arquivos_saida, int M)
{
	FILE *in = fopen(nome_arquivo_entrada, "rb"),
		 *out;
	int i = 0, m = 0, gravado = 0, j = 0;
	ListaClientes *list = NULL;
        int congelados[M];
	Cliente *menor = NULL, *lido;
        memset(congelados, 0, sizeof(congelados));
	
	//	1. Ler M registros do arquivo para a memória
	ler_clientes(in, &list, M, congelados);
        int tam = conta_nomes(nome_arquivos_saida);
	for(j = 0; j < tam; j++){
            out = fopen(nome_arquivos_saida->nome, "wb");
            while(existe_nao_congelado(congelados, M)){
                    //	2. Selecionar, no array em memória, o registro r com menor chave
                    int menor_val = INT_MAX;
                    for(i = 0; i < M; i++){
                        if(congelados[i]) continue;
                        if(list->lista[i]->cod_cliente <= menor_val){
                            menor_val = list->lista[i]->cod_cliente;
                            menor = list->lista[i];
                            m = i;
                        }
                    }
                    //	3. Gravar o registro r na partição de saída
                    salva_cliente(menor, out);
                    //	4. Substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada
                    //if(list->lista[m] != NULL){
                    gravado = menor->cod_cliente;
                    lido = le_cliente(in);
                    if(lido != NULL){
                        substituirRegistro(list->lista[m], lido);
                    }

                    if(lido == NULL || lido->cod_cliente < gravado){
                        //	5. Caso a chave deste último seja menor do que a chave recém gravada, 
                        //	considerá-lo congelado e ignorá-lo no restante do processamento
                        congelados[m] = 1;
                    }
                    if(lido != NULL)
                            free(lido);

                    //	6. Caso existam em memória registros não congelados, voltar ao passo 2
		}
		/*	7. Caso contrário:
			- fechar a partição de saída*/
                if(j+1<tam){
                    salva_cliente(cliente(INT_MAX, ""), out);
                }
		fclose(out);
		//	- descongelar os registros congelados
		for(i = 0; i < M; i++) 
                    if(list->lista[i] != NULL) congelados[i] = 0;
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

