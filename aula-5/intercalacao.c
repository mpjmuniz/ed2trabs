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

	int restantes = qtd,
		i = 0,
		freq = 3;
        Fila *arquivos = Fila_criar();
        for(i=0; i<qtd; i++){
            Fila_enflr(arquivos, nome_particoes[i]);
        }

	for(i = restantes; i > 0; i -= freq){
		/*	abro freq partições, da fila de partições
		 *	intercalo estas partições, usando a árvore de vencedores
		 *	coloco denovo esta partição gerada na fila	
		 */
            // Abre os arquivos do ciclo
            int j, k, space, real_freq;
            real_freq = (Fila_qtd(arquivos)<freq)? Fila_qtd(arquivos):freq;
            FILE *files[real_freq];
            for(j = 0; j<real_freq; j++){
                files[j] = Fila_desnflr(arquivos);                
            }
            // cria arvore de vencedores
            ArvoreVencedores *av[real_freq];
            for(j=0; j<real_freq; j++){
                Cliente *c = le_cliente(files[j]);
                av[j] = cria_no(c, j);
            }
            k=2;
            space=1;
            while(space<real_freq){
                for(j=0; j+space<real_freq; j+=k){
                    av[j] = cria_arvore(av[j] , av[j+space]);
                }
                k*=2;
                space=k>>1;
            }
            ArvoreVencedores *venc = av[0];
            // intercala partições
            while(venc->cliente->cod_cliente != INT_MAX){
                FILE *saida = fopen(nome_arquivo_saida, "wb");
                salva_cliente(venc->cliente, saida);
                Cliente *c = le_cliente(files[venc->num_arq]);
                substituir(venc->cliente, c, venc);
            }
            restantes = Fila_qtd(arquivos);
	}

}