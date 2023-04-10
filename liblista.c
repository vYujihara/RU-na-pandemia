#include "liblista.h"
#include <stdio.h>
#include <stdlib.h>

/* Função para criar um nodo, se não deu certo retorna NULL */
nodo_l_t *cria_nodo(int elemento){
	nodo_l_t *nodo;

	nodo=malloc(sizeof(nodo_l_t));
	if (nodo==NULL)
		return NULL;
	nodo->elemento=elemento;
	nodo->prox=NULL;
	return nodo;
}

/* Percorre a lista verificando ordenação em ordem crescente.
   Se verdadeiro retorna 1, se não retorna 0 */
int verifica_ordenado(lista_t* l){
	int tamanho;
	nodo_l_t *aux;

	tamanho=lista_tamanho(l);
	/* se um elemento, está ordenado */
	if (tamanho == 1)
		return 1;
	aux=l->ini;
	
	/* testa se ordenado até o final do vetor */
	while (aux->prox != NULL){
		if (aux->elemento > aux->prox->elemento)
			return 0;
		aux=aux->prox;
	}
	return 1;
}

/* cria lista vazia e retorna, se falhar retorna NULL */
lista_t* lista_cria(){
	lista_t *l;

	l=malloc(sizeof(lista_t));
	if (l == NULL)
		return NULL;
	l->ini=NULL;
	l->tamanho=0;
	return l;
}

/* Destroi lista e retorna NULL */
lista_t* lista_destroi(lista_t* l){
	nodo_l_t *antes, *atual;
	
	antes=l->ini;
	/* se lista vazia, não entra no laço */
	while (antes != NULL){
		atual=antes->prox;
		free(antes);
		antes=atual;
	}
	free(l);
	return NULL;
}

/* se vazia retorna 1, se não 0. */
int lista_vazia(lista_t* l){
	if (l->ini==NULL)
		return 1;
	return 0;
}

/* Retorna o tamanho da lista.Esta função está na 
   função verifica_ordenado então ja está sendo testado */
int lista_tamanho(lista_t* l){
	return l->tamanho;

}

/* insere um elemento no inicio, se deu certo retorna 1 e
   0 caso contrario */
int lista_insere_inicio(lista_t* l, int elemento){
	nodo_l_t *novo;
	novo=cria_nodo(elemento);
	/* Se novo NULL, alocação deu errado */
	if (novo == NULL)
		return 0;
	novo->prox=l->ini;
	l->ini=novo;
	l->tamanho++;
	return 1;

}

/* Insere no final da lista e retorna 1 se deu certo,
   se não retorna 0 */
int lista_insere_fim(lista_t* l, int elemento){
	nodo_l_t *novo;
	nodo_l_t *aux;
	
	novo=cria_nodo(elemento);
	/* Se novo NULL, alocação deu errado */
	if (novo == NULL)
		return 0;

	/* se lista vazia, insere normal */
	if (lista_vazia(l))
		l->ini=novo;
	else{
		aux=l->ini;		
		/* Percorre até o fim da lista */
		while ((aux->prox) != NULL)
			aux=aux->prox;
		/* quando aux->prox=NULL, ele aponta para o novo */
		aux->prox=novo;
		}
	l->tamanho++;
	return 1;
}

/* Insere o elemento se estiver em ordem crescente. Se deu
   certo retorna 1, se não retorna 0 */
int lista_insere_ordenado(lista_t* l, int elemento){
	nodo_l_t *novo=cria_nodo(elemento);
	nodo_l_t *aux;

	if (novo == NULL)
		return 0;

	if (lista_vazia(l)){
		l->ini=novo;
		l->tamanho++;
		return 1;
	}

	/* Aqui compara o primeiro elemento com o novo
	    e se está ordenado */
	if (novo->elemento <= l->ini->elemento && verifica_ordenado(l)){
		/* Se satisfeito, insere no início */
		novo->prox=l->ini;
		l->ini=novo;
		return 1;			
	}

	aux=l->ini;
	/* laço para procurar e inserir de forma ordenada,
	   percorre até o fim */
	while (aux->prox != NULL){
		/* se novo for menor ou igual a aux->prox, insere
		entre o aux e o aux->prox */	   			   
		if (novo->elemento <= aux->prox->elemento){
			novo->prox=aux->prox;
			aux->prox=novo;
			l->tamanho++;
			return 1;
		}
		aux=aux->prox;	
	}
	/* Se saiu do laço, deve inserir no final */
	aux->prox=novo;
	return 1;		
}

/* Remove do inicio da lista e retorna em 'elemento'.
   Retorna 1 se deu certo, caso contrário retorna 0 */
int lista_retira_inicio(lista_t* l, int* elemento){
	nodo_l_t *aux;

	if (lista_vazia(l))
		return 0;

	/* *elemento recebe o elemento a ser retirado */
	*elemento=l->ini->elemento;
	aux=l->ini;
	l->ini=l->ini->prox;
	free(aux);
	l->tamanho--;
	return 1;	
}

/* retira no fim da lista e retorna o elemento.
   se deu certo retorna 1, se não retorna 0 */
int lista_retira_fim(lista_t* l, int* elemento){
	nodo_l_t *atual, *antes;
	
	if (lista_vazia(l))
		return 0;

	antes=l->ini;
	/* se só tem um nodo entra aqui */
	if (antes->prox == NULL){
		*elemento=antes->elemento;
		l->ini=NULL;
		free(antes);
		l->tamanho--;
		return 1;
	}

	atual=l->ini->prox;	
	/* Percorre a lista até o final */
	while (atual->prox != NULL){
		antes=antes->prox;
		atual=atual->prox;
	}

	/* *elemento recebe o elemento a ser retirado */
	*elemento=atual->elemento;
	antes->prox=atual->prox;
	free(atual);
	l->tamanho--;
	return 1;
}

/* retorna  se o elemento existe na lista. Retorna 1 se 
   operação sucedida, caso contrario retorna 0 */
int lista_retira_elemento(lista_t* l, int *elemento){
	nodo_l_t *atual;
	
	if (lista_vazia(l))
		return 0;

	atual=l->ini;
	/* Se o elemento a ser retirado estiver no inicio */
	if (l->ini->elemento == *elemento){
		l->ini=atual->prox;
		free(atual);
		return 1;
	}
	else{
		atual=l->ini;
		/* Percorre até o final */
		while (atual->prox != NULL){
			/* Se achar o elemento, retira ele */
			if (atual->prox->elemento == *elemento){
				nodo_l_t *aux;
				/* aux aponta para o nodo a ser tirado */
				aux=atual->prox;
				atual->prox=aux->prox;
				free(aux);
				return 1;
			}
			atual=atual->prox;
		}
		return 0;
	}
}

/* retorna 1 se elemeneto pertence, 0 caso contrario */
int lista_pertence(lista_t *l, int elemento){
	if (lista_vazia(l))
		return 0;
	nodo_l_t *aux;
	aux=l->ini;
	/* Percorre a lista verificando se tem o elemento */
	while (aux != NULL){
		if (aux->elemento == elemento)
			return 1;
		aux=aux->prox;
	}
	return 0;
}

/* imprime do inicio até o fim, e o ultimo sem espaço no final,
   Se estiver vazia não imrpimi nada */
void lista_imprime(lista_t* l){
	/* Se lista vazia, não retorna nada */
	if (lista_vazia(l))
		return;
	nodo_l_t *aux=l->ini;
	while(aux->prox != NULL){
		printf("%d ",aux->elemento);
		aux=aux->prox;	
	}
	printf("%d",aux->elemento);
	printf("\n");
}
