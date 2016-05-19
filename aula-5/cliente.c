#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>

#include "cliente.h"


void imprime_cliente(Cliente *cliente)
{
	printf("%d, %s, %s\n", cliente->cod_cliente, cliente->nome, cliente->data_nascimento);
}

Cliente *cliente(int cod, char *nome, char *nascimento)
{
	Cliente *cliente = (Cliente *) malloc(sizeof(Cliente));
	if (cliente) memset(cliente, 0, sizeof(Cliente));
	cliente->cod_cliente = cod;
	strcpy(cliente->nome, nome);
	strcpy(cliente->data_nascimento, nascimento);
	return cliente;
}

void salva_cliente(Cliente *cliente, FILE *out)
{
	fwrite(&cliente->cod_cliente, sizeof(int), 1, out);
	fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
	fwrite(cliente->data_nascimento, sizeof(char), sizeof(cliente->data_nascimento), out);
}

Cliente *le_cliente(FILE *in)
{
	Cliente *cliente = (Cliente *) malloc(sizeof(Cliente));
	if (0 >= fread(&cliente->cod_cliente, sizeof(int), 1, in)) {
		free(cliente);
		return NULL;
	}
	fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
	fread(cliente->data_nascimento, sizeof(char), sizeof(cliente->data_nascimento), in);
	return cliente;
}

int cmp_cliente(Cliente *c1, Cliente *c2)
{
	if (c1 == NULL) {
		return (c2 == NULL);
	}
	if (c1->cod_cliente != c2->cod_cliente) {
		return 0;
	}
	if (strcmp(c1->nome, c2->nome) != 0) {
		return 0;
	}
	if (strcmp(c1->data_nascimento, c2->data_nascimento) != 0) {
		return 0;
	}
	return 1;
}

int tamanho_cliente() {
	return sizeof(int) + // cod_cliente
		sizeof(char) * TAM_NOME + // nome
		sizeof(char) * TAM_DATA_NASCIMENTO; // data_nascimento
}