#include <stdio.h>
#include <stdlib.h>
#include "agencia.h"
#include "contacorrente.h"

// retorna -1 quando não é possível encontrar o código da agência referenciada
int cadastrar_conta(FILE *in, FILE *ag){
    int cod, codAg;
    double saldo;
    printf("Informe os seguintes dados da conta:\n");
    printf("Codigo: ");
    scanf("%d", &cod);
    printf("Codigo da Agencia: ");
    scanf("%d", &codAg);
    if(buscar_agencia(ag, codAg) != 0){
        return -1;
    }
    printf("Saldo em conta: ");
    scanf("%lf", &saldo);
    ContaCorrente *new_cc = contacorrente(cod, codAg, saldo);
    cc_salva(new_cc, in);
    free(new_cc);
    return 0;
}

int cadastrar_agencia(FILE *in){
    return 0;
}

int buscar_conta(FILE *in, int codConta){
    rewind(in);
    ContaCorrente *cc = NULL;
    cc = cc_le(in);
    if(cc == NULL) return 1;
    while(cc != NULL && cc->cod != codConta) cc = cc_le(in);
    if(cc == NULL) return 1;
    cc_imprime(cc);
    free(cc);
    return 0;
}

int buscar_agencia(FILE *in, int codAgencia){
    return 0;
}

/*void insere_5_funcionarios(FILE *out) {
    printf("Inserindo 5 funcionários no arquivo...");
    
    Funcionario *f1 = funcionario(1, "Ana", "000.000.000-00", "01/01/1980", 3000);
    salva(f1, out);
    free(f1);
    Funcionario *f2 = funcionario(2, "Carlos", "111.111.111-11", "01/01/1990", 500);
    salva(f2, out);
    free(f2);
    Funcionario *f3 = funcionario(3, "Fátima", "222.222.222-22", "02/02/1980", 1000);
    salva(f3, out);
    free(f3);
    Funcionario *f4 = funcionario(4, "Marcelo", "333.333.333-33", "03/03/1990", 1500);
    salva(f4, out);
    free(f4);
    Funcionario *f5 = funcionario(5, "Silvia", "444.444.444-44", "04/04/1980", 900);
    salva(f5, out);
    free(f5);       
}

void le_funcionarios(FILE *in) {
    printf("\n\nLendo funcionários do arquivo...\n\n");
    rewind(in);
    Funcionario *f;
    while ((f = le(in)) != NULL) {
        imprime(f);
        free(f);
    }
}

void le_segundo_funcionario(FILE *in) {
    printf("\n\nLendo segundo funcionário do arquivo...\n\n");
    //tamanho() indica quantos bytes vamos pular, o que aqui é igual ao tamanho de um registro 
    //(vamos pular o primeiro e nos posicionar no início do segundo)
    //** ATENÇÃO: não usar sizeof(Funcionario), pois ele pode retornar valor maior que o tamanho ocupado em disco, 
    //            devido a alinhamento automático (ver https://en.wikipedia.org/wiki/Data_structure_alignment))
    //SEEK_SET indica o início do arquivo
    //ao final, o cursor estará posicionado em 0 + tamanho() +1
    fseek(in, tamanho(), SEEK_SET);
    Funcionario *f = le(in);
    if (f != NULL) {
        imprime(f);
        free(f);
    }
}

void adiciona_funcionario(FILE *in) {
    printf("\n\nAdicionando funcionário no final do arquivo...\n\n");
    //pula 5 registros para posicionar no início do final do arquivo
    fseek(in, tamanho() * 5, SEEK_SET);
    Funcionario *f = funcionario(6, "Bruna", "666.666.666-66", "06/06/1980", 2500);
    salva(f, in);
    free(f);
    
    //lê funcionário que acabou de ser gravado
    //posiciona novamente o cursor no início desse registro
    fseek(in, tamanho() * 5, SEEK_SET);
    Funcionario *f6 = le(in);
    if (f6 != NULL) {
        imprime(f6);
        free(f6);
    }    
}

void sobrescreve_quarto_funcionario(FILE *in) {
    printf("\n\nSobrescrevendo quarto funcionário do arquivo...\n\n");
    //pula primeiros 3 registros para posicionar no início do quarto registro
    fseek(in, tamanho() * 3, SEEK_SET);
    Funcionario *f4 = funcionario(7, "Catarina", "777.777.777-77", "07/07/1990", 5000);
    salva(f4, in);
    free(f4);
    
    //lê funcionário que acabou de ser gravado
    //posiciona novamente o cursor no início desse registro
    fseek(in, tamanho() * 3, SEEK_SET);
    Funcionario *f = le(in);
    if (f != NULL) {
        imprime(f);
        free(f);
    }
}*/

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
                        if(resposta == 1) cadastrar_conta(outConta, outAgencia);
                        if(resposta == 2) cadastrar_agencia(outAgencia);
                        break;

                    case 2:
                        resposta = -1; rc=0;
                        while(teste_resposta(resposta)){
                            printf("Escolha uma operacao:\n\t1. Ler uma Conta Corrente\n\
\t2. Ler uma Agencia\n\t0. Sair\n");
                            rc = scanf("%d", &resposta);           
                        }
                        if(resposta == 1){
                            int cod;
                            printf("Digite o codigo da Conta Corrente requerida: ");
                            scanf("%d", &cod);
                            int r = buscar_conta(outConta, cod);
                            if(r == 1) printf("Nao foi possivel encontrar uma conta com o codigo fornecido\n");
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
