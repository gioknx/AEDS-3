#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "bitMag.h"


void escreveBit(unsigned char *bytesArray, int bit, long contador){
	if (bit == 1) {
		long deslocamento = 7 - (contador%8);
		bytesArray[contador/8] |= (1<<deslocamento);
	}

}

void escreveByte(unsigned char *bytesArray, unsigned char letra, long contador){
	if (!(contador%8)) {
		bytesArray[contador/8] = letra;
	} else {
		long deslocamento = contador%8;
		// Se não, colocamos uma parte do byte no que sobrou do byte atual
		bytesArray[contador/8] |= letra>>deslocamento;
		// E o resto no próximo byte 
		bytesArray[(contador/8) + 1] |= letra<<(8 - deslocamento);
	}
}


void escreveLiteral(unsigned char *bytesArray, unsigned char c, long *contador){
	// Identificador
	escreveBit(bytesArray, 0, *contador);
	(*contador)++;

	// Literal
	escreveByte(bytesArray, c, *contador);
	(*contador) += 8;

}

	// Recebemos uint8 pois o comprimento é sempre 8 bits e uint16 pois o offset sempre tem 15 bits
void escrevePonteiro(unsigned char *bytesArray, short comprimento, uint16_t offset, long *contador){

	offset -= 1;
	comprimento -= 3;
	comprimento = (unsigned char)comprimento;
	escreveBit(bytesArray, 1, *contador);
	(*contador)++;

	escreveByte(bytesArray, comprimento, *contador);
	(*contador) += 8;

	// Colocamos os 7 bits da esquerda do offset
	escreveByte(bytesArray, (offset>>8)<<1, *contador);
	(*contador) += 7;

	// Resto do offset
	escreveByte(bytesArray, (unsigned char)(offset), *contador);
	(*contador) += 8;

}


int leBit(unsigned char *bytesArray, long pos){
	long deslocamento = 7 - (pos%8);
	if((bytesArray[pos/8] & (1<<deslocamento)) != 0)
		return 1;
	return 0;
}	

unsigned char leByte(unsigned char *bytesArray, long pos){
	// Se temos uma posicao exata, retornamos o byte na posicao
	if (pos%8 == 0) {
		return bytesArray[pos/8];
	}
    // Pegamos uma parte de um byte
	unsigned char byteEsquerda = bytesArray[pos/8]<<(pos%8);
	unsigned char byteDireita = bytesArray[(pos/8) + 1]>>(8 - (pos%8));

    // Somamos, formando o byte na posição correta
	return byteEsquerda | byteDireita;
}