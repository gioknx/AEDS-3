typedef struct{
	int L;
	int H;
	int expoente;
	int k;
	char **texto;
	int qtdPalavras;
	int **memo;
	int *pai;
}Dados;

typedef struct{
	int qtdLinhas;
	int *qtdPalavrasPorLinha;
	int *tamanhoLinhas;
}Linhas;


int words(int size, char* string);
void copiaTexto(char **texto, char* string);
int tamanhoLinha(Dados *data, int i, int j);