#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdarg.h>

#include "lista_registros_indice_idade.h"


void imprime_registros(ListaRegistrosIndiceIdade *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		imprime_registro_indice_idade(l->lista[i]);
	}
}

ListaRegistrosIndiceIdade *cria_registros(int qtd, ...)
{
	va_list ap;
	ListaRegistrosIndiceIdade *lc = (ListaRegistrosIndiceIdade *)  malloc(sizeof(ListaRegistrosIndiceIdade));
	lc->qtd = qtd;
	lc->lista = (RegistroIndiceIdade **) malloc(sizeof(RegistroIndiceIdade *) * (qtd));
	int i;
	va_start(ap, qtd);
	for (i = 0; i < qtd; i++) {
        lc->lista[i] = va_arg(ap, RegistroIndiceIdade *);
    }
    va_end(ap);
    return lc;
}

void salva_registros(char *nome_arquivo, ListaRegistrosIndiceIdade *l)
{
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < l->qtd; i++) {
		salva_registro_indice_idade(l->lista[i], out);
	}
	fclose(out);
}

ListaRegistrosIndiceIdade * le_registros(char *nome_arquivo)
{
	int qtd = 0;
	ListaRegistrosIndiceIdade *l = (ListaRegistrosIndiceIdade *)  malloc(sizeof(ListaRegistrosIndiceIdade));
	FILE *in = fopen(nome_arquivo, "rb");
	if (in != NULL) {
		RegistroIndiceIdade *registro = NULL;
		while((registro = le_registro_indice_idade(in)) != NULL) {
			qtd += 1;
			free(registro);
		}
		fseek(in, 0, SEEK_SET);
		l->qtd = qtd;
		l->lista = (RegistroIndiceIdade **) malloc(sizeof(RegistroIndiceIdade *) * (qtd));
		qtd = 0;
		while((registro = le_registro_indice_idade(in)) != NULL) {
			l->lista[qtd++] = registro;
		}
		fclose(in);
	} else {
		l->qtd = 0;
		l->lista = NULL;
	}
	return l;
}

int cmp_registros(ListaRegistrosIndiceIdade *c1, ListaRegistrosIndiceIdade *c2)
{
	if (c1->qtd != c2->qtd) {
		return 0;
	}
	int i;
	for (i = 0; i < c1->qtd; i++) {
		if (!cmp_registro_indice_idade(c1->lista[i], c2->lista[i])) {
			return 0;
		}
	}
	return 1;
}

void libera_registros(ListaRegistrosIndiceIdade *l)
{
	int i;
	for (i = 0; i < l->qtd; i++) {
		free(l->lista[i]);
	}
	free(l->lista);
	free(l);
}

void adiciona_registro(ListaRegistrosIndiceIdade *l, RegistroIndiceIdade *r){
	RegistroIndiceIdade **novo_r = (RegistroIndiceIdade **) calloc((l->qtd + 1), sizeof(ListaRegistrosIndiceIdade *));
	int i = 0;
	
	for(i = 0; i < l->qtd; i++){
		novo_r[i] = l->lista[i];
		l->lista[i] = NULL;
	}
	
	novo_r[l->qtd] = r;

	free(l->lista);
	l->lista = novo_r;
	l->qtd++;
}

ListaRegistrosAuxiliaresA4 *le_registrosA4(char *nome_arquivo){

	int qtd = 0;
	ListaRegistrosAuxiliaresA4 *l = (ListaRegistrosAuxiliaresA4 *)  malloc(sizeof(ListaRegistrosAuxiliaresA4));
	FILE *in = fopen(nome_arquivo, "rb");

	if (in != NULL) {
		RegistroAuxiliarA4 *registro = NULL;
		while((registro = le_registro_a4(in)) != NULL) {
			qtd += 1;
			free(registro);
		}
		fseek(in, 0, SEEK_SET);
		l->qtd = qtd;
		l->lista = (RegistroAuxiliarA4 **) malloc(sizeof(RegistroAuxiliarA4 *) * (qtd));
		qtd = 0;
		while((registro = le_registro_a4(in)) != NULL) {
			l->lista[qtd++] = registro;
		}
		fclose(in);
	} else {
		l->qtd = 0;
		l->lista = NULL;
	}
	return l;
}

inline void ordenar_a4(ListaRegistrosAuxiliaresA4 **lista) {

	RegistroAuxiliarA4 *temp = (RegistroAuxiliarA4 *) malloc(sizeof(RegistroAuxiliarA4));
    int i = 0,
     	j = 0;

    for(i = 0; i < (*lista)->qtd; i++) {
        for(j = 0; j < (*lista)->qtd - 1; j++) {
            if((*lista)->lista[j]->cs > (*lista)->lista[j + 1]->cs) {
                temp = (*lista)->lista[j + 1];
                (*lista)->lista[j + 1] = (*lista)->lista[j];
                (*lista)->lista[j] = temp;
            }
        }
    }
}

void salva_registros_a4(char *nome_arquivo, ListaRegistrosAuxiliaresA4 *l){
	FILE *out = fopen(nome_arquivo, "wb");
	int i;
	for (i = 0; i < l->qtd; i++) {
		salva_registro_a4(l->lista[i], out);
	}
	fclose(out);
}

void libera_registros_a4(ListaRegistrosAuxiliaresA4 *l){
	int i;
	for (i = 0; i < l->qtd; i++) {
		free(l->lista[i]);
	}
	free(l->lista);
	free(l);
}
