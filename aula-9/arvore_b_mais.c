#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <assert.h>
#include <stdlib.h>

#include "arvore_b_mais.h"
#include "lista_clientes.h"
#include "metadados.h"
#include "no_folha.h"
#include "no_interno.h"

int insere_indice(int chave, int p, int pAnt, Metadados *metadado, FILE *indices ,int endNo, int apontaFolha);
int particiona_interno(int chave, int p, Metadados *metadado, FILE *indices ,int endNo);

int busca(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
		char *nome_arquivo_dados, int *pont_folha, int *encontrou) {

	*encontrou = 0;
	*pont_folha = 0;
	
	int rc,
		posicao,
		i,
		m,
		p_interno,
		//proximo é nó folha
		prox_folha = 0;
	FILE *indice,
		 *dados;
	Metadados *meta;
	NoFolha *folha;
	NoInterno *no_Atual;

	meta = le_arq_metadados(nome_arquivo_metadados);
	assert(meta != NULL && "Arquivo de metadados inválido, ou função errada.\n");

	*pont_folha = meta->pont_raiz;
	prox_folha = meta->raiz_folha;

	indice = fopen(nome_arquivo_indice, "rb");
	
	while(!prox_folha){
		i = 0;

		rc = fseek(indice, *pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		no_Atual = le_no_interno(indice);
		if(no_Atual == NULL) break;

		m = no_Atual->m;

		while(i < m){
			if(cod_cli >= no_Atual->chaves[i]){
				i++;
			}else{
				*pont_folha = no_Atual->p[i];
				i = m + 1;
			}
		}

		if(i == m) {
			*pont_folha = no_Atual->p[m];
		}

		prox_folha = no_Atual->aponta_folha;
		libera_no_interno(no_Atual);
	}

	//Busca na folha
	dados = fopen(nome_arquivo_dados, "rb");

	rc = fseek(dados, *pont_folha, SEEK_SET);
	assert(rc == 0 && "Falha no seek.\n");

	folha = le_no_folha(dados);
	assert(folha != NULL && "Folha é nula.\n");
	
	*encontrou = buscar_cliente(folha->clientes, folha->m, cod_cli, &posicao);

	libera_no_folha(folha);
	
	fclose(dados);
	fclose(indice);
	if(meta != NULL) free(meta);
	return posicao;
}
int particiona_interno(int chave, int p, Metadados *metadado, FILE *indices ,int endNo){
    int nChave, nP;
    // Abriu nó
    fseek(indices, endNo, SEEK_SET);
    NoInterno *indice = le_no_interno(indices);
    // Cria novo nó interno
    NoInterno *novo = no_interno(D+1, indice->pont_pai, indice->aponta_folha);
    // Separa chaves
    // Insere referência a nova partição no pai
    //insere_indice(nChave, nP, endNo, metadado, indices, endNo, 0);
    free(novo);
    return 0;
}

int insere_indice(int chave, int p, int pAnt, Metadados *metadado, FILE *indices ,int endNo, int apontaFolha){
    int i;
    // Se não existe nó interno, cria-se um
    if(endNo == -1) {
        NoInterno *novo = no_interno(1, -1, apontaFolha);
        novo->chaves[0] = chave;
        novo->p[0] = pAnt;
        novo->p[1] = p;
        fseek(indices, metadado->pont_prox_no_interno_livre, SEEK_SET);
        salva_no_interno(novo, indices);
        
        metadado->raiz_folha = 0;
        metadado->pont_raiz = metadado->pont_prox_no_interno_livre;
        metadado->pont_prox_no_interno_livre += tamanho_no_interno();
        libera_no_interno(novo);
        return metadado->pont_prox_no_interno_livre - tamanho_no_interno();
    }
    // Carrega os índices    
    fseek(indices, endNo, SEEK_SET);
    NoInterno *indice = le_no_interno(indices);


    if(indice->m != 2*D) {
        // Adiciona novo indice
        for(i=indice->m; i>0 && indice->chaves[i-1] > chave; i--) {
            indice->chaves[i] = indice->chaves[i-1];
            indice->p[i+1]    = indice->p[i];
        }
        indice->chaves[i] = chave;
        indice->p[i+1] = p;
        indice->m++;
        // Salva os índices
        fseek(indices, endNo, SEEK_SET);
        salva_no_interno(indice, indices);
        libera_no_interno(indice);
    } else {
        //particiona_interno(chave, p, metadado, indices, endNo, apontaFolha);
    }
    return endNo;
}

int particiona(Cliente *novo_c, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados ,int endFolha){
    FILE *dados = fopen(nome_arquivo_dados, "r+b"), *indices = fopen(nome_arquivo_indice, "r+b"), *meta = fopen(nome_arquivo_metadados, "r+b");
    fseek(dados, endFolha, SEEK_SET);
    Metadados *metadado = le_metadados(meta);
    NoFolha *folha = le_no_folha(dados);
    
    NoFolha *nova_folha = no_folha(D+1, -1, folha->pont_prox);
    folha->pont_prox = metadado->pont_prox_no_folha_livre;
    folha->m = D;

    int i, j=2*D-1, pos;
    for(i=D; i>=0; i--){
        if(novo_c->cod_cliente > folha->clientes[j]->cod_cliente){
            nova_folha->clientes[i] = novo_c;
        } else {
            nova_folha->clientes[i] = folha->clientes[j];
            folha->clientes[j] = NULL;
            j--;
        }
    }
    if(j == D-2) {
        folha->clientes[D-1] = novo_c;
        pos = endFolha;
    } else {
        pos = metadado->pont_prox_no_folha_livre;
    }
    
    // Resolve os índices
    // Verifica se existe indices
    int end_no_pai = insere_indice(nova_folha->clientes[0]->cod_cliente, metadado->pont_prox_no_folha_livre, endFolha, metadado, indices, folha->pont_pai, 1);
    
    //Salva a folha nova
    nova_folha->pont_pai = end_no_pai;
    fseek(dados, metadado->pont_prox_no_folha_livre, SEEK_SET);
    salva_no_folha(nova_folha, dados);
    
    folha->pont_pai = end_no_pai;
    fseek(dados, endFolha, SEEK_SET);
    salva_no_folha(folha, dados);
    
    // Resolve Metadados
    metadado->pont_prox_no_folha_livre = metadado->pont_prox_no_folha_livre + tamanho_no_folha();
    fseek(meta, 0, SEEK_SET);
    salva_metadados(metadado, meta);
    
    fclose(dados);
    fclose(meta);
    fclose(indices);
    libera_no_folha(folha);
    
    return pos;
}


int insere(int cod_cli, char *nome_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
    
    int endFolha, encontrou, i, pos;
    busca(cod_cli, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, &endFolha, &encontrou);
    if(encontrou) return -1;
    FILE *dados = fopen(nome_arquivo_dados, "r+b");
    fseek(dados, endFolha, SEEK_SET);
    NoFolha *folha = le_no_folha(dados);
    fclose(dados);
    Cliente *novo_c = cliente(cod_cli, nome_cli);
    
    if(folha->m == 2*D) pos = particiona(novo_c, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados,endFolha);
    else {
        dados = fopen(nome_arquivo_dados, "r+b");
        
        for(i=folha->m-1; i>=0 && folha->clientes[i]->cod_cliente > novo_c->cod_cliente; i--){
            folha->clientes[i+1] = folha->clientes[i];
        }
        folha->clientes[i+1] = novo_c;

        folha->m = folha->m+1;
        pos = i + 1;
        
        fseek(dados, endFolha, SEEK_SET);
        salva_no_folha(folha, dados);
        fclose(dados);
    }
    
    
    free(novo_c);
    libera_no_folha(folha);
    return pos;
}

inline Cliente *_remover_cliente(NoFolha* folha, int posicao_inicial){
	int i = posicao_inicial;
	Cliente *removido = folha->clientes[posicao_inicial];
	do{
		folha->clientes[i] = folha->clientes[i + 1];
	} while(++i < folha->m);
	folha->m--;
	folha->clientes[folha->m] = NULL;
	return removido;
}

inline void _adicionar_cliente(NoInterno *pai, NoFolha *folha, NoFolha *irmao, Cliente *novo){
	int i = folha->m - 1,
		j = 0,
		chave = novo->cod_cliente;

	while(i >= 0 && folha->clientes[i]->cod_cliente > novo->cod_cliente){
		folha->clientes[i + 1] = folha->clientes[i];
		i--;
	}

	folha->clientes[i + 1] = novo;
	folha->m++;

	if(i == 0){
		for(j = 0; j < pai->m; j++){
			if(pai->chaves[j] == chave){
				pai->chaves[j] = irmao->clientes[0]->cod_cliente;
			}
		}
	}
}

inline void _redistribuir(NoInterno *pai, NoFolha *folha, NoFolha *irmao){
	/*
	 *	Estado: irmao está à direita da folha, pai é pai de ambos
	 * */
	int i = 0;

	for(i = 0; i < irmao->m - D; i++){
		_adicionar_cliente(pai, folha, irmao, _remover_cliente(irmao, i));
	}
}

int exclui(int cod_cli, char *nome_arquivo_metadados, char *nome_arquivo_indice,
		char *nome_arquivo_dados) {
	int pont_folha = 0,
		encontrou = 0,
		rc = -1,
		posicao = 0,
		i = 0;
	FILE *dados,
		 *indices;
	NoFolha *folha,
			*irmao;
	NoInterno *pai;
	
	/*
	 *	Estado: posicao guarda a posicao onde o registro está na página
	 *			pont_folha guarda o ponteiro para a folha em que o registro está
	 *			encontrou diz se o registro está na árvore ou não
	 *
	 * */
	posicao = busca(cod_cli, nome_arquivo_metadados, nome_arquivo_indice,
			nome_arquivo_dados, &pont_folha, &encontrou);

	if(encontrou){
		/*
		 *	Se o registro está na árvore, só é necessário removẽ-lo da folha
		 * */
		dados = fopen(nome_arquivo_dados, "r+b");

		//Seek para leitura
		rc = fseek(dados, pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");

		folha = le_no_folha(dados);
		
		free(_remover_cliente(folha, posicao));

		if(folha->m < D){

			rc = fseek(dados, folha->pont_prox, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			irmao = le_no_folha(dados);

			indices = fopen(nome_arquivo_indice, "r+b");

			rc = fseek(indices, folha->pont_pai, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			pai = le_no_interno(indices);

			if(folha->m + irmao->m < 2 * D){
				//Concatenação
			} else {
				//Usar redistribuição
				_redistribuir(pai, folha, irmao);
			}

			rc = fseek(indices, folha->pont_pai, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			salva_no_interno(pai, indices);

			rc = fseek(dados, folha->pont_prox, SEEK_SET);
			assert(rc == 0 && "Falha no seek.\n");
			salva_no_folha(irmao, dados);

			if(pai != NULL) libera_no_interno(pai);
			if(irmao != NULL) libera_no_folha(irmao);
			fclose(indices);
		}

		rc = fseek(dados, pont_folha, SEEK_SET);
		assert(rc == 0 && "Falha no seek.\n");
		salva_no_folha(folha, dados);

		if(folha != NULL) libera_no_folha(folha);
		fclose(dados);
	} 

    return pont_folha;
}
