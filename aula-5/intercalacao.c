#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "intercalacao.h"
#include "fila.h"
#include "cliente.h"
#include "arvore_vencedores.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void intercalacao(char **nome_particoes, int qtd, char *nome_arquivo_saida)
{
	/*
	 *	Implementação do método de intercalação ótima de arquivos
	 *	por uso da árvore de vencedores.
	 *	
	 *	TODO:
	 *		- Implementar fila de arquivos
	 *		- Implementar árvore de vencedores
	 */

	/*	Assumindo que não há limite para arquivos sendo abertos pela parte
	 *	do SO, não farei aqui verificações sobre a variavel @qtd.
	 */
	
	/* 	restantes: Número de partições restantes, que pode variar ao passo que as 
	 *  	partições geradas entram novamente no algoritmo 
	 *      freq: Número de partições abertas por vez
	 */

	/*	abro freq partições, da fila de partições
        *	intercalo estas partições, usando a árvore de vencedores
        *	coloco denovo esta partição gerada na fila	
        */
        // Abre os arquivos do ciclo
        int j, k, space;
        FILE *files[qtd];
        for(j = 0; j<qtd; j++){
            files[j] = fopen(nome_particoes[j], "rb");
        }
        // cria arvore de vencedores
        ArvoreVencedores *av[qtd];
        for(j=0; j<qtd; j++){
            Cliente *c = le_cliente(files[j]);
            av[j] = cria_no(c, j);
        }
        k=2;
        space=1;
        while(space<qtd){
            for(j=0; j+space<qtd; j+=k){
                av[j] = cria_arvore(av[j] , av[j+space]);
            }
            k*=2;
            space=k>>1;
        }
        ArvoreVencedores *venc = av[0];
        // intercala partições
        FILE *saida = fopen(nome_arquivo_saida, "wb");
        while(venc->cliente->cod_cliente != INT_MAX){                
            salva_cliente(venc->cliente, saida);
            Cliente *c = le_cliente(files[venc->num_arq]);
            substituir(venc->cliente, c, venc);
        }
        salva_cliente(venc->cliente, saida);
        fclose(saida);
}