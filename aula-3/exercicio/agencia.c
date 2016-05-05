#include "agencia.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Imprime agencia
void ag_imprime(Agencia *ag) {
    printf("**********************************************");
    printf("\nCódigo da Agencia:");
    printf("%d", ag->cod);
    printf("\nNome: ");
    printf("%s", ag->nome);
    printf("\nGerente: ");
    printf("%s", ag->gerente);
    printf("\n**********************************************");
}

// Cria agencia. Lembrar de usar free(func)
Agencia *agencia(int cod, char *nome, char *gerente){
    Agencia *ag = (Agencia *) malloc(sizeof(Agencia));
    //inicializa espaço de memória com ZEROS
    if (ag) memset(ag, 0, sizeof(Agencia));
    //copia valores para os campos de ag
    ag->cod = cod;
    strcpy(ag->nome, nome);
    strcpy(ag->gerente, gerente);
    return ag;
}

// Salva agencia no arquivo out, na posicao atual do cursor
void ag_salva(Agencia *ag, FILE *out) {
    fwrite(&ag->cod, sizeof(int), 1, out);
    //ag->nome ao invés de &ag->nome, pois string já é ponteiro
    fwrite(ag->nome, sizeof(char), sizeof(ag->nome), out);
    fwrite(ag->gerente, sizeof(char), sizeof(ag->gerente), out);
}

// Le um agencia do arquivo in na posicao atual do cursor
// Retorna um ponteiro para agencia lido do arquivo
Agencia *ag_le(FILE *in) {
    Agencia *ag = (Agencia *) malloc(sizeof(Agencia));
    if (0 >= fread(&ag->cod, sizeof(int), 1, in)) {
		free(ag);
		return NULL;
    }
    fread(ag->nome, sizeof(char), sizeof(ag->nome), in);
    fread(ag->gerente, sizeof(char), sizeof(ag->gerente), in);
    return ag;
}

// Retorna tamanho do agencia em bytes
int ag_tamanho() {
    return sizeof(int)  //cod
            + sizeof(char) * 50 //nome
            + sizeof(char) * 50; //gerente
}

