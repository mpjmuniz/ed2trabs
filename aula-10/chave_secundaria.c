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
	ListaClientes *listaClientes = le_clientes(nome_arquivo);
	
	//Geração do Arquivo A2 (e A3)
	Cliente *cAtual = NULL;

	//Geração do arquivo A4
	ListaRegistrosAuxiliaresA4 *listaA4;
	RegistroAuxiliarA4 *rAtual = NULL;
	
	//Geração do arquivo A5
	ListaRegistrosIndiceIdade *lista;
	RegistroIndiceIdade *riAtual = NULL,
						*riAnterior = NULL;

	FILE *a2 = fopen("a2.dat", "r+b");
	int i = 0,
		rc = -1,
		qtdAtual = 0,
		idadeAnterior = 0;

	for(i = 0; i < tam; i++){
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
	
	listaA4 = le_registrosA4("a2.dat");
	ordenar_a4(&listaA4);
	salva_registros_a4("a4.dat", listaA4);
	
	//Geração do Arquivo A5
	rAtual = NULL;
	lista = cria_registros(0);

	for(i = 0; i < tam; i++){
		rAtual = listaA4->lista[i];
		
		if(rAtual->cs == riAnterior->idade){
			riAnterior->q++;
			riAnterior->pt = rAtual->ed;
		} else {
			//Idade correta, ponteiro ainda não se sabe e a quantidade final também não
			riAtual = registro_indice_idade(rAtual->cs, -1,	1);
			//adicionar um novo registro com os dados de rAtual
			//TODO: ajustar controle da lista
		}
		
		riAnterior = riAtual;
		idadeAnterior = rAtual->cs;
		if(rAtual != NULL) free(rAtual);
	}

	//Geração do Arquivo A6
	//Geração do Arquivo A7
	
	libera_registros_a4(listaA4);
	libera_registros(lista);
}
