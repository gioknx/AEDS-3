# e possivel definir variaveis, como o compilador e flags de compilacao
# o flag -g serve para usar o gdb e o valgrind, e o flag -O0 impede que 
# o compilador otimize o codigo
CC = gcc
CFLAGS = O3

# a primeira regra sera executada, e nesse caso, ela depende das regras 
# multiplicador e somador, que vao gerar um executavel cada.
# as operacoes foram divididas em executaveis diferentes apenas para 
# exemplificar um makefile com dois executaveis
all: arena

# a regra multiplicador sera chamada a partir da all, que por sua vez 
# depende das regras multiplicador.o e operacoes.o
# veja que a indentacao e feita exclusivamente com <tab>
arena: 
	$(CC) $(CFLAGS) main.c TAD_GRAFO.c TAD_HEAP.c TAD_MAPA.c -o tp2.exe
clean:
	rm *.exe
