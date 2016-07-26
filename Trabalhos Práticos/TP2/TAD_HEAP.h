#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Elemento{
	int indice;
	int d;
}Elemento;

typedef struct Heap{
	struct Elemento *array;
	int tamanho;
}Heap;



int parent(int i);
int left(int i);
void heap_decrease_key(Heap *heap, int indice, int key);
int right(int i);
Heap* inicializa_heap(int t);
void min_heapify(Heap* heap, int i);
void adiciona_heap(Heap* heap, int i, int d, int indice);
void build_min_heap(Heap* heap);
/*
void heapsort(Heap* heap){
	int i, aux;
	for(i = heap->tamanho; >=2;i--){
		aux = heap->array[i];
		heap->array[i] = heap->array[1];
		heap->array[1] = aux;
		heap->tamanho--;
		min_heapify(heap, 1);
	}
}*/

int extract_min(Heap *heap);
void libera_heap(Heap* heap);


void print_heap(Heap *heap);