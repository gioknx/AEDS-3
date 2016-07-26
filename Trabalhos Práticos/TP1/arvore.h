#pragma once

#include <stdio.h>
#include <stdlib.h>


typedef struct Registro{
	unsigned long long int chave;
	char **campos;
	long numeroCampos;
}Registro;


typedef struct nodo {
	int numeroRegistros;
	long long unsigned *chaves;
	int folha;
	int ordem;
	Registro **registros;
	//struct nodo **apontadores;
	long long unsigned int *offsetFilhos;
	long long unsigned int offset;

	long int prox;
}nodo;


nodo* aloca_nodo(FILE* fp, int ordem, int numeroCampos, int folha);
void desaloca_nodo(nodo* no, int numeroCampos);
void desaloca_registro(Registro* reg, int numeroCampos);
nodo* cria_arvore(FILE* fp, int ordem, int numeroCampos);
void separa_filho(FILE* fp, nodo *x, int i, nodo *y, int numeroCampos);
void insere_nodo_com_espaco(nodo* x, FILE* fp, Registro *reg, int is_raiz);
nodo* wide_search(nodo* raiz, FILE* fpwriter, FILE* fparvore, int numeroCampos);
nodo *inserir(nodo* raiz, FILE* fp, Registro *reg);
Registro* busca(FILE* fp, nodo* x, int k, int numeroCampos);


void salva_raiz(nodo* no, FILE* fp, long numeroCampos);

nodo* carrega_raiz(long long unsigned offset, FILE* fp);


void pa(nodo* node);
