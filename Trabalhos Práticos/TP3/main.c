#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "tad_dados.h"


// A a B-ésima potencia
int int_pow(int a, int b){
	int i, resultado = a;
	for(i = 1; i < b; i++)
		resultado *= a;

	return resultado;
}
int min(int a, int b){
	if(a < b)
		return a;
	return b;
}
int soma_infinito(int a, int b){
	// Se algum é infinito
	if(a == INT_MAX || b == INT_MAX)
		return INT_MAX;
	// Se a soma é >= infinito
	if(a > INT_MAX - b)
		return INT_MAX;
	// Se a soma é >= infinito
	if(b > INT_MAX - a)
		return INT_MAX;

	// Soma normal
	return a+b;
}

/* GREEDY ******************/
// printa as linhas para heuristica greedy
void print_greedy_lines(FILE* fp, Dados *data, Linhas *linhas){
	int i = 0, j, palavraAtual = 0;
	//Custo
	fprintf(fp, "%d\n", badness_greedy(data, linhas));
	for(i; i < linhas->qtdLinhas; i++){
		for(j = 0; j < linhas->qtdPalavrasPorLinha[i]; j++){
			fprintf(fp, "%s", data->texto[palavraAtual++]);
			// Se não for a ultima palavra da linha, escrevemos um espaço
			if(j < linhas->qtdPalavrasPorLinha[i] - 1)
				fprintf(fp, " ");
		}
		if(i != linhas->qtdLinhas-1)
			fprintf(fp, "\n");
	}
}
void greedy(FILE *fp, Dados *data){
	Linhas *linhas = calloc(1, sizeof(Linhas));

	// Pior caso onde cada linha tem apenas uma palavra.
	linhas->tamanhoLinhas = calloc(data->qtdPalavras, sizeof(int));
	linhas->qtdPalavrasPorLinha = calloc(data->qtdPalavras, sizeof(int));

	int tamanhoLinhaAtual = 0, qtdPalavrasLinhaAtual = 0,\
	palavraAtual = 0, qtdLinhas = 0;

	// Para cada linha
	while(palavraAtual < data->qtdPalavras){
		tamanhoLinhaAtual = 0;
		qtdPalavrasLinhaAtual = 0;

		// Adicionamos a primeira palavra na linha
		tamanhoLinhaAtual += strlen(data->texto[palavraAtual++]);
		qtdPalavrasLinhaAtual++;

		if(palavraAtual != data->qtdPalavras){
			
		// Vamos colocando palavras na linha enquanto couber e ainda existirem palavras
			while(tamanhoLinhaAtual + 1 + strlen(data->texto[palavraAtual]) <= data->L){
				tamanhoLinhaAtual += 1;
				tamanhoLinhaAtual += strlen(data->texto[palavraAtual++]);
				qtdPalavrasLinhaAtual++;

				if(palavraAtual == data->qtdPalavras)
					break;
			}
		}

		// Vamos para a proxima linha
		linhas->qtdPalavrasPorLinha[qtdLinhas] = qtdPalavrasLinhaAtual;
		linhas->tamanhoLinhas[qtdLinhas++] = tamanhoLinhaAtual;
	}
	linhas->qtdLinhas = qtdLinhas;

	print_greedy_lines(fp, data, linhas);
	free(linhas->tamanhoLinhas);
	free(linhas->qtdPalavrasPorLinha);
	free(linhas);
}
// calcula o custo para a heuristica greedy
int badness_greedy(Dados *data, Linhas *linhas){
	int primeiraParte, segundaParte = 0, i;
	primeiraParte = data->k * (int_pow(data->H - linhas->qtdLinhas, data->expoente));
	
	for(i = 0; i < linhas->qtdLinhas; i++)
		segundaParte += data->k*\
	(int_pow(data->L - linhas->tamanhoLinhas[i], data->expoente));
	return primeiraParte + segundaParte;
}


/*********************************************/

/*************Dinamica***********************/

// Calcula custo para DP e forca bruta
int badness_dp(Dados *data, int i, int j){
	int tamanho = tamanhoLinha(data, i ,j);
	//printf("badness(%d, %d) = %d\n",i,j,tamanho);
	if(data->L < tamanho)
		return INT_MAX;
	return data->k*(int_pow(data->L - tamanho, data->expoente));
}

// Dinamica
int DP(Dados *data, int i, int numeroLinhas){
	int j, valorMinimo = INT_MAX, minJ, valorAtual, badness, dp;
	
	// Checamos se o numero de linhas atual é permitido
	if(numeroLinhas > data->H){
		return INT_MAX;
	}

	// Se já temos este valor memorizado, vamos retorná-lo
	if(data->memo[i][numeroLinhas] != -1)
		return data->memo[i][numeroLinhas];

	// Condição de parada ou caso base
	if(i == data->qtdPalavras){
		data->memo[i][numeroLinhas] = data->k * (int_pow(data->H - numeroLinhas, data->expoente));

		return data->memo[i][numeroLinhas];

	}else{
		for(j = i + 1; j <= data->qtdPalavras; j++){
			badness = badness_dp(data, i, j);
			dp = DP(data, j, numeroLinhas + 1);
			valorAtual = soma_infinito(badness, dp);

			if( valorAtual < valorMinimo ){
				valorMinimo = valorAtual;
				minJ = j;
			}
		}

		data->memo[i][numeroLinhas] = valorMinimo;
		data->pai[i] = minJ;	
		return valorMinimo;
	}
}

// Reconstroi o texto justificado a partir do vetor pai
void print_lines(FILE* fp, Dados *data){
	int i, pai = data->pai[0], indice = 0;
	while(indice < (data->qtdPalavras-1)){
		fprintf(fp, "%s",data->texto[indice++]);
		if(indice == pai){
			fprintf(fp, "\n");
			pai = data->pai[pai];
		}else{
			fprintf(fp, " ");
		}
	}
	// Ultima palavra
	fprintf(fp, "%s",data->texto[indice]);
}


// Inicia nosso dicionario de memoization
void iniciaDicionario(Dados* data){
	int i,j;
	for(i = 0; i <= data->qtdPalavras; i++){
		for(j = 0; j <= data->qtdPalavras; j++){
			data->memo[i][j] = -1;
		}
	}
}

// Desaloca nosso dicionario de memoization
void desalocaDicionario(Dados* data){
	int i,j;
	for(i = 0; i <= data->qtdPalavras; i++){
		free(data->memo[i]);
	}
	free(data->memo);
}
/********************************/
/************* FORCA BRUTA *********************/
int BF(Dados *data, int i, int numeroLinhas){
	int j, valorMinimo = INT_MAX, minJ, valorAtual, badness, bf;
	int b;

	// Checamos se o numero de linhas atual é permitido
	if(numeroLinhas > data->H)
		return INT_MAX;
	// Condição de parada
	if(i == data->qtdPalavras){
		return data->k * (int_pow(data->H - numeroLinhas, data->expoente));
	}else{
		for(j = i + 1; j <= data->qtdPalavras; j++){
			badness = badness_dp(data, i, j);
			bf = BF(data, j, numeroLinhas + 1);
			valorAtual = soma_infinito(badness, bf);

			if( valorAtual < valorMinimo ){
				valorMinimo = valorAtual;
				minJ = j;
			}
		}
		data->pai[i] = minJ;	
		return valorMinimo;
	}
}
/********************************/

int main(int argc, char const *argv[]){
	// Estrutura que mantem os dados do texto
	Dados *data = calloc(1, sizeof(Dados));

	int L, H, expoente, k, i, qtdPalavras;
	char **texto, *buffer;
	long fileSize;

	FILE *fileIn, *fileOut;
	fileIn = fopen(argv[2], "r");
	fileOut = fopen(argv[3], "w");

	// Pegamos o tamanho do arquivo
	fseek(fileIn, 0, SEEK_END);
	fileSize = ftell(fileIn);
	fseek(fileIn, 0, SEEK_SET);
	buffer = calloc(fileSize, sizeof(char));

	// Lemos os parametros
	fscanf(fileIn, "%d %d", &(data->L), &(data->H));
	fscanf(fileIn, "%d %d\n", &(data->expoente), &(data->k));
	fgets(buffer, fileSize, fileIn);

	// Tiramos o \n da entrada, se existir
	if(buffer[strlen(buffer)-1] == '\n')
		buffer[strlen(buffer)-1] = 0;

	// Alocamos memória para nosso vetor de strings
	data->qtdPalavras = words(strlen(buffer), buffer);
	data->texto = calloc(data->qtdPalavras, sizeof(char*));
	for(i = 0; i < data->qtdPalavras; i++)
		data->texto[i] = calloc(500, sizeof(char));
	copiaTexto(data->texto, buffer);

	if(!strcmp(argv[1], "-g")){
		greedy(fileOut, data);
	} else if(!strcmp(argv[1], "-d")){
		// Alocamos memória para o dicionario
		data->pai = calloc(data->qtdPalavras, sizeof(int));
		data->memo = calloc(data->qtdPalavras+1, sizeof(int*));
		for(i = 0; i <= data->qtdPalavras; i++)
			data->memo[i] = calloc(data->qtdPalavras+1, sizeof(int));

		iniciaDicionario(data);
		fprintf(fileOut, "%d\n",DP(data, 0, 0));
		print_lines(fileOut, data);
		free(data->pai);
		desalocaDicionario(data);
	}else if(!strcmp(argv[1], "-b")){
		data->pai = calloc(data->qtdPalavras, sizeof(int));
		fprintf(fileOut, "%d\n",BF(data, 0, 0));
		print_lines(fileOut, data);
		free(data->pai);

	}
	
	free(buffer);
	for(i = 0; i < data->qtdPalavras; i++)
		free(data->texto[i]);
	free(data->texto);
	free(data);
	fclose(fileIn);
	fclose(fileOut);

	return 0;
}