
typedef struct mapa{
	int N;
	int M;
	int** matriz_mapa;
	int* lista_atalhos;
	int tamanho_lista_atalhos;
}Mapa;

// Essa função irá conectar todos os atalhos com o custo 0 entre eles
void conecta_atalhos(Grafo *grafo, Mapa *map);
// Lemos a entrada do arquivo e colocamos em um mapa de pesos 
// que será posteriormente convertido para um grafo
Mapa* ler_entrada(FILE *fp);
// Printa todos os indices
// Função para auxiliar na leitura do grafo quando necessário
void print_indices(int N, int M);
// Retorna o minimo entre dois numeros
int min(int a, int b);
// Dado um vértice, precisamos inserir as arestas para os possiveis caminhamentos dele
// levando em consideração a restrição de movimento d_x e d_y.
void insere_adjacentes(int N, int M, int d_x, int d_y, int** matriz_mapa, int i, int j, Vertice** lista);
// Faz um caminhamento no eixo Y a partir de uma posiçao e retorna o peso desse caminho
// ou -1 caso tenha um obstáculo
int valida_caminho_pelo_y(int **matriz_mapa, int x, int y, int d_y);
// Faz um caminhamento no eixo X a partir de uma posiçao e retorna o peso desse caminho
// ou -1 caso tenha um obstáculo
int valida_caminho_pelo_x(int **matriz_mapa, int x, int y, int d_x);
// Para cada vértice, chamamos a função para inserir as arestas nele
void matriz_para_grafo(int** matriz_mapa, Vertice** lista, int N, int M, int d_x, int d_y);
// Desaloca memoria do mapa
void libera_mapa(Mapa* mapa);