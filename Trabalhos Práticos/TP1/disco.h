#pragma once

#include <stdio.h>

void escreve_zeros(FILE* fp, int n);
void serializar(FILE* f, nodo* no, int numeroCampos, long long unsigned offset);
void serializa_vetor_chaves(FILE* f, nodo* no);
void serializa_vetor_offsetFilhos(FILE* f, long long unsigned int* offsetFilhos, int ordem);
//void serializa_matriz_registros(FILE* f, Registro **registros, long long unsigned ordem, int isFolha, int numeroCampos);
void serializa_matriz_registros(FILE* f, Registro **registros, long long unsigned ordem, int isFolha, int numeroCampos, int numeroRegistros);

void serializa_registro(FILE* f, Registro *registro, int isFolha);
void serializa_matriz_campos(FILE* f, char **campos, int numeroCampos, int isFolha);
void desserializar(FILE* fp, long long unsigned offset, nodo* no);
void desserializa_vetor_chaves(FILE* f, long long unsigned* chaves, int ordem);
void desserializa_vetor_offsetFilhos(FILE* f, long long unsigned int* offsetFilhos, int ordem);
void desserializa_registros(FILE* f, nodo* no, int ordem);