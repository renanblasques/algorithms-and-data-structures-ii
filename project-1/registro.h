#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>

#include "tipos.h"

void inicializar_dado(Dado *dado);
void liberar_dado(Dado *dado);
int ler_linha_csv(FILE *csv, Dado *dado);
void escrever_registro(FILE *arquivo, Dado *dado);
int ler_registro(FILE *arquivo, Dado *dado);
void imprimir_registro(Dado *dado);
int registro_ativo(Dado *dado);

#endif
