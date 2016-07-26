#include "arvore.h"
#include "fila.h"
Fila* CriaFila(int tamanhoMaximo){
	Fila* f;
	f = (Fila *)malloc(sizeof(Fila));

	f->tamanhoMaximo = tamanhoMaximo;
	f->tamanho = 0;
	return f;
}
void Enfileira(Fila *f, ElementoFila *elemento){
	//Fila cheia
	if(f->tamanho == f->tamanhoMaximo){
		printf("Não é possível adicionar o elemento. Motivo: Fila cheia\n");
	}else if(f->tamanho == 0){
		f->frente = elemento;
		f->tras = f->frente;
		f->tamanho++;
	}else{
		f->tamanho++;
		f->tras->prox = elemento;
		f->tras = elemento;
		f->tras->prox = NULL;
	}
}
ElementoFila* Desenfileira(Fila *f){
	if(f->tamanho == 0)
		printf("Não é possível remover o elemento. Motivo: Fila vazia\n");

	ElementoFila *elemento = f->frente;
	f->frente = f->frente->prox;
	f->tamanho--;

	return elemento;
}