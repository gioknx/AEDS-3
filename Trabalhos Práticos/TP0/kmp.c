#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kmp.h"

void prefixo(unsigned char* p, int m, int *pi){
	int q = 1, k = -1;
	pi[0] = k;

	for(;q < m;q++){
		while(k > -1 && p[k+1] != p[q])
			k = pi[k];
		if(p[k+1] == p[q])
			k++;
		pi[q] = k;
	}
}

// Modifiquei o algoritimo KMP do Cormem de modo que ele insira todas as posicoes de matching
// em um vetor de inteiros, para depois comparar e ver qual tem o maior comprimento


int kmp(unsigned char* t, int n, unsigned char* p, int m, int* posicoes, int* byteFlags){
	int indicePosicoes = 0;
	int *pi = (int*)malloc(sizeof(int)*m); 
	int i = 0, q = -1;
	prefixo(p,m,pi);
	for(;i < n;i++){
		while(q>-1 && p[q+1] != t[i])
			q = pi[q];
		if(p[q+1] == t[i])
			q++;
		if(q == m-1){
			// So sera um match esse os tres tiverem sido CS, mostramos isso colocando flag no primeiro
			if(byteFlags[i-q] == 1){
				// So entra pro vetor de posicoes se tiver dentro do offst permitido
				if((n-2) - (i-q) <= 32768)
					posicoes[indicePosicoes++] = i-q;
			}
			q = pi[q];
		}
	}

	free (pi);
	return indicePosicoes;
}
