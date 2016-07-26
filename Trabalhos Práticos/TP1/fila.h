#pragma once
#include "arvore.h"


typedef struct ElementoFila{
	struct ElementoFila* prox;
	nodo* nodo;
}ElementoFila;

typedef struct Fila{
	ElementoFila *frente, *tras;
	int tamanho;
	int tamanhoMaximo;
}Fila; 

Fila* CriaFila(int tamanhoMaximo);

void Enfileira(Fila *f, ElementoFila *elemento);

ElementoFila* Desenfileira(Fila *f);