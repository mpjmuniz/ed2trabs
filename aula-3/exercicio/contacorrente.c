#include "contacorrente.h"
#include "agencia.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Imprime agencia
void cc_imprime(ContaCorrente *cc) {
    printf("**********************************************");
    printf("\nCodigo da Conta Corrente:");
    printf("%d", cc->cod);
    printf("\nCodigo da Agencia: ");
    printf("%d", cc->codAgencia);
    printf("\nSaldo: ");
    printf("%.2f", cc->saldo);
    printf("\n**********************************************\n");
}

// Cria agencia. Lembrar de usar free(func)
ContaCorrente *contacorrente(int cod, int codAg, double saldo){
    ContaCorrente *cc = (ContaCorrente *) malloc(sizeof(ContaCorrente));
    //inicializa espaço de memória com ZEROS
    if (cc) memset(cc, 0, sizeof(ContaCorrente));
    //copia valores para os campos de ag
    cc->cod = cod;
    cc->codAgencia = codAg;
    cc->saldo = saldo;
    return cc;
}

// Salva agencia no arquivo out, na posicao atual do cursor
void cc_salva(ContaCorrente *cc, FILE *out) {
    fwrite(&(cc->cod), sizeof(int), 1, out);
    fwrite(&(cc->codAgencia), sizeof(int), 1, out);
    fwrite(&(cc->saldo), sizeof(double), 1, out);
}

// Le um agencia do arquivo in na posicao atual do cursor
// Retorna um ponteiro para agencia lido do arquivo
ContaCorrente *cc_le(FILE *in) {
    ContaCorrente *cc = (ContaCorrente *) malloc(sizeof(ContaCorrente));
    if (0 >= fread(&cc->cod, sizeof(int), 1, in)) {
		free(cc);
		return NULL;
    }
    fread(&cc->codAgencia, sizeof(int), 1, in);
    fread(&cc->saldo, sizeof(double), 1, in);
    return cc;
}

// Retorna tamanho do agencia em bytes
int cc_tamanho() {
    return sizeof(int) * 2 //cods
            + sizeof(double); //saldo
}
