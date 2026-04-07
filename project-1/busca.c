#include "busca.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilitarios.h"

/*
    Função para verificar se um campo é uma string
    Entrada: ponteiro para o campo
    Saída: 1 se o campo é uma string, 0 caso contrário
*/
static int campo_eh_string(const char *campo) {
    return strcmp(campo, "nomeEstacao") == 0 || strcmp(campo, "nomeLinha") == 0;
}

/*
    Função para comparar um inteiro
    Entrada: ponteiro para o dado, ponteiro para o filtro
    Saída: 1 se o inteiro é igual, 0 caso contrário
*/
static int comparar_inteiro(Dado *dado, FiltroBusca *filtro) {
    // Compara cada campo do dado com o filtro

    if (strcmp(filtro->campo, "codEstacao") == 0) {
        return dado->codEstacao == filtro->valorInteiro;
    }

    if (strcmp(filtro->campo, "codLinha") == 0) {
        return dado->codLinha == filtro->valorInteiro;
    }

    if (strcmp(filtro->campo, "codProxEstacao") == 0) {
        return dado->codProxEstacao == filtro->valorInteiro;
    }

    if (strcmp(filtro->campo, "distProxEstacao") == 0) {
        return dado->distProxEstacao == filtro->valorInteiro;
    }

    if (strcmp(filtro->campo, "codLinhaIntegra") == 0) {
        return dado->codLinhaIntegra == filtro->valorInteiro;
    }

    if (strcmp(filtro->campo, "codEstIntegra") == 0) {
        return dado->codEstIntegra == filtro->valorInteiro;
    }

    return 0;
}

/*
    Função para comparar uma string
    Entrada: ponteiro para o dado, ponteiro para o filtro
    Saída: 1 se a string é igual, 0 caso contrário
*/
static int comparar_string(Dado *dado, FiltroBusca *filtro) {
    // Compara cada campo do dado com o filtro

    // Compara o campo nomeEstacao do dado com o filtro
    if (strcmp(filtro->campo, "nomeEstacao") == 0) {
        if (filtro->valorString[0] == '\0') {
            return dado->tamNomeEstacao == 0;
        }

        return strcmp(dado->nomeEstacao, filtro->valorString) == 0;
    }

    // Compara o campo nomeLinha do dado com o filtro
    if (strcmp(filtro->campo, "nomeLinha") == 0) {
        if (filtro->valorString[0] == '\0') {
            return dado->tamNomeLinha == 0;
        }

        return strcmp(dado->nomeLinha, filtro->valorString) == 0;
    }

    return 0;
}

/*
    Função para ler um filtro
    Entrada: ponteiro para o filtro
    Saída: 1 se o filtro foi lido com sucesso, 0 caso contrário
*/
int ler_filtro(FiltroBusca *filtro) {
    // Leitura do campo do filtro
    char valorLido[128];

    // Leitura do campo do filtro e verifica se a leitura foi feita corretamente
    if (scanf("%31s", filtro->campo) != 1) {
        return 0;
    }

    // Verifica se o campo é uma string
    if (campo_eh_string(filtro->campo)) {
        filtro->tipo = TIPO_FILTRO_STRING;

        // Leitura do valor do filtro e verifica se a leitura foi feita corretamente
        // Fornecido no run.codes
        ScanQuoteString(filtro->valorString);
        return 1;
    }

    // Se o campo não é uma string, então é um inteiro
    filtro->tipo = TIPO_FILTRO_INTEIRO;

    // Leitura do valor do filtro e verifica se a leitura foi feita corretamente
    if (scanf("%127s", valorLido) != 1) {
        return 0;
    }

    // Verifica se o valor é NULO
    if (strcmp(valorLido, "NULO") == 0) {   
        // Se o valor é NULO, então o valor do filtro é -1
        filtro->valorInteiro = -1;
    } else {
        // Se o valor não é NULO, então o valor do filtro é o valorlido convertido para inteiro
        filtro->valorInteiro = atoi(valorLido);
    }

    // Inicialização do valor da string do filtro
    filtro->valorString[0] = '\0';
    return 1;
}

/*
    Função para verificar se um registro satisfaz os filtros
    Entrada: ponteiro para o dado, quantidade de filtros, ponteiro para os filtros
    Saída: 1 se o registro satisfaz os filtros, 0 caso contrário
*/
int registro_satisfaz_filtros(Dado *dado, int quantidadeFiltros, FiltroBusca *filtros) {
    // Declaração de variável para percorrer os filtros
    int i;

    // Percorre todos os filtros
    for (i = 0; i < quantidadeFiltros; i++) {
        // Verifica se o filtro é uma string
        if (filtros[i].tipo == TIPO_FILTRO_STRING) {
            // Verifica se o filtro é uma string e se não é, retorna 0
            if (!comparar_string(dado, &filtros[i])) {
                return 0;
            }
        } else if (!comparar_inteiro(dado, &filtros[i])) {
            // Verifica se o filtro é um inteiro e se não é, retorna 0
            return 0;
        }
    }

    return 1;
}
