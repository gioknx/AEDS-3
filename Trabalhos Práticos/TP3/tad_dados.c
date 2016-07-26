#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad_dados.h"


// Retorna o numero de palavras em uma string
int words(int size, char* string){
	int i, contador = 0;
	for(i = 0; i < size; i++){
		if(string[i] == ' ') contador++;
	}

	return contador+1;
}
// Copiamos o texto do buffer para a matriz de palavras
void copiaTexto(char **texto, char* string){
	int i, indiceInsercaoPalavra = 0, indiceInsercaoCaracter = 0;
	for(i = 0; i < strlen(string); i++){
		if(string[i] == ' '){
			indiceInsercaoPalavra++;
			indiceInsercaoCaracter = 0;
		} else {
			texto[indiceInsercaoPalavra][indiceInsercaoCaracter++] = string[i];
		}
	}
}

// Retorna o tamanho da linha com as palavras nos indices
// de i até j excludente, ou seja, [i,j)
int tamanhoLinha(Dados *data, int i, int j){
	int k, tamanho = 0;
	for(k = i; k < j; k++)
		tamanho += strlen(data->texto[k]);
	// Espaços entre as palavras
	tamanho += j-i-1;
	return tamanho;
}