#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_clientes.h"


void imprime_clientes(ListaClientes *lc)
{
	int i;
	for (i = 0; i < lc->qtd; i++) {
		imprime_cliente(lc->lista[i]);
	}
}

ListaClientes *cria_clientes(int qtd, ...)
{
	va_list ap;
	ListaClientes *lc = (ListaClientes *)  malloc(sizeof(ListaClientes));
	lc->qtd = qtd;
	lc->lista = (Cliente **) malloc(sizeof(Cliente *) * (qtd));
	int i;
	va_start(ap, qtd);
	for (i = 0; i < qtd; i++) {
        lc->lista[i] = va_arg(ap, Cliente *);
    }
    va_end(ap);
    return lc;
}

void salva_clientes(char *nome_arquivo, ListaClientes *lc)
{
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < lc->qtd; i++) {
		if(lc->lista[i] != NULL) salva_cliente(lc->lista[i], out);
	}
	fclose(out);
}

ListaClientes * le_clientes(char *nome_arquivo)
{
	int qtd = 0;
	ListaClientes *lc = (ListaClientes *)  malloc(sizeof(ListaClientes));
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		Cliente *cliente = NULL;
		while((cliente = le_cliente(in)) != NULL) {
			qtd += 1;
			free(cliente);
		}
		fseek(in, 0, SEEK_SET);
		lc->qtd = qtd;
		lc->lista = (Cliente **) malloc(sizeof(Cliente *) * (qtd));
		qtd = 0;
		while((cliente = le_cliente(in)) != NULL) {
			lc->lista[qtd++] = cliente;
		}
		fclose(in);
	} else {
		lc->qtd = 0;
		lc->lista = NULL;
	}
	return lc;
}


void ler_clientes(FILE *in, ListaClientes **list, int qtd, int *congelados){
	if(qtd < 0 || in == NULL){
		(*list)->qtd = 0;
		(*list)->lista = NULL;
		return;
	}
	
	int i = 0;

	if(*list == NULL){
		*list = (ListaClientes *) malloc(sizeof(ListaClientes));
		Cliente *cliente = NULL;
		(*list)->qtd = qtd;
		(*list)->lista = (Cliente **) calloc(qtd, sizeof(Cliente));

		for(i = 0; i < qtd && (cliente = le_cliente(in)) != NULL; i++){
			(*list)->lista[i] = cliente;
			congelados[i] = 0;
		}
	} else {
		Cliente *cliente = NULL;
		int qtdAntiga = (*list)->qtd;
		(*list)->qtd += qtd;
		(*list)->lista = (Cliente **) realloc((*list)->lista, (*list)->qtd * sizeof(Cliente));

		for(i = qtdAntiga; i < (*list)->qtd && (cliente = le_cliente(in)) != NULL; i++){
			(*list)->lista[i] = cliente;
			congelados[i] = 0;
		}
	}
	for(; i < qtd; i++) congelados[i] = 1;
}

int cmp_clientes(ListaClientes *c1, ListaClientes *c2)
{
	if (c1->qtd != c2->qtd) {
		return 0;
	}
	int i;
	for (i = 0; i < c1->qtd; i++) {
		if (!cmp_cliente(c1->lista[i], c2->lista[i])) {
			return 0;
		}
	}
	return 1;
}

void libera_clientes(ListaClientes *lc)
{
	int i;
	for (i = 0; i < lc->qtd; i++) {
		free(lc->lista[i]);
	}
	free(lc->lista);
	free(lc);
}