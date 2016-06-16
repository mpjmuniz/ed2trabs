#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include <stdlib.h>
#include <assert.h>

#include "arvore_b.h"
#include "metadados.h"
#include "no.h"

int busca(int x, char *nome_arquivo_metadados, char *nome_arquivo_dados, int *pt, int *encontrou)
{
	int     p,
		rc = -1,
		i,
		pos,
		m;
	FILE *fnos = fopen(nome_arquivo_dados, "r+b");
	No *no;
	Metadados *meta;

	assert(fnos != NULL && "Falha na abertura do arquivo de dados.\n");
	
	//Abre arquivo de metadados e lê a raiz
	meta = le_arq_metadados(nome_arquivo_metadados);

	/*p:= ptraiz; pt:= λ; encontrou := 0;*/
	*pt = INT_MAX;
	*encontrou = 0;
	p = meta->pont_raiz;

	//enquanto p ≠ λ faça
	while(p != INT_MAX && p != -1){
		//i:= 1; pos:= 1; pt:= p
		i = 0;
		pos = 0;
		*pt = p;
                
		rc = fseek(fnos, p, SEEK_SET);
                assert(rc == 0 && "Falha no seek.\n");
		no = le_no(fnos);
		if(no == NULL) break;

		m = no->m;

		// enquanto i ≤ m faça % m é o número de chaves que a página p contém
		while(i < m){
			//se x > p↑.s[i] então i: = i+1; pos: = i + 1
			if(x > no->clientes[i]->cod_cliente){
                                pos = ++i;
			} else {
				//se x = p↑.s[i] então
				if(x == no->clientes[i]->cod_cliente){
					//p:= λ; encontrou := 1 % chave encontrada
					p = INT_MAX;
					*encontrou = 1;
				} else {
					//senão p := p↑.pont[i-1]
					p = no->p[i];
				}
				//i:= m + 2
				i = m + 1;
			}
		}

		//se i = m + 1 então p:= p↑.pont[m]
		if(i == m){
			p = no->p[m];
		}
                free(no);
	}

	if(*encontrou == 0){
		pos = 0;
	}

	//Libera variáveis
	fclose(fnos);
	if(meta != NULL) free(meta);
	if(no != NULL) libera_no(no);

    return pos;
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
