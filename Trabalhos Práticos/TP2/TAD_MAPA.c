#include "TAD_GRAFO.h"
#include "TAD_HEAP.h"
#include "TAD_MAPA.h"

void libera_mapa(Mapa* mapa){
	int i;
	for(i=0;i<mapa->N;i++){
		free(mapa->matriz_mapa[i]);
	}

	free(mapa->lista_atalhos);
	free(mapa->matriz_mapa);
	free(mapa);
}

// Essa função irá conectar todos os atalhos com o custo 0 entre eles
void conecta_atalhos(Grafo *grafo, Mapa *map){
	int i, j;
	for(i = 0; i < map->tamanho_lista_atalhos;i++){
		for(j = 0; j < map->tamanho_lista_atalhos;j++){
			if(i != j)
				insere_aresta(grafo->lista[map->lista_atalhos[i]], map->lista_atalhos[j], 0, 0);
		}
	}

}

// Lemos a entrada do arquivo e colocamos em um mapa de pesos 
// que será posteriormente convertido para um grafo
Mapa* ler_entrada(FILE *fp){
	Mapa *map = calloc(1, sizeof(Mapa));
	int N, M;
	int indice_lista_atalhos = 0;
	int** matriz_mapa;


	fscanf(fp, "%d %d", &M, &N);
	int *lista_atalhos = calloc(M*N, sizeof(int));

	matriz_mapa = calloc(N, sizeof(int*));
	int i, j, valor;

	
	for(i=0;i<N;i++){
		matriz_mapa[i] = calloc(M, sizeof(int));

		for(j=0;j < M;j++){
			fscanf(fp, "%d", &(valor));
			matriz_mapa[i][j] = valor;
			if(valor == -1)
				lista_atalhos[indice_lista_atalhos++] = (i*M) + j;
		}
	}

	map->N = N;
	map->M = M;
	map->tamanho_lista_atalhos = indice_lista_atalhos;
	map->lista_atalhos = lista_atalhos;
	map->matriz_mapa = matriz_mapa;
	
	return map;
}

void print_indices(int N, int M){
	int i,j, c = 0;
	for(i=0;i < N;i++){
		for(j=0;j < M;j++){
			printf("%d ", c++);
		}
		printf("\n");
	}
}

// Retorna o minimo entre dois numeros
int min(int a, int b){
	if(a >= b)
		return b;
	return a;
}

// Dado um vértice, precisamos inserir as arestas para os possiveis caminhamentos dele
// levando em consideração a restrição de movimento d_x e d_y.
void insere_adjacentes(int N, int M, int d_x, int d_y, int** matriz_mapa, int i, int j, Vertice** lista){
	int indice_vertice = (i*M)+j;
	lista[indice_vertice]->peso = matriz_mapa[i][j];
	lista[indice_vertice]->x = j;
	lista[indice_vertice]->y = i;
	if(matriz_mapa[i][j] == 0) return;

	// No primeiro caso temos restrição 0 0, ou seja, podemos mover uma casa para qualquer direção
	if(d_x == 0 && d_y == 0){
		int indice_vertice_esquerda = indice_vertice - 1;
		int indice_vertice_direita = indice_vertice + 1;
		int indice_vertice_acima = indice_vertice - M;
		int indice_vertice_abaixo = indice_vertice + M;
		if(j > 0 && matriz_mapa[i][j-1] != 0){
			insere_aresta(lista[indice_vertice], indice_vertice_esquerda, matriz_mapa[i][j-1], matriz_mapa[i][j]);
		}
		if(j < M-1 && matriz_mapa[i][j+1] != 0){
			insere_aresta(lista[indice_vertice], indice_vertice_direita, matriz_mapa[i][j+1], matriz_mapa[i][j]);
		}
		if(i > 0 && matriz_mapa[i-1][j] != 0){
			insere_aresta(lista[indice_vertice], indice_vertice_acima, matriz_mapa[i-1][j], matriz_mapa[i][j]);
		}
		if(i < N-1 && matriz_mapa[i+1][j] != 0){
			insere_aresta(lista[indice_vertice], indice_vertice_abaixo, matriz_mapa[i+1][j], matriz_mapa[i][j]);
		}
	}else{
		int peso_x, peso_y, peso_x2, peso_y2, pesototal_1, pesototal_2, valido1, valido2;
		int x = j, y = i;
		int q1_x = j + d_x;
		int q1_y = i - d_y;

		int q2_x = j + d_x;
		int q2_y = i + d_y;

		int q3_x = j - d_x;
		int q3_y = i + d_y;

		int q4_x = j - d_x;
		int q4_y = i - d_y;


		// Quadrante 1
		if(q1_x <= M-1 && q1_y >= 0 && matriz_mapa[q1_y][q1_x] != 0){
		// Pra cima e pra direita
			peso_y = valida_caminho_pelo_y(matriz_mapa, x, y, q1_y); 
			peso_x = valida_caminho_pelo_x(matriz_mapa, x, q1_y, q1_x);
			pesototal_1 = peso_x + peso_y;

			// Pra direita e pra cima
			peso_x2 = valida_caminho_pelo_x(matriz_mapa, x, y, q1_x);
			peso_y2 = valida_caminho_pelo_y(matriz_mapa, q1_x, y, q1_y);
			pesototal_2 = peso_x2 + peso_y2;


			// Validação dos caminhos para garantir que são válidos, isto é, sem 0 no meio
			valido1 = 0;
			valido2 = 0;
			if(peso_x != -1 && peso_y != -1)
				valido1 = 1;
			if(peso_x2 != -1 && peso_y2 != -1)
				valido2 = 1;

			if(valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q1_y*M)+q1_x, min(pesototal_1, pesototal_2), 0);
			} 
			if(valido1 && !valido2){
				insere_aresta(lista[indice_vertice], (q1_y*M)+q1_x, pesototal_1, 0);
			}
			if(!valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q1_y*M)+q1_x, pesototal_2, 0);
			}
		}


		// Quadrante 2
		if(q2_x <= M-1 && q2_y <= N-1 && matriz_mapa[q2_y][q2_x] != 0){
			// Pra baixo e pra direita
			peso_y = valida_caminho_pelo_y(matriz_mapa, x, y, q2_y);
			peso_x = valida_caminho_pelo_x(matriz_mapa, x, q2_y, q2_x);
			pesototal_1 = peso_x + peso_y;

			// pra direita e pra baixo
			peso_x2 = valida_caminho_pelo_x(matriz_mapa, x, y, q2_x);
			peso_y2 = valida_caminho_pelo_y(matriz_mapa, q2_x, y, q2_y);
			pesototal_2 = peso_x2 + peso_y2;


			// Validação dos caminhos para garantir que são válidos, isto é, sem 0 no meio
			valido1 = 0;
			valido2 = 0;
			if(peso_x != -1 && peso_y != -1)
				valido1 = 1;
			if(peso_x2 != -1 && peso_y2 != -1)
				valido2 = 1;

			if(valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q2_y*M)+q2_x, min(pesototal_1, pesototal_2), 0);
			} 
			if(valido1 && !valido2){
				insere_aresta(lista[indice_vertice], (q2_y*M)+q2_x, pesototal_1, 0);
			}
			if(!valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q2_y*M)+q2_x, pesototal_2, 0);
			}
		}

		// Quadrante 3
		if(q3_x >= 0 && q3_y <= N-1 && matriz_mapa[q3_y][q3_x] != 0){
			// Pra baixo e pra esquerda
			peso_y = valida_caminho_pelo_y(matriz_mapa, x, y, q3_y);
			peso_x = valida_caminho_pelo_x(matriz_mapa, x, q3_y, q3_x);
			pesototal_1 = peso_x + peso_y;

			// pra esquerda e pra baixo
			peso_x2 = valida_caminho_pelo_x(matriz_mapa, x, y, q3_x);
			peso_y2 = valida_caminho_pelo_y(matriz_mapa, q3_x, y, q3_y);
			pesototal_2 = peso_x2 + peso_y2;


			// Validação dos caminhos para garantir que são válidos, isto é, sem 0 no meio
			valido1 = 0;
			valido2 = 0;
			if(peso_x != -1 && peso_y != -1)
				valido1 = 1;
			if(peso_x2 != -1 && peso_y2 != -1)
				valido2 = 1;

			if(valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q3_y*M)+q3_x, min(pesototal_1, pesototal_2), 0);
			} 
			if(valido1 && !valido2){
				insere_aresta(lista[indice_vertice], (q3_y*M)+q3_x, pesototal_1, 0);
			}
			if(!valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q3_y*M)+q3_x, pesototal_2, 0);
			}
		}

		// Quadrante 4
		if(q4_x >= 0 && q4_y >= 0 && matriz_mapa[q4_y][q4_x] != 0){
		// Pra cima e pra esquerda
			peso_y = valida_caminho_pelo_y(matriz_mapa, x, y, q4_y); 
			peso_x = valida_caminho_pelo_x(matriz_mapa, x, q4_y, q4_x);
			pesototal_1 = peso_x + peso_y;

		// Pra esquerda e pra cima
			peso_x2 = valida_caminho_pelo_x(matriz_mapa, x, y, q4_x);
			peso_y2 = valida_caminho_pelo_y(matriz_mapa, q4_x, y, q4_y);
			pesototal_2 = peso_x2 + peso_y2;

			// Validação dos caminhos para garantir que são válidos, isto é, sem 0 no meio
			valido1 = 0;
			valido2 = 0;
			if(peso_x != -1 && peso_y != -1)
				valido1 = 1;
			if(peso_x2 != -1 && peso_y2 != -1)
				valido2 = 1;

			if(valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q4_y*M)+q4_x, min(pesototal_1, pesototal_2), 0);
			} 
			if(valido1 && !valido2){
				insere_aresta(lista[indice_vertice], (q4_y*M)+q4_x, pesototal_1, 0);
			}
			if(!valido1 && valido2){
				insere_aresta(lista[indice_vertice], (q4_y*M)+q4_x, pesototal_2, 0);
			}
		}
	}
}

// Faz um caminhamento no eixo Y a partir de uma posiçao e retorna o peso desse caminho
// ou -1 caso tenha um obstáculo
int valida_caminho_pelo_y(int **matriz_mapa, int x, int y, int d_y){
	int i, peso = 0;
	if(d_y >= y){
		for(i = y; i <= d_y; i++){
			if(matriz_mapa[i][x] == 0)
				return -1;
			else if(matriz_mapa[i][x] != -1){
				if(i == y || i == d_y){
					peso += matriz_mapa[i][x];
				} else{
					peso += (2*matriz_mapa[i][x]);
				}
			}
		}
	} else{
		for(i = y; i >= d_y; i--){
			if(matriz_mapa[i][x] == 0)
				return -1;
			else if(matriz_mapa[i][x] != -1){
				if(i == y || i == d_y){
					peso += matriz_mapa[i][x];
				} else{
					peso += (2*matriz_mapa[i][x]);
				}
			}
		}
	}
	return peso;
}
// Faz um caminhamento no eixo X a partir de uma posiçao e retorna o peso desse caminho
// ou -1 caso tenha um obstáculo
int valida_caminho_pelo_x(int **matriz_mapa, int x, int y, int d_x){
	int i, peso = 0;
	if(d_x >= x){
		for(i = x; i <= d_x; i++){
			if(matriz_mapa[y][i] == 0)
				return -1;
			else if(matriz_mapa[y][i] != -1){
				if(i == x || i == d_x){
					peso += matriz_mapa[y][i];
				} else{
					peso += (2*matriz_mapa[y][i]);
				}
			}
		}
	} else{
		for(i = x; i >= d_x; i--){
			if(matriz_mapa[y][i] == 0)
				return -1;
			else if(matriz_mapa[y][i] != -1){
				if(i == x || i == d_x){
					peso += matriz_mapa[y][i];
				} else{
					peso += (2*matriz_mapa[y][i]);
				}
			}
		}
	}
	return peso;
}
// Para cada vértice, chamamos a função para inserir as arestas nele
void matriz_para_grafo(int** matriz_mapa, Vertice** lista, int N, int M, int d_x, int d_y){
	int i,j, cont_indice = 0;
	for(i=0;i<N;i++){
		for(j=0;j < M;j++){
			insere_adjacentes(N, M, d_x, d_y, matriz_mapa, i, j, lista);
		}
	}
}