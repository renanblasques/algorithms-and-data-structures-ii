#ifndef BUSCA_H
#define BUSCA_H

#include "tipos.h"

int ler_filtro(FiltroBusca *filtro);
int registro_satisfaz_filtros(Dado *dado, int quantidadeFiltros, FiltroBusca *filtros);

#endif
