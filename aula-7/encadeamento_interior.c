#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

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
	//TODO: Inserir aqui o codigo do algoritmo
    return INT_MAX;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, int tam)
{
	//TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod_cli, char *nome_arquivo_hash, int tam)
{
	//TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}
