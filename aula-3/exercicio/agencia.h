#ifndef AGENCIA_H
#define AGENCIA_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Agencia {
    int cod;
    char nome[50];
    char gerente[50];
} Agencia;

// Imprime agencia
void ag_imprime(Agencia *ag);

// Cria agencia. Lembrar de usar free(agencia)
Agencia *agencia(int cod, char *nome, char *gerente);

// Salva agencia no arquivo out, na posicao atual do cursor
void ag_salva(Agencia *ag, FILE *out);

// Le uma agencia do arquivo in na posicao atual do cursor
// Retorna um ponteiro para agencia lido do arquivo
Agencia *ag_le(FILE *in);

// Retorna tamanho do agencia em bytes
int ag_tamanho();

#endif
