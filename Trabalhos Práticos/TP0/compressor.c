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

	if(argc != 3){
		printf("Digite na forma ./compressor.exe entrada saida");
		return 1;	
	}
	lz77_compress(source,target);
	
	return 0;

}
