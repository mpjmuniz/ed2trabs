#ifndef LISTA_REGISTROS_INDICE_IDADE_H
#define LISTA_REGISTROS_INDICE_IDADE_H

#include "registro_indice_idade.h"

typedef struct ListaRegistrosIndiceIdade {
	RegistroIndiceIdade **lista;
	int qtd;
} ListaRegistrosIndiceIdade;

// Imprime registros
void imprime_registros(ListaRegistrosIndiceIdade *lr);

// Cria lista de registros. Lembrar de usar libera_registros(lista_registros)
ListaRegistrosIndiceIdade *cria_registros(int qtd, ...);

// Salva lista de registros no arquivo nome_arquivo
void salva_registros(char *nome_arquivo, ListaRegistrosIndiceIdade *l);

// Le lista de registros do arquivo nome_arquivo
ListaRegistrosIndiceIdade *le_registros(char *nome_arquivo);

// Compara duas listas de registros
// Retorna 1 se os clientes das duas listas forem iguais
// e 0 caso contrario
int cmp_registros(ListaRegistrosIndiceIdade *l1, ListaRegistrosIndiceIdade *l2);

// Desaloca lista de registros
void libera_registros(ListaRegistrosIndiceIdade *l);

//Adiciona registro @r à lista @l
void adiciona_registro(ListaRegistrosIndiceIdade *l, RegistroIndiceIdade *r);

//Registros auxiliares
typedef struct listaregistrosauxiliaresa4 {
	RegistroAuxiliarA4 **lista;
	int qtd;
} ListaRegistrosAuxiliaresA4;

// Le lista de registros do arquivo nome_arquivo
ListaRegistrosAuxiliaresA4 *le_registrosA4(char *nome_arquivo);

//Ordena lista de registros A4
inline void ordenar_a4(ListaRegistrosAuxiliaresA4 **lista);

// Salva lista de registros no arquivo nome_arquivo
void salva_registros_a4(char *nome_arquivo, ListaRegistrosAuxiliaresA4 *l);

// Desaloca lista de registros
void libera_registros_a4(ListaRegistrosAuxiliaresA4 *l);
#endif
