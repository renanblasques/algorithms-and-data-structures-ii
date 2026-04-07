#ifndef TIPOS_H
#define TIPOS_H

#define TIPO_FILTRO_INTEIRO 1
#define TIPO_FILTRO_STRING 2

typedef struct cabecalho {
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacao;
} Cabecalho;

typedef struct dado {
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char *nomeEstacao;
    int tamNomeLinha;
    char *nomeLinha;
} Dado;

typedef struct filtroBusca {
    char campo[32];
    int tipo;
    int valorInteiro;
    char valorString[128];
} FiltroBusca;

#endif
