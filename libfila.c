#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

/* Cria fila retorna NULL se deu errado */
fila_t* fila_cria (){
	fila_t *f;
	
	f=malloc(sizeof(fila_t));
	if (f == NULL)
		return NULL;

	f->ini=NULL;
	f->fim=NULL;
	f->tamanho=0;
	return f;
}

/* se fila vazia retorna 1, se não retorna 0 */
int fila_vazia(fila_t* f){
    if (f->ini == NULL)
        return 1;
    return 0;
}

fila_t* fila_destroi(fila_t* f){
	/* se fila não vazia, dar free nos nodos */
	if (!fila_vazia(f)){
		nodo_f_t *aux;
		while (f->ini !=  NULL){
			aux=f->ini;
			f->ini=f->ini->prox;
			free(aux);
		}
	}
	free(f);
	return NULL;
}

/* Retorna o tamanho da fila */
int fila_tamanho(fila_t* f){
	return f->tamanho;
}

/* insere o elemento no final e retorna 1 se deu certo,
   se não retorna 0 */
int queue (fila_t* f, int elemento){
	nodo_f_t *novo;

	novo=malloc(sizeof(nodo_f_t));
    if (novo == NULL){
        return 0;
	}
    novo->chave=elemento;
    novo->prox=NULL;
    /* Se fila vazia, f->ini e f->fim recebe o novo nodo */
	if (fila_vazia(f)){
        f->ini=novo;
	}
	else{
		f->fim->prox=novo;
	}
    f->fim=novo;
    f->tamanho++;
	return 1;
}

/* Remove o elemento do inicio da fila e retorna,se deu certo
   retorna 1, se não retorna 0 */
int dequeue(fila_t* f, int* elemento){
	if (fila_vazia(f))
		return 0;
	*elemento=f->ini->chave;
	nodo_f_t *aux;
	/* aux recebe o primeiro nó e f->ini
	   aponta para o segundo */
	aux=f->ini;
	f->ini=aux->prox;
	/* se só tiver um nó, faz o fim apontar para NULL */
	if (f->ini == NULL)
        f->fim = NULL;
	aux->prox=NULL;
	/* Desaloca o primeiro da fila */
	free (aux);
	f->tamanho--;
	return 1;
}

/* imrpime a fila sem espaço no final, se vazia
   nao retorna nada */
void fila_imprime(fila_t* f){
	if (fila_vazia(f))
		return;
	/* Usa um nodo aux para percorrer a fila */
	nodo_f_t *aux;
	aux=f->ini;
	while (aux->prox != NULL){
		printf("%d ",aux->chave);
		aux=aux->prox;
	}
	printf("%d",aux->chave);
	printf("\n");
}

