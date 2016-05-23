/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arvore_vencedores.h"
#include "cliente.h";
#include <stdlib.h>;

ArvoreVencedores *cria_no(Cliente *c, int num_arq){
    ArvoreVencedores *a = malloc(sizeof(arvore_vencedores));
    a->cliente = c;
    a->num_arq = num_arq;
    a->left = NULL;
    a->right = NULL;
    return a;
}

ArvoreVencedores *cria_arvore(ArvoreVencedores *r, ArvoreVencedores *l){
    if(r == NULL || l == NULL) return NULL;
    ArvoreVencedores *res = malloc(sizeof(arvore_vencedores));
    ArvoreVencedores *aux = (r->cliente->cod_cliente < l->cliente->cod_cliente)? r:l;
    res->cliente = aux->cliente;
    res->num_arq = aux->num_arq;
    res->left = r;
    res->right = l;
    return res;    
}

void substituir(Cliente *c, Cliente *c_subst, ArvoreVencedores *av){
    if(!av->right && !av->left){
        av->cliente = c_subst;
        return;
    }        
    if(av->right->cliente == c) substituir(c, c_subst, av->right);
    if(av->left->cliente == c) substituir(c, c_subst, av->left);
    ArvoreVencedores *aux = (av->right->cliente->cod_cliente < av->left->cliente->cod_cliente)? av->right:av->left;
    av->cliente = aux->cliente;
    av->num_arq = aux->num_arq;
}