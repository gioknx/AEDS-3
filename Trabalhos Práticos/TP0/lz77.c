#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitMag.h"
#include "kmp.h"
#include "leitor.h"
#include "lz77.h"

int lz77_decompress(char* source, char* target){


	long* tamanho = (long*)malloc(sizeof(long));
	long *contador = (long*) malloc(sizeof(long));
	long comprimento;
	
	// Lemos o arquivo e colocamos num array de bytes
	unsigned char* bytes = le_arquivo(source, tamanho);

	// Array que conterá os bytes a serem escritos
	unsigned char *arrayFinal = (unsigned char*)malloc(sizeof(unsigned char)*(*tamanho));
	int insereArray = 0, helper, bit, posicao = 0, tamanhoArray;
	
	tamanhoArray = (*tamanho);
	*contador = 0;
	uint16_t offset;


	// Caminhamos pelos bytes até faltar um numero menor que 8, ou seja, será o padding
	while((((*tamanho)*8) - (posicao+1) ) >  8){
		bit = leBit(bytes, posicao);
		// Se for literal, colocamos o byte logo após a posicao
		if(bit == 0){
			arrayFinal[insereArray++] = leByte(bytes, posicao+1);
			posicao+=9;
			if(insereArray == (tamanhoArray)){
				tamanhoArray *= 2;
				arrayFinal = (unsigned char *)realloc(arrayFinal, tamanhoArray * sizeof(unsigned char));
			}
			// Se for ponteiro, colocamos o byte de comprimento e os 15 bits de offset
		}else if(bit == 1){
			comprimento = 0;
			offset = 0;
			// Lemos os bytes correspondentes ao ponteiro
			comprimento = (long)leByte(bytes, posicao+1);
			offset = leByte(bytes, posicao+9);
			offset = (offset >> 1) << 8;
			offset |= leByte(bytes, posicao+16);
			comprimento+=3;
			offset+=1;
			for(helper = 0; helper < comprimento; helper++){
				
				//Crescemos o array caso seja necessario
				if(insereArray + helper == (tamanhoArray)){
					tamanhoArray *= 2;
					arrayFinal = (unsigned char *)realloc(arrayFinal, tamanhoArray * sizeof(unsigned char));
				}

				arrayFinal[insereArray + helper] = arrayFinal[insereArray - offset + helper];
			}
			insereArray += comprimento;
			posicao+=24;

		}

	}

	FILE *file = fopen(target, "wb");
	fwrite(arrayFinal, sizeof(unsigned char), insereArray, file);

	fclose(file);
	free(tamanho);
	free(contador);
	free(bytes);
	free(arrayFinal);

	return 0;

}



int lz77_compress(char* source, char* target){
	unsigned char* c = {0};
	long* tamanho = (long*)malloc(sizeof(long));
	c = le_arquivo(source, tamanho);

	// Vetor de flags para saber se o primeiro de um conjunto de 3 caraceteres já passaram pelo CS
	int *byteFlags = (int*)malloc(sizeof(int)*(*tamanho));
	int setter = 0;
	for(;setter < (*tamanho); setter++)
		byteFlags[setter] = 0;
	// Vetor de inteiros que vai conter as posições onde um matching foi encontrnado
	int* posicoesEncontradas = (int*)malloc(sizeof(int)*32768);

	int i = 3, j = 6, verificadorMaisCaracteres = 3, maxDelta = 0, maxDeltaPosicao = 0, auxDelta, flagToSet = 1, numeroPosicoes, tamanhoJanela;

 	// Criamos o array de bytes que armazenará os bytes a serem escritos
	unsigned char *bytesArray = (unsigned char*)malloc((*tamanho) * sizeof(unsigned char));
	long *contador = (long*) malloc(sizeof(long) * 1);
	*contador = 0;
	// Zeramos o array
	memset(bytesArray, 0, *tamanho);


	// Setamos os bytesFlags iniciais como 1, pois os tres passam pelo CS
	// Escrevemos tambem os tres primeiros literais, pois eles nunca terao match
	int helper = 0;
	for(helper;helper<3;helper++){
		//printf("%c",c[helper]);
		escreveLiteral(bytesArray, c[helper], contador);
		byteFlags[helper] = 1;
	}



	// Esse for representa o caminhamento da janela ou CS
	for(i = 3; i < (*tamanho);i++){
		byteFlags[i] = 1;
		maxDelta = 0;	


		// Checamos se estamos nos 2 ultimos. Se estivermos, eles são literais
		if(i >= (*tamanho)-3){
			numeroPosicoes = 0;	
		}else{
			// Rodamos o KMP e descobrimos todas as posições de ocorrencia
			// Passando o vetor antes da janela até a segunda posicao e a janela
			/*if( (i - 32768) < 0){
				tamanhoJanela = 0;
			} else{
				tamanhoJanela = i - 32768;
			}*/
			tamanhoJanela = 0;
			numeroPosicoes = kmp(&c[tamanhoJanela], (i+2 - tamanhoJanela), &c[i], 3, posicoesEncontradas, byteFlags);
		}
		if(numeroPosicoes > 0){
			// Iteramos nas posicoes de matching para descobrir qual tem o maior comprimento
			for(auxDelta = 0; auxDelta < numeroPosicoes; auxDelta++){
				verificadorMaisCaracteres = 3;
				// Caso tenha encontrado algum matching, verificamos se tem mais caracteres que podem ser matcheds
				while(c[posicoesEncontradas[auxDelta]+verificadorMaisCaracteres] == c[i + verificadorMaisCaracteres] && verificadorMaisCaracteres < 255){
					verificadorMaisCaracteres++;
				}

				// Caso esse seja o matching com maior posicao, guardamos esta posicao. Se for igual, pegamos o mais recente
				if(verificadorMaisCaracteres >= maxDelta){
					maxDelta = verificadorMaisCaracteres;
					maxDeltaPosicao = posicoesEncontradas[auxDelta];
				}
			}
			escrevePonteiro(bytesArray, (short)maxDelta, (uint16_t)(i-maxDeltaPosicao), contador);
				// Deslocamos o i na posicao do comprimento -1, pois o for já vai incrementa-lo
			i += maxDelta-1;

		} else{
			escreveLiteral(bytesArray, c[i], contador);
		}

		// Resetamos os valores para a próxima busca
		memset(posicoesEncontradas, 0, sizeof(32768));
		verificadorMaisCaracteres = 3;
	}

	// Ao terminar de guardar todos os bytes no nosso vetor, escrevemos no arquivo
	FILE* output = fopen(target, "wb");
	long numeroBytes;
	// Se tivermos um numero exato de bytes, o tamanho seŕa ele. Se não, ele mais 1 e o resto desse adicional completa com 0
	if(*contador%8 == 0){
		numeroBytes = (*contador/8);
	}else{
		numeroBytes = (*contador/8)+1;
	}
	fwrite(bytesArray, sizeof(unsigned char), numeroBytes, output);
	fclose(output);

    // Liberamos os ponteiros utilizados
	free(c);
	free(tamanho);
	free(byteFlags);
	free(posicoesEncontradas);
	free(bytesArray);

	free(contador);


	return(0);
}

