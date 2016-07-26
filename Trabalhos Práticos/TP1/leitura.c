#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"



/* MANIPULAÇÃO DA LEITURA */

// Convertemos nosso registro em uma string para ser printada no arquivo de saida
char *registro_toString(Registro *r){
	int i;
	char *saida = calloc(1024, sizeof(char));
	for (i = 0; i < r->numeroCampos; i++){
		if(i > 0)
			strcat(saida, "\t");
		strcat(saida, r->campos[i]);
	}

	return saida;
}


// Retiramos da linha os campos para inserirmos no registro
char **extract_fields_from_line(char* line, int numero_campos, char* op){
	int i,aux = 0, indice_insercao = 0;
	char **campos;
	
	// Alocamos memoria para nossa matriz contendo os campos lidos
	campos = calloc(numero_campos, sizeof(char*));
	for(i=0;i<numero_campos;i++){
		campos[i] = calloc(30, sizeof(char));
	}

	char *pch;
	pch = strtok(line,"\t");
	while (pch != NULL)
	{
		// Adicionamos a operação na variavel que guarda isso
		if(aux == 0){
			strcpy(op,pch);
		}else{
			strcpy(campos[indice_insercao++], pch);
		}

		aux++;
		pch = strtok (NULL, "\t");
	}

	// Corrigimos a leitura do dump, que vem com \n
	if(op[0] == 'd')
		strcpy(op,"dump");
	return campos;
}

void desaloca_campos(char** campos, int numero_campos){
	int x;
	for(x=0;x<numero_campos;x++)
		free(campos[x]);
	free(campos);
}


void main(int argc, char **argv){
	if(argc <= 1){
		printf("Numero de parametros invalidos\n");return;
	}
	
	// Variavel auxiliar que irá manter o offset da raiz entre as operações
	// de salvamento e carregamento
	long long unsigned offset_raiz;
	int i;

	char ** campos;
	// Variavel que ira conter a operacao a ser feita
	char *operacao = (char*)malloc(sizeof(char) * 10);;
	
	// Leitura dos parametros
	char line[1024];
	char *saida;
	char* arquivo_saida = argv[1];
	char* arquivo_entrada = argv[2];
	int ordem = 0;
	int numero_campos = 0;
	int indice = 0;

	ordem = atoi(argv[3]);
	numero_campos = atoi(argv[4]);
	indice = atoi(argv[5]);

	FILE* fpreader = fopen( arquivo_entrada, "r");
	FILE *fpwriter = fopen( arquivo_saida , "w" );
	FILE *fparvore = fopen( "arvore.txt" , "w+" );

	nodo* raiz = cria_arvore(fparvore, ordem, numero_campos);

	// Lemos o arquivo, linha por linha
	while (1) {

		if (fgets(line, 1024, fpreader) == NULL) break;

		int tamanho = strlen(line);
		if (line[tamanho -1] == '\n')
			line[tamanho-1] = '\0';

		// Extraimos os campos da linha lida os campos 
		// Para colocarmos em um registro e inserir na B-TREE*
		campos = extract_fields_from_line(line, numero_campos, operacao);

		// Tomada de decisão baseado no que lemos
		if(!strcmp(operacao,"add")){
			// INSERÇÃO		
			Registro *r = (Registro*)malloc(sizeof(Registro));

			// Copiamos os campos lidos para nosso registro
			r->campos = calloc(numero_campos, sizeof(char*));
			for(i=0;i<numero_campos;i++){
				r->campos[i] = calloc(30, sizeof(char));
				strcpy(r->campos[i], campos[i]);
			}
			r->numeroCampos = numero_campos;
			r->chave = atoi(campos[indice]);

			//printf("Adiciona: %llu\n", r->chave);

			// Efetivamente insere o registro na raiz
			raiz = inserir(raiz, fparvore, r);

		} else if(!strcmp(operacao,"search")){
			// Operação de busca
			Registro *rb;
			int valorBusca = atoi(campos[0]);
			// Salvamos a raiz pois ela será desalocada no processo de busca
			salva_raiz(raiz, fparvore, numero_campos);
			offset_raiz = raiz->offset;

			rb = busca(fparvore, raiz, valorBusca, numero_campos);
			
			// Carregamos novamente a raiz para a memória
			raiz = carrega_raiz(offset_raiz, fparvore);

			fprintf(fpwriter, "search\n");
			if(rb != NULL){
				saida = registro_toString(rb);
				fprintf(fpwriter, "%s\n", saida);
				desaloca_registro(rb, numero_campos);
				free(saida);
			}else{
				fprintf(fpwriter, "null\n");
			}
			fprintf(fpwriter, "search\n");
			//printf("\nBusca: %d\n", valorBusca);
		} else if(!strcmp(operacao,"dump")){
			// DUMP DA ARVORE
			fprintf(fpwriter, "dump\n");
			
			// Salvamos a raiz do jeito que está para não perdemos ela durante o processo
			// Pois iremos recuperá-la do arquivo no final desta função
			salva_raiz(raiz, fparvore, numero_campos);
			offset_raiz = raiz->offset;
			wide_search(raiz, fpwriter, fparvore, numero_campos);
			// Recuperamos a raiz da forma que ela estava no começo da execução
			// Evitando qualquer problema como perda de referencia

			raiz = carrega_raiz(offset_raiz, fparvore);
			
			fprintf(fpwriter, "dump\n");
		} 

		desaloca_campos(campos, numero_campos);

	}

	free(operacao);
	fclose(fpreader);
	fclose(fpwriter);
	fclose(fparvore);
	desaloca_nodo(raiz, numero_campos);
}
