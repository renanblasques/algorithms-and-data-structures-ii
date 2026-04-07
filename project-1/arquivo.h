#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

#include "tipos.h"

void inicializar_cabecalho(Cabecalho *cabecalho);
int ler_cabecalho(FILE *arquivo, Cabecalho *cabecalho);
void escrever_cabecalho(FILE *arquivo, Cabecalho *cabecalho);
int calcular_offset_registro(int rrn);
int posicionar_no_registro(FILE *arquivo, int rrn);

#endif
