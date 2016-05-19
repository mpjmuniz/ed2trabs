#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define BEFORE_TEST before();
#define NOME_ARQUIVO_SAIDA "saida.dat"

#include <limits.h>
#include <stdlib.h>

#include "ufftest.h"
#include "lista_clientes.h"
#include "cliente.h"
#include "intercalacao.h"

//Implementação
#include "fila.h"

void before() {
	remove(NOME_ARQUIVO_SAIDA);
}

BEGIN_TESTS();
TEST("Teste de criação de filas");
if(!skip){
	Fila *fila1 = Fila_criar();
	ASSERT_TRUE(fila1 != NULL)
	Fila_limpar_destruir(fila1);
}

TEST("Teste de inserção nas filas");
if(!skip){
	Fila *fila1 = Fila_criar();
	Fila_enflr(fila1, "particao1.dat");
	Fila_enflr(fila1, "particao2.dat");
	Fila_enflr(fila1, "particao3.dat");
	Fila_enflr(fila1, "particao4.dat");
	Fila_enflr(fila1, "particao5.dat");
	Fila_enflr(fila1, "particao6.dat");
	ASSERT_TRUE(Fila_qtd(fila1) == 6)

	Fila_limpar_destruir(fila1);
}

TEST("Teste de remoção nas filas");
if(!skip){
	Fila *fila1 = Fila_criar();
	int i = 0, rc = 0;
	char nome[20];
	FILE *arq;
	Fila_enflr(fila1, "particao6.dat");
	Fila_enflr(fila1, "particao3.dat");
	Fila_enflr(fila1, "particao2.dat");
	Fila_enflr(fila1, "particao1.dat");
	
	arq = Fila_desnflr(fila1);
	ASSERT_TRUE(arq != NULL);
	fclose(arq);

	arq = Fila_desnflr(fila1);
	ASSERT_TRUE(arq != NULL);
	fclose(arq);

	arq = Fila_desnflr(fila1);
	ASSERT_TRUE(arq != NULL);
	fclose(arq);
	
	ASSERT_EQUAL(Fila_qtd(fila1), 1)
	Fila_limpar_destruir(fila1);
}

/*
TEST("Testa arquivo vazio");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 1;

	char * nome_particoes[] = {
	    "particao1.dat"
	};
	particao = cria_clientes(1, cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(1, cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa uma particao");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 1;
	char * nome_particoes[] = {
	    "particao1.dat"
	};
	particao = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa uma particao cheia e duas vazias");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 3;
	char * nome_particoes[] = {
	    "particao1.dat",
	    "particao2.dat",
	    "particao3.dat"
	};
	particao = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao2.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(1, cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	salva_clientes("particao3.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa duas particoes");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 2;
	char * nome_particoes[] = {
	    "particao1.dat",
	    "particao2.dat",
	};
	particao = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(2,
		cliente(3, "Ana", "12/12/1960"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao2.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(5,
		cliente(1, "Joao", "12/04/2000"),
		cliente(3, "Ana", "12/12/1960"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa tres particoes");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 3;
	char * nome_particoes[] = {
	    "particao1.dat",
	    "particao2.dat",
	    "particao3.dat",
	};
	particao = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(5,
		cliente(2, "Jose", "02/04/2001"),
		cliente(4, "Mariana", "15/09/1997"),
		cliente(10, "Cintia", "01/01/1991"),
		cliente(11, "Camila", "09/12/1987"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao2.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(4,
		cliente(3, "Marcos", "09/04/1986"),
		cliente(6, "Alice", "12/06/1999"),
		cliente(12, "Marcela", "19/04/1991"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao3.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(11,
		cliente(1, "Joao", "12/04/2000"),
		cliente(2, "Jose", "02/04/2001"),
		cliente(3, "Marcos", "09/04/1986"),
		cliente(4, "Mariana", "15/09/1997"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(6, "Alice", "12/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(10, "Cintia", "01/01/1991"),
		cliente(11, "Camila", "09/12/1987"),
		cliente(12, "Marcela", "19/04/1991"),
		cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa quatro particoes");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 4;
	char * nome_particoes[] = {
	    "particao1.dat",
	    "particao2.dat",
	    "particao3.dat",
	    "particao4.dat",
	};
	particao = cria_clientes(4,
		cliente(1, "Joao", "12/04/2000"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao1.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(4,
		cliente(2, "Jose", "02/04/2001"),
		cliente(10, "Cintia", "01/01/1991"),
		cliente(11, "Camila", "09/12/1987"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao2.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(3,
		cliente(3, "Marcos", "09/04/1986"),
		cliente(6, "Alice", "12/06/1999"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao3.dat", particao);
	libera_clientes(particao);

	particao = cria_clientes(3,
		cliente(4, "Mariana", "15/09/1997"),
		cliente(12, "Marcela", "19/04/1991"),
		cliente(INT_MAX, "", ""));
	salva_clientes("particao4.dat", particao);
	libera_clientes(particao);

	oraculo = cria_clientes(11,
		cliente(1, "Joao", "12/04/2000"),
		cliente(2, "Jose", "02/04/2001"),
		cliente(3, "Marcos", "09/04/1986"),
		cliente(4, "Mariana", "15/09/1997"),
		cliente(5, "Maria", "14/06/1999"),
		cliente(6, "Alice", "12/06/1999"),
		cliente(7, "Pedro", "23/07/1992"),
		cliente(10, "Cintia", "01/01/1991"),
		cliente(11, "Camila", "09/12/1987"),
		cliente(12, "Marcela", "19/04/1991"),
		cliente(INT_MAX, "", ""));

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
}

TEST("Testa dez particoes");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 10, i;
	char *nome_particoes[10];
	int qtd_particao[10];
	int qtd_oraculo = 0;

	for (i = 1; i <= qtd; ++i) {
		nome_particoes[i - 1] = (char *) malloc(sizeof(char) * 15);
		qtd_particao[i - 1] = 3;
		qtd_oraculo += 2;
		if (i % 2 == 0) {
			qtd_particao[i - 1] += 2;
			qtd_oraculo += 2;
		}
		if (i % 7 == 0) {
			qtd_particao[i - 1] += 1;
			qtd_oraculo += 1;
		}
	}

	qtd_oraculo += 1;
	oraculo = (ListaClientes *) malloc(sizeof(ListaClientes));
	oraculo->qtd = qtd_oraculo;
	oraculo->lista = (Cliente **) malloc(sizeof(Cliente *) * qtd_oraculo);
	qtd_oraculo = 0;

	for (i = 1; i <= qtd; ++i) {
		int n = 0;
		particao = (ListaClientes *) malloc(sizeof(ListaClientes));
		particao->qtd = qtd_particao[i - 1];
		particao->lista = (Cliente **) malloc(sizeof(Cliente *) * qtd_particao[i - 1]);
		particao->lista[n++] = cliente(i * 10 + 1, "Ana", "01/01/2000");
		particao->lista[n++] = cliente(i * 10 + 3, "Ana", "01/01/2000");
		if (i % 2 == 0) {
			particao->lista[n++] = cliente(i * 10 + 5, "Ana", "01/01/2000");
			particao->lista[n++] = cliente(i * 10 + 7, "Ana", "01/01/2000");
		}
		if (i % 7 == 0) {
			particao->lista[n++] = cliente(i * 10 + 9, "Ana", "01/01/2000");
		}
		particao->lista[n++] = cliente(INT_MAX, "", "");
		sprintf(nome_particoes[i - 1], "particao%d.dat", i);
		salva_clientes(nome_particoes[i - 1], particao);
		libera_clientes(particao);

		oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 1, "Ana", "01/01/2000");
		oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 3, "Ana", "01/01/2000");
		if (i % 2 == 0) {
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 5, "Ana", "01/01/2000");
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 7, "Ana", "01/01/2000");
		}
		if (i % 7 == 0) {
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 9, "Ana", "01/01/2000");
		}
	}
	oraculo->lista[qtd_oraculo++] = cliente(INT_MAX, "", "");

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
	for (i = 1; i <= qtd; ++i) {
		free(nome_particoes[i - 1]);
	}
}

TEST("Testa cem particoes");
if (!skip) {
	ListaClientes *particao, *oraculo, *saida;
	int qtd = 100, i;
	char *nome_particoes[100];
	int qtd_particao[100];
	int qtd_oraculo = 0;

	for (i = 1; i <= qtd; ++i) {
		nome_particoes[i - 1] = (char *) malloc(sizeof(char) * 16);
		qtd_particao[i - 1] = 3;
		qtd_oraculo += 2;
		if (i % 2 == 0) {
			qtd_particao[i - 1] += 2;
			qtd_oraculo += 2;
		}
		if (i % 10 == 0) {
			qtd_particao[i - 1] += 1;
			qtd_oraculo += 1;
		}
	}

	qtd_oraculo += 1;
	oraculo = (ListaClientes *) malloc(sizeof(ListaClientes));
	oraculo->qtd = qtd_oraculo;
	oraculo->lista = (Cliente **) malloc(sizeof(Cliente *) * qtd_oraculo);
	qtd_oraculo = 0;

	for (i = 1; i <= qtd; ++i) {
		int n = 0;
		particao = (ListaClientes *) malloc(sizeof(ListaClientes));
		particao->qtd = qtd_particao[i - 1];
		particao->lista = (Cliente **) malloc(sizeof(Cliente *) * qtd_particao[i - 1]);
		particao->lista[n++] = cliente(i * 10 + 1, "Ana", "01/01/2000");
		particao->lista[n++] = cliente(i * 10 + 3, "Ana", "01/01/2000");
		if (i % 2 == 0) {
			particao->lista[n++] = cliente(i * 10 + 5, "Ana", "01/01/2000");
			particao->lista[n++] = cliente(i * 10 + 7, "Ana", "01/01/2000");
		}
		if (i % 10 == 0) {
			particao->lista[n++] = cliente(i * 10 + 9, "Ana", "01/01/2000");
		}
		particao->lista[n++] = cliente(INT_MAX, "", "");
		sprintf(nome_particoes[i - 1], "particao%d.dat", i);
		salva_clientes(nome_particoes[i - 1], particao);
		libera_clientes(particao);

		oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 1, "Ana", "01/01/2000");
		oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 3, "Ana", "01/01/2000");
		if (i % 2 == 0) {
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 5, "Ana", "01/01/2000");
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 7, "Ana", "01/01/2000");
		}
		if (i % 10 == 0) {
			oraculo->lista[qtd_oraculo++] = cliente(i * 10 + 9, "Ana", "01/01/2000");
		}
	}
	oraculo->lista[qtd_oraculo++] = cliente(INT_MAX, "", "");

	intercalacao(nome_particoes, qtd, NOME_ARQUIVO_SAIDA);

	saida = le_clientes(NOME_ARQUIVO_SAIDA);

	ASSERT_EQUAL_CMP(oraculo, saida, cmp_clientes)
	libera_clientes(oraculo);
	libera_clientes(saida);
	for (i = 1; i <= qtd; ++i) {
		free(nome_particoes[i - 1]);
	}
}
*/
END_TESTS();
