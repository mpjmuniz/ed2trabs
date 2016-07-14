#ifndef CHAVE_SECUNDARIA_H
#define CHAVE_SECUNDARIA_H


#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "lista_clientes.h"
#include "lista_registros_indice_idade.h"

#define NOME_ARQUIVO_INDICE "indice_idade.dat"

// Executa o algoritmo de Lefkowitz para indexar o arquivo por idade
// nome_arquivo: nome do arquivo que contem os registros a serem indexados
// tam: número de registros do arquivo
void indexa(char *nome_arquivo, int tam);

#endif
