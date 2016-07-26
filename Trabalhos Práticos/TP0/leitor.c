#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "leitor.h"

unsigned char* le_arquivo(char *nome,  long *tamanho){
	FILE *fp;
	long fileSize;

	fp = fopen(nome, "rb");  
	fseek(fp, 0, SEEK_END);          
	fileSize = ftell(fp);             
	*tamanho = fileSize;
	rewind(fp);                     
	//printf("Tamanho: %ld\n", tamanho );
	unsigned char *resultado = (unsigned char *)malloc((fileSize)*sizeof(unsigned char)); 
	fread(resultado, 1, fileSize, fp); // Read in the entire file

	//printf("%s\n", buffer );
	fclose(fp); // Close the file
	return resultado;
}