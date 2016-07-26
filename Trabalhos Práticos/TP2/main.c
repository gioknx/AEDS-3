#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "TAD_GRAFO.h"
#include "TAD_HEAP.h"
#include "TAD_MAPA.h"


int main(int argc, char **argv) {

	int s_x, s_y, f_x, f_y, d_x, d_y;
	char *arquivo_entrada;
	if(argc != 8)
		return 1;
	arquivo_entrada = argv[1];
	s_x = atoi(argv[2]);
	s_y = atoi(argv[3]);
	f_x = atoi(argv[4]);
	f_y = atoi(argv[5]);
	d_x = atoi(argv[6]);
	d_y = atoi(argv[7]);	

	FILE* fp;
	fp = fopen (arquivo_entrada, "r");

	Mapa *map = ler_entrada(fp);
	Grafo *grafo = calloc(1, sizeof(Grafo));
	grafo->lista = calloc(map->N * map->M, sizeof(Vertice*));
	grafo->tamanho = map->N*map->M;

	aloca_lista(grafo);

	int indice_inicial = (s_y * map->M) + s_x;
	int indice_final = (f_y * map->M) + f_x;

	// Uma vez tendo nossa matriz de valores, devemos transformá-la no grafo
	matriz_para_grafo(map->matriz_mapa, grafo->lista, map->N, map->M, d_x, d_y );
	// Conectamos os atalhos
	conecta_atalhos(grafo, map);


	// Rodamos Djkistra a partir do nosso vertice inicial
	Djkistra(grafo, indice_inicial);

	int resultado;
	resultado = grafo->lista[indice_final]->d;
	// Se o resultado é o número máximo, significa que não existe caminho
	// logo printamos -1
	if(resultado ==  99999999)
		resultado = -1;
	printf("%d\n", resultado);

	libera_grafo(grafo);
	libera_mapa(map);
	fclose(fp);

	return 0;
}
