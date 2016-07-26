#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "TAD_HEAP.h"



int parent(int i){
	return i/2;
}


int left(int i){
	return 2*i;
}

int right(int i){
	return (2*i) + 1;
}

void libera_heap(Heap* heap){
	free(heap->array);
	free(heap);
}
void heap_decrease_key(Heap *heap, int indice, int key){
	int i = busca_indice(heap, indice);
	if(key > heap->array[i].d)	
		//printf("Nova chave é maior que anterior\n");
		heap->array[i].d = key;

	while(i > 1 && heap->array[parent(i)].d > heap->array[i].d){
		Elemento e;
		e = heap->array[i];
		heap->array[i] = heap->array[parent(i)];
		heap->array[parent(i)] = e;
		i = parent(i);
	}	
}

int busca_indice(Heap* heap, int indice){
	int i;
	for(i = 1; i <= heap->tamanho;i++){
		if(heap->array[i].indice == indice){
			return i;
		}
	}
	//printf("Nao achou elemento no aatualiza elemento\n");
	return 99999999;
}

void print_heap(Heap *heap){
	//printf("PRINTING HEAP\n");
	int i;
	for(i = 1; i <= heap->tamanho;i++){
		printf("Posição no heap: %d       D: %d  Indice_Vertice: %d\n", i, heap->array[i].d, heap->array[i].indice );
	}
}

Heap* inicializa_heap(int t){
	Heap* heap = calloc(1, sizeof(Heap));
	heap->tamanho = 0;
	heap->array = calloc(t+1, sizeof(Elemento));

	return heap;
}

void adiciona_heap(Heap* heap, int i, int d, int indice){
	Elemento e;
	e.d = d;
	e.indice = indice;
	heap->array[i] = e;
	heap->tamanho++;
}
void min_heapify(Heap* heap, int i){
	int l = left(i);
	int r = right(i);
	Elemento aux;
	int minimo;

	if(l <= heap->tamanho ){
		if((heap->array[l].d < heap->array[i].d)){
			minimo = l;
		} else{
			minimo = i;
		}
	}else{
		minimo = i;
	}

	if(r <= heap->tamanho){
		if(heap->array[r].d < heap->array[minimo].d)
			minimo = r;
	}  

	if(minimo != i){
		aux = heap->array[i];
		heap->array[i] = heap->array[minimo];
		heap->array[minimo] = aux;
		min_heapify(heap, minimo);
	}
}

void build_min_heap(Heap* heap){
	int i ;
	for(i = (heap->tamanho/2); i >= 1;i--)
		min_heapify(heap, i);
}


int extract_min(Heap *heap){
	//if(heap->tamanho < 1)
		//printf("Erro para extrair do heap\n");
	int min = heap->array[1].indice;
	heap->array[1] = heap->array[heap->tamanho];
	heap->tamanho--;
	min_heapify(heap, 1);

	return min;
}