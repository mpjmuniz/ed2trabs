#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "chave_secundaria.h"

void indexa(char *nome_arquivo, int tam) {
	/*
	 * Entrada: arquivo binário com a seguinte estrutura (COD, NOME, IDADE)
	 *  onde COD é a chave primária, e IDADE é o atributo que será indexado
	 * */

	//Registros do Arquivo A1
	ListaRegistrosIndiceIdade *lista;
	ListaClientes *listaClientes = le_clientes(nome_arquivo);

	//Geração do Arquivo A2 (e A3)
	Cliente *cAtual = NULL;
	FILE *a2 = fopen("a2.dat", "r+b");
	int i = 0,
		rc = -1;

	for( i = 0; i < tam; i++){
		cAtual = listaClientes->lista[i]; 
		if(cAtual == NULL) break; 

		rc = fwrite(&i, sizeof(int), 1, a2);
		assert(rc == 1 && "Falha na escrita.\n");

		salva_cp(a2, cAtual);
		salva_cs(a2, cAtual);

		free(cAtual);
	}

	fclose(a2);
	libera_clientes(listaClientes);

	//Geração do Arquivo A4
	
	ListaRegistrosAuxiliaresA4 *listaA4 = le_registrosA4("a2.dat");
	ordenar_a4(&listaA4);
	salva_registros_a4("a4.dat", listaA4);

	//Geração do Arquivo A5
	//Geração do Arquivo A6
	//Geração do Arquivo A7
	
	libera_registros(lista);
}
