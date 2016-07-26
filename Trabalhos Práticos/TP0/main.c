#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitMag.h"
#include "kmp.h"
#include "leitor.h"
#include "lz77.h"


int main(int argc, char *argv[]){
	char* source = argv[1];
	char* target = argv[2];

	if(argc != 4){
		printf("Rode o programa na forma <entrada> <saida> <opcao>\n");
		return 0;
	}
	if(*argv[3] == '1'){
		lz77_compress(source,target);
	}else if(*argv[3] == '2'){
			printf("CHEG2OU\n");

		lz77_decompress(source,target);
	}
	return 0;

}