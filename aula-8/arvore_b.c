#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include <stdlib.h>
#include <assert.h>

#include "arvore_b.h"
#include "lista_clientes.h"
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

	/*if(*encontrou == 0){
		pos = 0;
	}*/

	//Libera variáveis
	fclose(fnos);
	if(meta != NULL) free(meta);
	if(no != NULL) libera_no(no);

    return pos;
}
void particionar(FILE *arq, Cliente *insert_cli,int pt, FILE *meta);
int _insert(FILE *arq, Cliente *insert_cli,int pt, int cli_pointer, FILE *meta);

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados)
{
    int pt, encontrou;
    busca(cod_cli, nome_arquivo_metadados, nome_arquivo_dados, &pt, &encontrou);
    
    if(encontrou == 1) return -1;
    
    FILE *fnos = fopen(nome_arquivo_dados, "r+b");
    FILE *meta = fopen(nome_arquivo_metadados, "r+b");
    Cliente *c = cliente(cod_cli, nome_cli);
    _insert(fnos, c, pt, -1, meta);
    
    free(c);
    fclose(fnos);
    return pt;
}

int _insert(FILE *arq, Cliente *insert_cli,int pt, int cli_pointer, FILE *meta){
    if(pt == -1){
        Metadados *m = le_metadados(meta);
        No *new_no = no(1, -1);
        new_no->clientes[0] = insert_cli;
        new_no->p[0] = m->pont_raiz;
        new_no->p[1] = pt;
        fseek(arq, 0, SEEK_END);
        m->pont_raiz = ftell(arq);
        salva_no(new_no, arq);
        rewind(meta);
        salva_metadados(m, meta);
        free(m);
        free(new_no);
        return m->pont_raiz;
    }
    fseek(arq, pt, SEEK_SET);
    No *no = le_no(arq);
    if(no->m < 2*D - 1){ // Há posições vazias
        // Encontra a posição dentro do nó
        int i;
        for(i=no->m; i>0 && insert_cli->cod_cliente < no->clientes[i-1]->cod_cliente; i--){
            no->clientes[i] = no->clientes[i-1];
            no->p[i+1] = no->p[i];
        }
        no->clientes[i] = insert_cli;
        no->p[i+1] = cli_pointer;
        no->m++;
        fseek(arq, pt, SEEK_SET); // Retorna a posição onde o nó foi lido
        salva_no(no, arq);        // Sobrescreve o nó
    } else {
        particionar(arq, insert_cli, pt, meta);
    }
    return pt;
}

void particionar(FILE *arq, Cliente *insert_cli,int pt, FILE *meta){
    fseek(arq, pt, SEEK_SET);
    No *n = le_no(arq);
    Cliente *node;
    if(n->clientes[D+1]->cod_cliente > insert_cli->cod_cliente){
        node = insert_cli;
    } else {
        node = n->clientes[D+1];
        n->clientes[D+1] = insert_cli;
    }
    No *new_no = no(D, n->pont_pai);
    int i;
    for(i =0; i<D; i++){
        new_no->clientes[i] = n->clientes[D+i];
        new_no->p[i] = n->p[D+i];
        n->clientes[D+i] = NULL;
    }
    n->m = D;
    new_no->p[i] = n->p[D+i];
    
    fseek(arq, 0, SEEK_END); // Coloca o cursor no final do arquivo
    
    n->pont_pai = _insert(arq, node, n->pont_pai, ftell(arq), meta);
    new_no->pont_pai = n->pont_pai;
    
    salva_no(new_no, arq);    // Salva o novo nó
    fseek(arq, pt, SEEK_SET); // Retorna a posição onde o nó foi lido
    salva_no(n, arq);         // Sobrescreve o nó
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_dados)
{
    int pont,
    	encontrou,    	
    	posicao = busca(cod_cli, nome_arquivo_metadados, nome_arquivo_dados, &pont, &encontrou),
    	rc = -1,
    	i = 0, 
    	j = 0,
    	d = 2;
	FILE *dados;
	No *no,
	   *filho;
	Cliente *menor;

    if(!encontrou) return -1;

	dados = fopen(nome_arquivo_dados, "r+b");
	assert(dados != NULL && "Arquivo inexistente.\n");

	rc = fseek(dados, pont, SEEK_SET);
	assert(rc == 0 && "Falha no seek.\n");
	
	no = le_no(dados);
	assert(no != NULL && "Inconsistência na estrutura.\n");

	for(i = 0; no->clientes[i]->cod_cliente != cod_cli; i++);
	remover_cliente(no->clientes, i);
	no->m--;
		
	if(no->p[i + 1] != -1){
		rc = fseek(dados, no->p[i + 1], SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		//Buscar menor registro no nó à direita
		filho = le_no(dados);
		menor = filho->clientes[0];

		no->clientes[i] = menor;
		ordenar_clientes(no->clientes);

		remover_cliente(filho->clientes, 0);
		filho->m--;
		transladar_clientes(filho->clientes, filho->m, 0);
	} else {
		transladar_clientes(no->clientes, no->m, i);
	}

	if(no->m < d){
	
	} else {
		
	}

	libera_no(no);
	fclose(dados);
    return pont;
}
