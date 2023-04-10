#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "libpilha.h"
#include "libfila.h"
#include "liblista.h"
#define TAM 1000

struct pessoa{
	int ticket;
	int vacinado;
	int de_mascara;
	float dinheiro;
};
typedef struct pessoa pessoa_t;

/* struct das informacoes, *razoes vai ser um vetor
   que vai guardar as razoes de 1 a 4 */
struct informacoes{
	float total;
	float refeicoes;
	float mascaras;
	int total_ntickets;
	int *razoes;
};
typedef struct informacoes informacoes_t;

/* Cria a struct informacoes */
informacoes_t* informacoes_cria(){
	informacoes_t *info;
	int i;

	info=malloc(sizeof(informacoes_t));
	if (info == NULL)
		return NULL; 

	info->razoes=malloc(sizeof(int)*TAM);
	if (info->razoes == NULL){
		free(info);
		return NULL;
	}
	
	for (i=0;i<TAM;i++)
		info->razoes[i]=0;

	info->total=0;
	info->refeicoes=0;
	info->mascaras=0;
	info->total_ntickets=0;
	return info;
}

/* funcao que randomiza um valor de a até b */
int aleat(int a,int b){
	int valor;
	
	valor=(rand()%(b-a))+a;
	return valor;
}

/* funcao enche pilha */
void enche_pilha(pilha_t *pilha, int n){
	int i;
	
	for (i=0;i<n;i++)
		push(pilha,1);
}

/* funcao que gera 1 ou 0 dependendo da probabilidade */
int gera_valor(int prob){
	int numero, a, b;
	
	a=1;
	b=100;
	numero=aleat(a,b);
	if (numero <= prob)
		return 1;
	return 0;
}

/* funcao que gera as 1000 pessoas */
void gera_pessoas(pessoa_t *p){
	int i;
	int prob_vacinado;
	int prob_mascara;
	int prob_dinheiro;
	
	prob_vacinado=90;
	prob_mascara=80;
	prob_dinheiro=60;

	for (i=0;i<TAM;i++){
		p[i].ticket=1+i;
		p[i].vacinado=gera_valor(prob_vacinado);
		p[i].de_mascara=gera_valor(prob_mascara);
		if (gera_valor(prob_dinheiro)){
			p[i].dinheiro=1.30;
		}
		else
			p[i].dinheiro=3.80;	
	}
}

/* funcao que coloca os tickets na fila */
void popula_fila(pessoa_t*p, fila_t *fila_ti){
	int i;

	for (i=0;i<TAM;i++)
		queue(fila_ti,p[i].ticket);
}

/* funcao em que o "fiscal" decide o que fazer */
void atendimento_fila(pessoa_t* p, pilha_t* pi_mascaras,pilha_t* pi_refeicoes, fila_t* fila_ti, lista_t* lista_ti, informacoes_t* info){
	int nticket;
	int *n_ticket;
	float valor_comida, valor_mascara;
	
	valor_comida=1.30;
	valor_mascara=2.50;
	nticket=0;
	n_ticket=&nticket;
	while (!pilha_vazia(pi_refeicoes) && (!fila_vazia(fila_ti))){
		dequeue(fila_ti,n_ticket);
		/* Se está vacinado, entra na condicao */
		if (p[*n_ticket-1].vacinado){

			/* Se está de mascara, ganha a refeicao */
			if (p[*n_ticket-1].de_mascara){
				p[*n_ticket-1].dinheiro-=valor_comida;
					info->total+=valor_comida;
					info->refeicoes+=valor_comida;
				if (!pop(pi_refeicoes))
					printf("Retirar da pilha nao deu certo");				
			}

			/* Se a pilha de mascara não está vazia e a pessoa não está de mascara */
			if (!pilha_vazia(pi_mascaras) && !p[*n_ticket-1].de_mascara){
				
				/* Se tem dinheiro pra comprar mascara */
				if ((p[*n_ticket-1].dinheiro >= valor_mascara)){
					p[*n_ticket-1].de_mascara=1;
					p[*n_ticket-1].dinheiro-=valor_mascara;
					info->total+=valor_mascara;
					info->mascaras+=valor_mascara;
					if (!queue(fila_ti,*n_ticket))
						printf("Nao entrou na fila novamente");
					if (!pop(pi_mascaras))
						printf("Retirar da pilha nao deu certo");
				}

				/* Se não tem dinheiro pra comprar máscara */
				else{
					if (lista_insere_ordenado(lista_ti,*n_ticket)){
						info->razoes[*n_ticket-1]=1;
						info->total_ntickets++;
					}		
				}
			}

			/* Se acabou a pilha de mascara */
			if (pilha_vazia(pi_mascaras) && !p[*n_ticket-1].de_mascara){
				if (lista_insere_ordenado(lista_ti,*n_ticket)){
					info->razoes[*n_ticket-1]=2;
					info->total_ntickets++;
				}
			}
		}

		/* Se não está vacinado */
		else{
			if (lista_insere_ordenado(lista_ti,*n_ticket)){
				info->razoes[*n_ticket-1]=3;
				info->total_ntickets++;
			}
		}	
	}
}

/* funcao que dispensa a fila, se ela ainda tem tickets, quer dizer que
   acabou as refeicoes */
void dispensa_fila(fila_t* fila_ti, lista_t* lista_ti, informacoes_t* info){
	int nticket;
	int *n_ticket;
	nticket=0;
	n_ticket=&nticket;

	while (!fila_vazia(fila_ti)){
		dequeue(fila_ti,n_ticket);
		if (!lista_insere_ordenado(lista_ti,*n_ticket))
			printf("Não inseriou ordenado");
		info->total_ntickets++;
		info->razoes[*n_ticket-1]=4;
	}
	fila_ti=fila_destroi(fila_ti);
		if (fila_ti != NULL)
			printf("Fila não destruida com sucesso\n");
}

/* funcao para destruir e liberar memória */
void destroi_tudo (pessoa_t* p, pilha_t* pi_mascara, pilha_t* pi_refeicoes, lista_t* lista_ti, informacoes_t* info){
	free(p);
	pi_mascara=pilha_destroi(pi_mascara);
	if (pi_mascara != NULL)
		printf("Não destruiu a pilha de mascara\n");

	pi_refeicoes=pilha_destroi(pi_refeicoes);
	if (pi_mascara != NULL)
		printf("Não destruiu a pilha de refeicoes\n");

	lista_ti=lista_destroi(lista_ti);
	if (lista_ti != NULL)
		printf("Lista não destruida\n");

	info->total=0;
	info->refeicoes=0;
	info->mascaras=0;
	info->total_ntickets=0;
	free(info->razoes);
	free(info);	
}

int main(){
    srand(time(0));
    pilha_t *pi_mascaras, *pi_refeicoes;
	pessoa_t *p;
	fila_t *fila_ti;
	lista_t *lista_ti;
	informacoes_t *info;
	int nmascaras, nrefeicoes;
	int min, max;
	int nticket;
	int *n_ticket;

	nticket=0;
	n_ticket=&nticket;
	min=1;
	max=100;
	nmascaras=aleat(min,max);
	min=500;
	max=TAM;
	nrefeicoes=aleat(min,max);
	pi_mascaras=pilha_cria(nmascaras);
	if (pi_mascaras == NULL){
		return 1;
		printf("pilha de mascaras não foi alocado");
	}
	
	pi_refeicoes=pilha_cria(nrefeicoes);	
	if (pi_refeicoes == NULL){
		printf("pilha de refeicoes nao foi alocado");
		return 1;
	}
	
	p=malloc(sizeof(pessoa_t)*TAM);
	if (p == NULL){
		printf("struct pessoas nao foi alocado");
		return 1;
	}

	fila_ti=fila_cria();
	if (fila_ti == NULL){
		printf("fila de tickets nao foi alocado");
		return 1;
	}

	lista_ti=lista_cria();
	if (lista_ti == NULL){
		printf("lista de tickets nao utilizados nao foi alocado");
		return 1;
	}
	
	info=informacoes_cria();
	if (info == NULL){
		printf("struct de informacoes nao foi alocada");
		return 1;
	}

	enche_pilha(pi_mascaras,nmascaras);
	enche_pilha(pi_refeicoes,nrefeicoes);
	gera_pessoas(p);
	popula_fila(p, fila_ti);
	/* funcao atendimento_fila vai lidar com os tickets */
	atendimento_fila(p, pi_mascaras, pi_refeicoes, fila_ti, lista_ti, info);
	
	/* Se fila nao vazia, acabou as refeicoes */
	dispensa_fila(fila_ti, lista_ti, info);
	
	/* Imprimindo os resultados */
	printf("O dinheiro arrecadado no total foi:R$ %0.1f0\n",info->total);
	printf("O dinheiro que foi para refeicoes foi:R$ %0.1f0\n",info->refeicoes);
	printf("O dinheiro que rendeu nas vendas de máscaras foi:R$ %0.2f\n",info->mascaras);
	printf("O total de tickets não utilizados foram: %d\n",info->total_ntickets);
	
	/* Imprimindo o numero do ticket e a razao */
	while (!lista_vazia(lista_ti)){
		if (lista_retira_inicio(lista_ti,n_ticket)){
			printf("Número do ticket não utilizado: %d | ",*n_ticket);
			printf("Razão: ");
		}
		else
			printf("Retira lista no inicio não deu certo");

		if (info->razoes[*n_ticket-1] == 1)
			printf("Sem dinheiro para a máscara\n");

		if (info->razoes[*n_ticket-1] == 2)
			printf("Acabou a pilha de mascara\n");

		if (info->razoes[*n_ticket-1] == 3)
			printf("Não está vacinado\n");

		if (info->razoes[*n_ticket-1] == 4)
			printf("Acabaram as refeicoes\n");
	}

	destroi_tudo(p, pi_mascaras, pi_refeicoes, lista_ti, info);
    return 0;
	
}
