#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"
#include "fila.h"
#include "disco.h"



void pa(nodo* node){
	int i,j;
	printf("                  Numero registros: %d \n                  Chaves:  ",node->numeroRegistros);
	for(i=1;i<=node->numeroRegistros;i++){
		printf("%llu ", node->chaves[i]);
	}
	printf("\n");
}


void desaloca_registro(Registro* reg, int numeroCampos){
	int j;
	if(reg->campos){
		for(j = 0;j < numeroCampos;j++){
			if(reg->campos[j])
				free(reg->campos[j]);
		}
	}
	free(reg->campos);
	free(reg);
}


void desaloca_nodo(nodo* no, int numeroCampos){
	int i, j;

	// Se o nó é uma folha, ele possui registros 
	if(no->folha){
		for(i = 1;i <= no->numeroRegistros;i++){
		// Para cada campo
			if(no->registros[i]){
				desaloca_registro(no->registros[i], numeroCampos);
			}
		}
	free(no->registros);

	}


	free(no->chaves);
	free(no->offsetFilhos);
	free(no);
}


nodo* aloca_nodo(FILE* fp, int ordem, int numeroCampos, int folha){
	nodo* novo_nodo = calloc(1, sizeof(nodo));

	novo_nodo->offsetFilhos = calloc((ordem+1+1), sizeof(long long unsigned int));
	novo_nodo->chaves =(long long unsigned*) calloc((ordem+1), sizeof(long long unsigned));
	novo_nodo->ordem = ordem;
	novo_nodo->prox = 0;

	novo_nodo->folha = folha;
	if(folha)
		novo_nodo->registros = calloc(ordem+1, sizeof(Registro*));


	fseek(fp, 0, SEEK_END);
	long long unsigned len = ftell(fp);
	novo_nodo->offset = len;
	serializar(fp, novo_nodo, numeroCampos, len);
	return novo_nodo;
}




nodo* cria_arvore(FILE* fp, int ordem, int numeroCampos){
	nodo *raiz = aloca_nodo(fp, ordem, numeroCampos, 1);
	raiz->numeroRegistros = 0;
	return raiz;
}

void separa_folha(nodo *y, nodo *z, int ordem){
	unsigned long long int j;
	int tamanho_y = (ordem/2);
	int tamanho_z = ordem - (ordem/2);

	z->numeroRegistros = tamanho_z;
	y->numeroRegistros = tamanho_y;

	y->prox = z->offset;


	// Movemos as chaves e registros apropriadas para o novo nó
	for(j = 1; j <= tamanho_z;j++){
		z->chaves[j] = y->chaves[tamanho_y+j];
		z->registros[j] = y->registros[tamanho_y+j];
	}
}
void separa_interno(nodo *y, nodo *z, int ordem){
	unsigned long long int j;


	int tamanho_y = (ordem/2);
	// -1 pois uma das chaves irá ser elevada
	int tamanho_z = ordem - (ordem/2) - 1;

	z->numeroRegistros = tamanho_z;
	y->numeroRegistros = tamanho_y;

	// Movemos os apontadores certos para o novo nó
	for(j = 1; j <= tamanho_z+1;j++){
		z->offsetFilhos[j] = y->offsetFilhos[j+tamanho_y+1];

	}	

	// Movemos as chaves e registros apropriadas para o novo nó
	// Teremos o +1 para pular a chave que será elevada
	for(j = 1; j <= tamanho_z;j++){
		z->chaves[j] = y->chaves[tamanho_y+j+1];
	}
}

void separa_filho(FILE* fp, nodo *x, int i, nodo *y, int numeroCampos){
	unsigned long long int j;
	int ordem = x->ordem;

	nodo* z = aloca_nodo(fp, ordem, numeroCampos, y->folha);

	// Copiamos os apontadores de Y para Z
	if(y->folha)
		separa_folha(y,z,ordem);
	else
		separa_interno(y,z,ordem);

 	// Agoramos que já ajustamos os dois nós irmãos, ajustaremos o pai

	// Movemos os apontadores do nodo pai
	// J começa no apontador final
	// J decresce até a posição imediatamente depois do apontador que apontava
	// para o filho que está sendo dividido
	for(j = x->numeroRegistros+1; j >= i+1;j--){
		x->offsetFilhos[j+1] = x->offsetFilhos[j];
	}
	x->offsetFilhos[i+1] = z->offset;


	//Deslocamos as chaves para a direita, inserindo a chave que subiu da divisão
	for(j = x->numeroRegistros; j >= i ;j--){
		x->chaves[j+1] = x->chaves[j];
	}
	x->chaves[i] = y->chaves[(ordem/2)+1];		

		// Atualizamos o numero de registros do pai
	x->numeroRegistros++;


	serializar(fp, x, numeroCampos, x->offset);
	serializar(fp, y, numeroCampos, y->offset);
	serializar(fp, z, numeroCampos, z->offset);

	desaloca_nodo(z, numeroCampos);


}


void insere_nodo_com_espaco(nodo* x, FILE* fp, Registro *reg, int is_raiz){
	// ORDEM SET
	int ordem = x->ordem;

	int i = x->numeroRegistros;
	if(x->folha){
		while(i >= 1 && reg->chave < x->chaves[i]){
			x->chaves[i+1] = x->chaves[i];
			x->registros[i+1] = x->registros[i];
			i--;	
		}
		x->chaves[i+1] = reg->chave;
		x->registros[i+1] = reg;

		x->numeroRegistros++;


		// Após inserir, atualizamos o nó contendo o registro e desalocamos
		serializar(fp, x, x->registros[i+1]->numeroCampos, x->offset);
		// Desalocamos o nó, caso ele não seja uma raiz
		if(!is_raiz)
			desaloca_nodo(x, reg->numeroCampos);

	}else{
		while(i >= 1 && reg->chave < x->chaves[i]){
			i--;
		}
		i++;

		// Carregamos o filho para comparar as chaves
		nodo* no = calloc(1, sizeof(nodo));
		desserializar(fp, x->offsetFilhos[i], no);

		// Caso o no filho esteja cheio, vamos dividi-lo
		if(no->numeroRegistros == ordem){
			separa_filho(fp, x,i, no , reg->numeroCampos);
		// Checamos se vamos inserir no da esquerda ou direita
			if(reg->chave > x->chaves[i])
				i++;
		}

		// Desalocamos o filho, pois carregaremos o correto agora
		desaloca_nodo(no, reg->numeroCampos);

		// Carregamos o filho correto
		no = calloc(1, sizeof(nodo));
		desserializar(fp, x->offsetFilhos[i], no);

		// Não precisaremos mais do pai, então o desalocaremos
		// Desalocamos o nó, caso ele não seja uma raiz
		if(!is_raiz)
			desaloca_nodo(x, reg->numeroCampos);


		insere_nodo_com_espaco(no,fp,reg, 0);
	}
}

nodo *inserir(nodo* raiz, FILE* fp, Registro *reg){
	// ORDEM SET
	int ordem = raiz->ordem, numeroCampos;
	long offset_raiz;
	nodo * r = raiz;
	// Caso a raiz esteja cheia, vamos quebrá-la
	if(r->numeroRegistros == (ordem)){
		nodo *s = aloca_nodo(fp, ordem, reg->numeroCampos, 0);
		raiz = s;
		s->numeroRegistros = 0;
		separa_filho(fp,s,1,r, reg->numeroCampos);
		s->offsetFilhos[1] = r->offset;
		
		// Desalocamos a antiga raiz, pois já efetuamos a divisão e serializamos ela
		desaloca_nodo(r, reg->numeroCampos);

		// Salvamos a nova raiz e efetuamos a operação
		insere_nodo_com_espaco(s,fp,reg, 1);

	}else{
		insere_nodo_com_espaco(r,fp,reg, 1);
	}

	return raiz;
}

// Imprime a arvore, começando da raiz, nível a nível
nodo* wide_search(nodo* raiz, FILE* fpwriter, FILE* fparvore, int numeroCampos){
	int tamanhoMaximo = 30;

	Fila *f = CriaFila(tamanhoMaximo);
	ElementoFila* ef = (ElementoFila*) malloc(sizeof(ElementoFila));
	ElementoFila* eff;
	ef->nodo = raiz;
	Enfileira(f, ef);

	int i;
	while(f->tamanho != 0) {
		ElementoFila* n = Desenfileira(f);

		for(i = 1; i<=n->nodo->numeroRegistros+1;i++){
			if(!n->nodo->folha /*&& n->nodo->offsetFilhos[1] != 0*/){
			eff = (ElementoFila*) malloc(sizeof(ElementoFila));
			eff->nodo = (nodo*)malloc(sizeof(nodo));
			desserializar(fparvore, n->nodo->offsetFilhos[i], eff->nodo);
			Enfileira(f,eff);
			}
		}

		for(i = 1; i<=n->nodo->numeroRegistros;i++){
			printf("%llu,", n->nodo->chaves[i]);
			fprintf(fpwriter, "%llu,", n->nodo->chaves[i]);
		}

		if(i>1){
			printf("folha: %d,", n->nodo->folha);
			fprintf(fpwriter, "\n");
			printf("\n");
		}

		if(f->tamanhoMaximo == 30)
			f->tamanhoMaximo = f->tamanhoMaximo*2;

			// Desalocamos os nós durante o processo, pois só podemos ter a raiz na memória
		desaloca_nodo(n->nodo, numeroCampos);
		free(n);

	}
	free(f);
	printf("\n");
}



// Usaremos essas funções para nos auxiliar na hora de desalocar memória
// Desta forma, não precisamos nos preocupar em perder a referencia para a raiz
// Tudo que precisamos manter entre elas é a offset de onde foi salvo a raiz
void salva_raiz(nodo* raiz, FILE* fp, long numeroCampos){
	serializar(fp, raiz, numeroCampos, raiz->offset);
}

nodo* carrega_raiz(long long unsigned offset, FILE* fp){
	nodo* nova_raiz = calloc(1, sizeof(nodo));
	desserializar(fp, offset, nova_raiz);
	return nova_raiz;
}

	// Usamos esta função para clonarmos um registro para outra variavel
	// Podendo assim desalocar o nó a qual este registro pertence sem perder sua referencia
Registro *clona_registro(Registro* reg_source){
	Registro *reg_dest = calloc(1, sizeof(Registro));
	reg_dest->chave = reg_source->chave;
	reg_dest->numeroCampos = reg_source->numeroCampos;
	reg_dest->campos = calloc(reg_dest->numeroCampos, sizeof(char*));
	int i;
	for(i=0;i<reg_dest->numeroCampos;i++){
		reg_dest->campos[i] = calloc(30, sizeof(char));
		strcpy(reg_dest->campos[i],reg_source->campos[i]);
	}

	return reg_dest;

}

Registro* busca(FILE* fp, nodo* x, int k, int numeroCampos){
	Registro* registro_encontrado;
	int i = 1;
	while(i <= x->numeroRegistros && k >= x->chaves[i])
		i++;
	if(x->folha){
		if(k == x->chaves[i-1]){
			registro_encontrado = clona_registro(x->registros[i-1]);
			desaloca_nodo(x, numeroCampos);
			return registro_encontrado;
		}else{
			desaloca_nodo(x, numeroCampos);
			return NULL;
		}
	}

	nodo* no = calloc(1, sizeof(nodo));
	// Desserializamos o filho para buscarmos nele
	desserializar(fp, x->offsetFilhos[i], no);
	// Desalocamos o pai
	desaloca_nodo(x, numeroCampos);

	return busca(fp, no, k, numeroCampos);
}
