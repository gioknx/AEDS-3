#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "TAD_GRAFO.h"
#include "TAD_HEAP.h"


// Alocação para a lista de vértices do grafo
void aloca_lista(Grafo *grafo){
	int i;
	//Inicializa lista
	for(i = 0; i < grafo->tamanho; i++){
		grafo->lista[i] = calloc(1, sizeof(Vertice));
		grafo->lista[i]->valor = 0;
		grafo->lista[i]->prox = NULL;
	}
}

void libera_grafo(Grafo* grafo){
	libera_lista(grafo);
	free(grafo->lista);
	free(grafo);
}
void libera_lista(Grafo *grafo){
	int i;
	for(i = 0;i < grafo->tamanho;i++){
		libera_adjacentes(grafo->lista[i]);
	}
}
void libera_adjacentes(Vertice *v){
	int numero_adj = v->valor;
	if(v->prox != NULL)
		libera_adjacentes(v->prox);
	free(v);
}

void inicializa_fonte_unica(Grafo *grafo, int s){
	int i = 0;
	for(i;i<grafo->tamanho;i++){
		grafo->lista[i]->d = 99999999;
		//grafo->lista[i]->pi = NULL;
	}
	grafo->lista[s]->d = 0;
}

void relax(Heap *heap, Grafo *grafo, int u, int v){
	int alt = grafo->lista[u]->d + peso(grafo,u,v);

	if(grafo->lista[v]->d > alt){
		heap_decrease_key(heap, v, alt);
		grafo->lista[v]->d = alt;
		//grafo->lista[v]->pi = grafo->lista[u];
	}
}

void Djkistra(Grafo* grafo, int s){
	// Inicializamos a distancias de todos os veritces com um numero muito grande
	inicializa_fonte_unica(grafo, s);
	int i, u;
	
	// Colocamos todos os vertices como não visitados
	for(i=0;i<grafo->tamanho;i++)
		grafo->lista[i]->S = 0;

	// Adiciona todos os vértices do grafo no heap
	Heap *heap = inicializa_heap(grafo->tamanho);
	for(i=0;i<grafo->tamanho;i++){
		adiciona_heap(heap, i+1, grafo->lista[i]->d, i);
	}
	// Construimos o heap
	build_min_heap(heap);

	Vertice *aux;
//	int numero_adj;
	while(heap->tamanho != 0){
		// Extrai o vértice com a menor distancia da origem
		u = extract_min(heap);

		// Coloca esse vértice no conjunto dos visitados
		grafo->lista[u]->S = 1;
		
		// Aux caminhará pelos adjacentes a u
		aux = grafo->lista[u]->prox;


			while(aux != NULL){
				if(grafo->lista[aux->valor]->S == 0)
					relax(heap, grafo, u, aux->valor);
				aux = aux->prox;
			}
		}
		libera_heap(heap);
	}


	/*void print_path(Grafo* grafo, int i){
		Vertice *v = grafo->lista[i];
		while(v != NULL){
			printf("(%d, %d)", v->x, v->y);
			v = v->pi;
		}
	}*/


	int peso(Grafo *grafo, int a, int k){
		Vertice *aux;
		Vertice* v = grafo->lista[a];
		if(v->prox == NULL){
			//printf("FALHOU POIS NÃO TEVE LIGAÇÃO PARA O PESO ENTRE %d => %d\n",a,k );
			return 0;
		}else{
			aux = v->prox;
	    //Caso vá inserir no começo da lista
			if(aux->valor == k){
				return aux->peso;
			}else{
			//Caminhamento
				while(aux->prox != NULL && k != aux->prox->valor)
					aux = aux->prox;
				if(aux->prox != NULL)
					return aux->prox->peso;
			}
		}
		//printf("FALHOU POIS NÃO TEVE LIGAÇÃO PARA O PESO ENTRE %d => %d\n",a,k );

		return 0;
	}
	void display_grafo(Vertice **lista, int C){
		int i, peso;
		Vertice *temp;
		for(i = 0; i < C; i++) {
			temp = lista[i]->prox;

			printf("%2d: (%d) ==>", i, lista[i]->valor);
			while (temp != NULL) {
				printf("(%d, %d)  ", temp->valor, temp->peso);
	  //printf("%d  ", lista[temp->valor]->peso);
				temp = temp->prox;
			}
			printf("\n");
		}
	}
	bool adj(Vertice *v, int k){
		Vertice *aux;
		if(v->prox == NULL){
			return false;
		}else{
			aux = v->prox;
	// Caso vá inserir no começo da lista
			if(aux->valor == k){
				return 1;
			}else{
			// Caminhamento
				while(aux->prox != NULL && k != aux->prox->valor)
					aux = aux->prox;
				if(aux->prox != NULL)
					return true;
			}
		}

		return false;
	}



	void insere_aresta(Vertice *v, int k, int peso, int peso_origem){
	// Quando adicionamos uma aresta que liga um atalho, o peso é 0
		if(peso == -1)
			peso = 0;
		if(peso_origem == -1)
			peso_origem = 0;

		Vertice *temp, *aux;
		temp = calloc(1, sizeof(Vertice));
		temp->valor = k;
		temp->peso = peso + peso_origem;
	//peso +(v->peso == -1 ? 0 : v->peso);
		temp->prox = NULL;

		v->valor++;

		if(v->prox == NULL){
			v->prox = temp;
		}else{
			aux = v->prox;
	// Caso vá inserir no começo da lista
			if(aux->valor > k){
				temp->prox = aux;
				v->prox = temp;
			}else{
			// Caminhamento
				while(aux->prox != NULL && k > aux->prox->valor)
					aux = aux->prox;

			// Estamos no lugar certo agora
				temp->prox = aux->prox;
				aux->prox = temp;
			}
		}
	}