#include <stdio.h>
#include <stdlib.h>
#include "agencia.h"
#include "contacorrente.h"

ContaCorrente *buscar_conta(FILE *in,int codConta,int codAg){
    rewind(in);
    ContaCorrente *cc = NULL;
    while((cc = cc_le(in)) != NULL && (cc->cod != codConta || cc->codAgencia != codAg)){
        free(cc);
    }

    rewind(in);
    return cc;
}

Agencia *buscar_agencia(FILE *in, int codAgencia){
    rewind(in);
    Agencia *ag = NULL;
    while((ag = ag_le(in)) != NULL && ag->cod != codAgencia){
        free(ag);
    }
    rewind(in);
    return ag;
}

// retorna -1 quando não é possível encontrar o código da agência referenciada
// retorna -2 quando já existe uma conta corrente com a mesma chave
int cadastrar_conta(FILE *in, FILE *agIn,int cod,int codAg,double saldo){
    ContaCorrente *cc;
    Agencia *ag;

    if((ag = buscar_agencia(agIn, codAg)) == NULL){
    	//free(ag);
        return -1;
    }

    if((cc = buscar_conta(in, cod, codAg)) != NULL){
        free(cc);
        return -2;
    } else {
		ContaCorrente *new_cc = contacorrente(cod, codAg, saldo);
		cc_salva(new_cc, in);
		free(cc);
		free(new_cc);
		free(ag);
		return 0;
    }
}

int cadastrar_agencia(FILE *in, int cod, char *nome, char *gerente){
    Agencia *ag;
    if((ag = buscar_agencia(in, cod)) != NULL){
        free(ag);
        return -1;
    }
    Agencia *new_ag = agencia(cod, nome, gerente);
    ag_salva(new_ag, in);
    free(new_ag);
    return 0;
}

int teste_resposta(int resposta){
    return (resposta < 0 || resposta > 2);
}

int main(int argc, char** argv) {
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
        	
        	if(rc != 1){
        		printf("Digite o índice na opção desejada.\n");
        		resposta = -1;
        		continue;
        	}

            rc = 0; 

			switch(resposta){
				case 1:
					resposta = -1;
					while(teste_resposta(resposta)){
						printf("Escolha uma operacao:\n\t1. Cadastrar uma Conta Corrente\n\
\t2. Cadastrar uma Agencia\n\t0. Sair\n");
						rc = scanf("%d", &resposta); 

						
						if(rc != 1){
							printf("Digite o índice na opção desejada.\n");
							resposta = -1;
							continue;
						}
						
						int cod, codAg, r;
						double saldo;
						char gerente[50], nome[50];

						switch(resposta){
							case 1:
								//Seria o correto verificar cada scanf, mas em noma da praticidade...
								printf("Informe os seguintes dados da conta:\n");
								printf("Codigo: ");
								rc = scanf("%d", &cod);
								printf("Codigo da Agencia: ");
								rc = scanf("%d", &codAg);
								printf("Saldo em conta: ");
								rc = scanf("%lf", &saldo);

								r = cadastrar_conta(outConta, outAgencia, cod, codAg, saldo);
								
								if(r == -1) printf("Nao existe uma agencia com esse codigo, operacao abortada\n");
								else if(r == -2) printf("Ja existe uma conta com esse codigo nessa agencia\n");
								else printf("Conta cadastrada com sucesso!\n");
								break;
							case 2:
								printf("Codigo: ");
								rc = scanf("%d", &cod);
								printf("Nome da Agencia: ");
								rc = scanf("%s", nome);
								printf("Nome do gerente da Agencia: ");
								rc = scanf("%s", gerente);
								
								r = cadastrar_agencia(outAgencia, cod, gerente, nome);
								
								if(r == -1) printf("Ja existe uma agencia com esse codigo, operacao abortada\n");
								else printf("Agencia cadastrada com sucesso!\n");
								break;
							default:
								break;
						}
					}
					break;

				case 2:
					resposta = -1;
					while(teste_resposta(resposta)){
						printf("Escolha uma operacao:\n\t1. Ler uma Conta Corrente\n\
\t2. Ler uma Agencia\n\t0. Sair\n");
						rc = scanf("%d", &resposta);           
					}
					
					if(rc != 1){
						printf("Digite o índice na opção desejada.\n");
						resposta = -1;
						continue;
					}

					int codConta, codAg;
						
					switch(resposta){
						case 1:
							printf("Digite o codigo da Conta Corrente requerida: ");
							rc = scanf("%d", &codConta);
							printf("Digite o codigo da Agencia referente a Conta Corrente requerida: ");
							rc = scanf("%d", &codAg);

							ContaCorrente *cc = buscar_conta(outConta, codConta, codAg);
							
							if(cc == NULL)
								printf("Nao foi possivel encontrar uma conta corrente.\n");
							else{
								cc_imprime(cc);
								free(cc);
							}
							break;
						case 2:
							printf("Digite o codigo da Agencia requerida: ");
							rc = scanf("%d", &codAg);
							
							Agencia *ag = buscar_agencia(outAgencia, codAg);
							
							if(ag == NULL)
								printf("Não foi possivel encontrar uma agencia.\n");
							else {
								ag_imprime(ag);
								free(ag);
							}
							break;
						default:
							break;
					}
					break;
				case 0:
					//para evitar o default;
					break;
				default:
					printf("Digite o índice da opção desejada.\n");
					continue;
			}
        }
        //fecha arquivo
        fclose(outAgencia);    
        fclose(outConta);
    }
    return 0;
}
