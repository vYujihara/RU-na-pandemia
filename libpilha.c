#include "libpilha.h"
#include <stdio.h>
#include <stdlib.h>

pilha_t* pilha_cria (int nelem){
	pilha_t *pilha;

    pilha=malloc(sizeof(pilha_t));
	if ( pilha == NULL)
			return NULL;

	/* alocacao do vetor */
	pilha->elems=malloc(nelem*sizeof(int));
	if (pilha->elems == NULL)
		return NULL;

	pilha->nelem=nelem;
	/* topo começa no -1 */
	pilha->topo=-1;
	return pilha;

}

int push(pilha_t* pilha, int elem){
    /* se topo cheio, retorna -1 */
    if (pilha->topo+1 == pilha->nelem)
        return -1;

    pilha->topo++;
    pilha->elems[pilha->topo]=elem;
    /* retorna o número de elementos na pilha */
    return pilha->topo+1;
}

/* retira da pilha e retorna o elemento retirado */
int pop(pilha_t* pilha){
	if (pilha_vazia(pilha))
		return 0;
    pilha->topo--;
    return pilha->elems[pilha->topo+1];
}

/* retorna o elemento do topo */
int pilha_topo ( pilha_t* pilha){
	if (pilha_vazia(pilha))
		return 0;
    return pilha->elems[pilha->topo];
}

/* Retorna o numero de eleentos na pilha */
int pilha_tamanho(pilha_t* pilha){
	return pilha->topo+1;
}

/* verifica se pilha vazia */
int pilha_vazia(pilha_t* pilha){
    if (pilha->topo==-1)
        return 1;
    else
        return 0;
}

/* free no vetor, depois free na pilha */
pilha_t* pilha_destroi(pilha_t* pilha){
    free(pilha->elems);
    free(pilha);
	return NULL;
}

/* imprime da base até o topo, sem espaço.
   Se vazio, nao imprime nada */
void pilha_imprime(pilha_t* pilha){
	int i;

	if (pilha_vazia(pilha))
		return;
	else{
		for(i=0;i<=pilha->topo;i++)
        	if (!i)
            	printf("%d", pilha->elems[i]);
        	else
            	printf(" %d", pilha->elems[i]);
	}
	printf("\n");
} 
