#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdlib.h>

#include "encadeamento_interior.h"
#include "lista_clientes.h"


void cria_hash(char *nome_arquivo_hash, int tam)
{
	int i = 0;
	ListaClientes *lc = cria_clientes(0);

	for(i = 0; i < tam; i++){
		adcn_cliente(lc, -1, "", i, LIBERADO); 
	}

	salva_clientes(nome_arquivo_hash, lc);
	libera_clientes(lc);
}

int busca(int cod_cli, char *nome_arquivo_hash, int tam, int *encontrou)
{
    // Inicializa variáveis
    *encontrou = 0;
    int indice = cod_cli % tam, j, indice_vazio = -1;
    Cliente *c;
    FILE *arq = fopen(nome_arquivo_hash, "r+b");
    
    // Posiciona o cursor no ponto de leitura e lê o cliente
    fseek(arq, tamanho_cliente()*indice, SEEK_SET);
    c = le_cliente(arq);
    // Verifica se o espaço está vazio e guarda a posição
    if(indice_vazio == -1 && c->flag == LIBERADO) indice_vazio = indice;
    if(c->flag == OCUPADO && cod_cli == c->cod_cliente ) {
        fclose(arq);
        *encontrou = 1;
        return indice;
    }
    free(c);
    
    j = c->prox;
    while(j != indice){
        indice = j;
        //similar ao de cima
        fseek(arq, tamanho_cliente()*indice, SEEK_SET);
        c = le_cliente(arq);
        if(indice_vazio == -1 && c->flag == LIBERADO) indice_vazio = indice;
        if(c->flag == OCUPADO && cod_cli == c->cod_cliente ) {
            fclose(arq);
            free(c);
            *encontrou = 1;
            return indice;
        }
        j = c->prox;
        free(c);
    }
    fclose(arq);
    // Caso atinja este ponto o cliente não foi encontrados
    // retorna o indice vazio se encontrado, e o final caso contrário
    if(indice_vazio != -1) return indice_vazio;
    else return indice;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, int tam)
{
    // Verifica se o registro existe
    int encontrou;
    int indice = busca(cod_cli, nome_arquivo_hash, tam, &encontrou);
    if(encontrou != 0) return -1;
    // Inicializa variáveis extras
    FILE *arq = fopen(nome_arquivo_hash, "a+b");
    Cliente *c = cliente(cod_cli, nome_cli, indice, OCUPADO);
    // Salva cliente
    fseek(arq, tamanho_cliente()*indice, SEEK_SET);
    salva_cliente(c, arq);
    free(c);
    fclose(arq);    
    return indice;
}

int exclui(int cod_cli, char *nome_arquivo_hash, int tam)
{
	int encontrou, a, saida = 0;
	a = busca(cod_cli, nome_arquivo_hash, tam, &encontrou);

	if(encontrou == 1){
		//T[end].estado = liberado
	} else {
		return -1;
	}

    return saida;
}
