/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef ARVORE_VENCEDORES_H
#define ARVORE_VENCEDORES_H

typedef struct arvore_vencedores{
    Cliente *cliente;
    int num_arq;
    ArvoreVencedores *right;
    ArvoreVencedores *left;
} ArvoreVencedores;

ArvoreVencedores *cria_no(Cliente *c, int num_arq);

ArvoreVencedores *cria_arvore(ArvoreVencedores *a, ArvoreVencedores *b);

void substituir(Cliente *c, Cliente *c_subst, ArvoreVencedores *av);

#endif /* ARVORE_VENCEDORES_H */

