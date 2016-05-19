#include "fila.h"

Fila *Fila_criar(){
	return calloc(1, sizeof(Fila)); 
}

void Fila_destruir(Fila * fila){
	assert(fila != NULL && "Fila inválida.");
	//assert(fila->qtd > 0 && fila->prim != NULL && fila->ult != NULL && "Fila Vazia.");
	FILA_ITERAR(fila, ult, prox, atual){
		if(atual->ant){
			free(atual->ant);
		}
	}

	free(fila->prim);
	free(fila);
}

void Fila_limpar(Fila * fila){
	assert(fila != NULL && "Fila Inválida.");
	//assert(fila->qtd > 0 && fila->prim != NULL && fila->ult != NULL && "Fila vazia.");
	FILA_ITERAR(fila, ult, prox, atual){
		free(atual->arq);
	}
}

void Fila_limpar_destruir(Fila * fila){
	assert(fila != NULL && "Fila Inválida");
	Fila_limpar(fila);
	Fila_destruir(fila);
	
}

void Fila_enflr(Fila *fila, char *nome_arq){
	assert(fila != NULL && "Fila Inválida");
	NoArquivo *no = calloc(1, sizeof(NoArquivo));
	assert(no != NULL && "Memória insuficiente");
	
	no->arq = malloc(512*sizeof(char));
	no->arq = strcpy(no->arq, nome_arq);

	if(fila->ult == NULL){
		fila->prim = no;
		fila->ult = no;
	} else {
		no->prox = fila->ult;
		fila->ult->ant = no;
		fila->ult = no;
	}

	fila->qtd++;
}

FILE *Fila_desnflr(Fila * fila){
	assert(fila != NULL && "Fila Inválida");
	assert(fila->qtd > 0 && fila->prim != NULL && "Fila Vazia.");
	NoArquivo *no = fila->prim;
	return no != NULL ? Fila_remover(fila, no) : NULL;
}

FILE *Fila_remover(Fila * fila, NoArquivo * no){
	FILE *result = NULL;
	assert(fila != NULL && "Fila Inválida.");
	assert(fila->prim && fila->ult && "Fila vazia.");
	assert(no && "no não pode ser NULL.");

	if(no == fila->prim && no == fila->ult){
		fila->prim = NULL;
		fila->ult = NULL;
	} else if(no == fila->prim){
		fila->prim = no->ant;
		assert(fila->prim != NULL && "Fila Inválida, De algum modo apareceu um primeiro nulo.");
		fila->prim->prox = NULL;
	} else if(no == fila->ult){
		fila->ult = no->prox;
		assert(fila->ult != NULL && "Fila Inválida, De algum modo apareceu um prox nulo.");
		fila->ult->ant = NULL;
	} else {
		NoArquivo *depois = no->prox;
		NoArquivo *antes = no->ant;
		depois->ant = antes;
		antes->prox = depois;
	}

	fila->qtd--;
	result = fopen(no->arq, "rb");
	free(no->arq);
	free(no);
	return result;
}
