#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void escreveBit(unsigned char *bytesArray, int bit, long contador);
void escreveByte(unsigned char *bytesArray, unsigned char letra, long contador);
void escreveLiteral(unsigned char *bytesArray, unsigned char c, long *contador);
void escrevePonteiro(unsigned char *bytesArray, short comprimento, uint16_t offset, long *contador); 
int leBit(unsigned char *bytesArray, long pos);
unsigned char leByte(unsigned char *bytesArray, long pos);
