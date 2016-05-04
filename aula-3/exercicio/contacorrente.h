#ifndef _CONTACORRENTE_H_
#define _CONTACORRENTE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct ContaCorrente {
    int cod;
    int codAgencia;
    double saldo;
} ContaCorrente;

// Imprime contacorrente
void imprime(ContaCorrente *cc);

// Cria contacorrente. Lembrar de usar free(contacorrente)
ContaCorrente *contacorrente(int cod, int codAgencia, double saldo);

// Salva contacorrente no arquivo out, na posicao atual do cursor
void salva(ContaCorrente *cc, FILE *out);

// Le uma contacorrente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para contacorrente lido do arquivo
ContaCorrente *le(FILE *in);

// Retorna tamanho do contacorrente em bytes
int tamanho();

#endif
