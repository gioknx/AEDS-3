#ifndef KMP_H
#define KMP_H

void prefixo(unsigned char *p, int m, int *pi);



int kmp(unsigned char *t, int n, unsigned char *p, int m, int* posicoes, int* byteFlags);

#endif