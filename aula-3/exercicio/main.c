#include <stdio.h>
#include <stdlib.h>
#include "agencia.h"
#include "contacorrente.h"

ContaCorrente *buscar_conta(FILE *in,int codConta,int codAg){
    FILE *aux = in;
    rewind(aux);
    ContaCorrente *cc = NULL;
    while((cc = cc_le(aux)) != NULL && cc->cod != codConta && cc->codAgencia != codAg){
        free(cc);
    }
    free(aux);
    return cc;
}

// retorna -1 quando não é possível encontrar o código da agência referenciada
// retorna -2 quando já existe uma conta corrente com a mesma chave
int cadastrar_conta(FILE *in, FILE *agIn,int cod,int codAg,double saldo){
    ContaCorrente *cc;
    Agencia ag;
    if((cc = buscar_conta(in, cod, codAg)) != NULL){
        free(cc);
        return -2;
    }
    if(buscar_agencia(agIn, codAg) != 0){
        return -1;
    }
    ContaCorrente *new_cc = contacorrente(cod, codAg, saldo);
    cc_salva(new_cc, in);
    free(new_cc);
    return 0;
}

int cadastrar_agencia(FILE *in){
    return 0;
}

int buscar_agencia(FILE *in, int codAgencia){
    return 0;
}

int teste_resposta(int resposta){
    return resposta < 0 || resposta > 2;
}

void main(int argc, char** argv) {
    //declara ponteiro para arquivo
    FILE *outAgencia, *outConta;
    //abre arquivos
    
    if ((outAgencia = fopen("agencia.dat", "a+b")) == NULL || 
    		(outConta = fopen("contacorrente.dat", "a+b")) == NULL){
		if(!outAgencia){
			printf("Erro ao abrir arquivo das Agencias\n");
		} 
		
		if(!outConta){
			printf("Erro ao abrir arquivo das Contas\n");
		}
        exit(1);
    } else {
        int resposta = -1, rc = 0;

        while(resposta != 0){
        	printf("Escolha uma operacao:\n\t1. Cadastrar uma Conta Corrente, ou uma Agencia\n\
\t2. Ler uma Conta Corrente, ou uma Agencia\n\t0. Sair\n");
        	rc = scanf("%d", &resposta);
                
                switch(resposta){
                    case 1:
                        resposta = -1; rc=0;
                        while(teste_resposta(resposta)){
                            printf("Escolha uma operacao:\n\t1. Cadastrar uma Conta Corrente\n\
\t2. Cadastrar uma Agencia\n\t0. Sair\n");
                            rc = scanf("%d", &resposta);           
                        }
                        if(resposta == 1) {
                            int cod, codAg;
                            double saldo;
                            printf("Informe os seguintes dados da conta:\n");
                            printf("Codigo: ");
                            scanf("%d", &cod);
                            printf("Codigo da Agencia: ");
                            scanf("%d", &codAg);
                            printf("Saldo em conta: ");
                            scanf("%lf", &saldo);
                            int r = cadastrar_conta(outConta, outAgencia, cod, codAg, saldo);
                            if(r == -1) printf("Nao existe uma agencia com esse codigo, operacao abortada\n");
                            else if(r == -2) printf("Ja existe uma conta com esse codigo nessa agencia\n");
                            else printf("Conta cadastrada com sucesso!\n");
                        }
                        if(resposta == 2){ 
                            cadastrar_agencia(outAgencia);
                        }
                        break;

                    case 2:
                        resposta = -1; rc=0;
                        while(teste_resposta(resposta)){
                            printf("Escolha uma operacao:\n\t1. Ler uma Conta Corrente\n\
\t2. Ler uma Agencia\n\t0. Sair\n");
                            rc = scanf("%d", &resposta);           
                        }
                        if(resposta == 1){
                            int codConta, codAg;
                            printf("Digite o codigo da Conta Corrente requerida: ");
                            scanf("%d", &codConta);
                            printf("Digite o codigo da Agencia referente a Conta Corrente requerida: ");
                            scanf("%d", &codAg);
                            ContaCorrente *cc = buscar_conta(outConta, codConta, codAg);
                            cc_imprime(cc);
                            free(cc);
                        } else if(resposta == 2){
                            int cod;
                            printf("Digite o codigo da Agencia requerida: ");
                            scanf("%d", &cod);
                            buscar_agencia(outAgencia, cod);
                        }
                        break;
                }
        }
        //fecha arquivo
        fclose(outAgencia);    
        fclose(outConta);
    }
}
