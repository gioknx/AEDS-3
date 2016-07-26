#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"


void escreve_zeros(FILE* fp, int n){
	int i;
	for(i=0;i<n;i++){
		fputc(0,fp);
	}
}

void serializa_vetor_chaves(FILE* f, nodo* no){
	int i = 1;
	for(i;i<=no->ordem;i++){
		fwrite(&(no->chaves[i]), sizeof(long long unsigned), 1, f);
	}
}

void serializa_vetor_offsetFilhos(FILE* f, long long unsigned int* offsetFilhos, int ordem){
	int i = 1;
	for(i; i <= ordem+1; i++){
		fwrite(&(offsetFilhos[i]), sizeof(long long unsigned int), 1, f);
	}
}

void serializa_matriz_campos(FILE* f, char **campos, int numeroCampos, int isFolha){
	int i
;	for(i = 0;i<numeroCampos;i++){
		// Ocupa 120 bytes
		fwrite((campos[i]), sizeof(char), 30, f);
	}
}

void serializa_registro(FILE* f, Registro *registro, int isFolha){
	//Se temos uma folha, serializamos normalmente
	long long unsigned metadata[2];

	metadata[0] = registro->chave;
	metadata[1] = registro->numeroCampos;
	// Ocupa 16 bytes
	fwrite(&(metadata[0]), sizeof(long long unsigned), 2, f);

	serializa_matriz_campos(f, registro->campos, registro->numeroCampos, isFolha);
}

void serializa_matriz_registros(FILE* f, Registro **registros, long long unsigned ordem, int isFolha, int numeroCampos, int numeroRegistros){
	int i, j;

	for(i = 1; i <= ordem;i++){
		if(registros[i])
			serializa_registro(f, registros[i], isFolha);
		// Caso não tenha um registro, substituiremos por bytes 0
		else{
			escreve_zeros(f,16);
			for(j = 0;j<numeroCampos;j++){
				escreve_zeros(f,120);
			}
		}
	}
}

void serializar(FILE* f, nodo* no, int numeroCampos, long long unsigned offset){
	// Primeiramente, vamos armazenar as primitivas de forma sequencial
	long long unsigned metadata[5];
	int i,j;

	// Colocamos o ponteiro na posição certa
	// Caso o offset passado seja -1, colocamos o ponteiro no final
	if(offset == -1)
		fseek(f, 0, SEEK_END);
	else
		fseek(f, offset, SEEK_SET);


	if(!no->numeroRegistros)
		no->numeroRegistros = 0;
	if(!no->folha)
		no->folha = 0;
	if(!no->ordem)
		no->ordem = 0;
	if(!no->offset)
		no->offset = 0;

	metadata[0] = no->numeroRegistros;
	metadata[1] = no->folha;
	metadata[2] = no->ordem;
	metadata[3] = no->offset;
	metadata[4] = no->prox;

	fwrite(&(metadata[0]), sizeof(long long unsigned), 5, f);

	serializa_vetor_chaves(f, no);

	// Devemos lembrar que folhas não possuem filhos
	// E nós internos não possuem registros
	// Entretanto, salvaremos nossos nós todos com o mesmo tamanho
	serializa_vetor_offsetFilhos(f, no->offsetFilhos, no->ordem);

	// Se for uma folha, serializamos seus registros
	if(no->folha){
		serializa_matriz_registros(f, no->registros, no->ordem, no->folha, numeroCampos, no->numeroRegistros);
	}
}

void desserializa_vetor_chaves(FILE* f, long long unsigned* chaves, int ordem){
	int i = 1;
	for(i; i <= ordem; i++){
		fread(&(chaves[i]), sizeof(long long unsigned), 1, f);
	}
}
void desserializa_vetor_offsetFilhos(FILE* f, long long unsigned int *offsetFilhos, int ordem){
	int i = 1;
	for(i; i <= ordem+1; i++){
		fread(&(offsetFilhos[i]), sizeof(long long unsigned int), 1, f);
	}
}

void desserializa_registros(FILE* f, nodo* no, int ordem){
	// Alocamos espaço para os ponteiros de registros
	no->registros = calloc(ordem+1, sizeof(Registro*));
	int i,j, numeroCampos;
	// Vamos desserializar o numero de registros que possuimos
	for(i=1;i<=no->numeroRegistros;i++){
		// Alocamos memória para este registro
		no->registros[i] = calloc(1, sizeof(Registro));
		fread(&(no->registros[i]->chave), sizeof(long long unsigned), 1, f);
		fread(&(no->registros[i]->numeroCampos), sizeof(long long unsigned), 1, f);
		numeroCampos = no->registros[i]->numeroCampos;

		// Alocamos memória para os campos
		no->registros[i]->campos = calloc(numeroCampos+1, sizeof(char*));
		// Para cada campo, lemos do arquivo
		for(j=0;j<numeroCampos;j++){
			no->registros[i]->campos[j] = calloc(30, sizeof(char));
			fread((no->registros[i]->campos[j]), sizeof(char), 30, f);	
		}
	}


}

void desserializar(FILE* fp, long long unsigned offset, nodo* no){
   // Colocamos o ponteiro na posiçao certa
	fseek( fp, offset, SEEK_SET );

	fread(&(no->numeroRegistros), sizeof(long long unsigned), 1, fp);
	fread(&(no->folha), sizeof(long long unsigned), 1, fp);
	fread(&(no->ordem), sizeof(long long unsigned), 1, fp);
	fread(&(no->offset), sizeof(long long unsigned), 1, fp);
	fread(&(no->prox), sizeof(long long unsigned), 1, fp);
	
	no->chaves = calloc((no->ordem)+1, sizeof(long long unsigned));
	no->offsetFilhos = calloc((no->ordem)+2, sizeof(long long unsigned int));
	
	desserializa_vetor_chaves(fp, no->chaves, no->ordem);
	desserializa_vetor_offsetFilhos(fp, no->offsetFilhos, no->ordem);

	//Desserializações referente às folhas, que possuem os registros
	if(no->folha){
		desserializa_registros(fp, no, no->ordem);
	}
}