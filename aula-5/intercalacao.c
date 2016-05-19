#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "intercalacao.h"

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
	 *  freq: Número de partições abertas por vez
	 */

	int restantes = qtd,
		i = 0,
		freq = 3;

	for(i = restantes; i > 0; i -= freq){
		/*	abro freq partições, da fila de partições
		 *	intercalo estas partições, usando a árvore de vencedores
		 *	coloco denovo esta partição gerada na fila	
		 */
	}

}
