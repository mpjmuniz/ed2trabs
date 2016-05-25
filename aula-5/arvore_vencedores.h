/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef ARVORE_VENCEDORES_H
#define ARVORE_VENCEDORES_H

#include "cliente.h"

typedef struct arvore_vencedores{
    Cliente *cliente;
    int num_arq;
    struct arvore_vencedores *right;
    struct arvore_vencedores *left;
} ArvoreVencedores;

ArvoreVencedores *cria_no(Cliente *c, int num_arq);

ArvoreVencedores *cria_arvore(ArvoreVencedores *a, ArvoreVencedores *b);

void substituir(Cliente *c, Cliente *c_subst, ArvoreVencedores *av);

#endif /* ARVORE_VENCEDORES_H */

