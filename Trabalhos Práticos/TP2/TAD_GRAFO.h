#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct Vertice{
	int valor;
	int peso;
	int x;
	int y;
	int d;
	int S;
	struct Vertice* prox;
	//struct Vertice* pi;
}Vertice;

typedef struct Grafo{
	Vertice **lista;
	int tamanho;
}Grafo;


// Printa o grafo
void display_grafo(Vertice **lista, int C);
// Retorna true ou false sobre um vertice ser adjancente a um vertice de numero K
bool adj(Vertice *v, int k);
//A primeira celula guarda o número de vertices adjacentes
void insere_aresta(Vertice *v, int k, int peso, int peso_origem);
// Alocação para a lista de vértices do grafo
void aloca_lista(Grafo *grafo);
void libera_grafo(Grafo* grafo);
void libera_lista(Grafo *grafo);
void libera_adjacentes(Vertice *v);