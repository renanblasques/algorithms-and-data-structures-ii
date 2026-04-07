#include "arquivo.h"

/*
    Função para inicializar o cabeçalho do arquivo binário
    Entrada: ponteiro para o cabeçalho
    Saída: não há
*/
void inicializar_cabecalho(Cabecalho *cabecalho) {
    // Inicialização do cabeçalho do arquivo binário
    cabecalho->status = '0';        // Status do arquivo binário (inicializa com 0 e depois atualiza para 1)
    cabecalho->topo = -1;           // Topo do arquivo binário (inicializa com -1 pois não há topo)
    cabecalho->proxRRN = 0;         // Próximo RRN do arquivo binário (inicializa com 0 pois é o primeiro RRN)
    cabecalho->nroEstacoes = 0;     // Número de estações do arquivo binário (inicializa com 0 pois não há estações)
    cabecalho->nroParesEstacao = 0; // Número de pares de estações do arquivo binário (inicializa com 0 pois não há pares de estações)
}

/*
    Função para ler o cabeçalho do arquivo binário
    Entrada: ponteiro para o arquivo, ponteiro para o cabeçalho
    Saída: 1 se o cabeçalho foi lido com sucesso, 0 caso contrário
*/
int ler_cabecalho(FILE *arquivo, Cabecalho *cabecalho) {
    // Lê cada campo do cabeçalho e verifica se a leitura foi feita corretamente
    if (fread(&cabecalho->status, sizeof(char), 1, arquivo) != 1) {
        return 0;
    }

    if (fread(&cabecalho->topo, sizeof(int), 1, arquivo) != 1) {
        return 0;
    }

    if (fread(&cabecalho->proxRRN, sizeof(int), 1, arquivo) != 1) {
        return 0;
    }

    if (fread(&cabecalho->nroEstacoes, sizeof(int), 1, arquivo) != 1) {
        return 0;
    }

    if (fread(&cabecalho->nroParesEstacao, sizeof(int), 1, arquivo) != 1) {
        return 0;
    }

    return 1;
}

/*
    Função para escrever o cabeçalho do arquivo binário
    Entrada: ponteiro para o arquivo, ponteiro para o cabeçalho
    Saída: não há
*/
void escrever_cabecalho(FILE *arquivo, Cabecalho *cabecalho) {
    // Escrita de cada campo do cabeçalho
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nroEstacoes, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nroParesEstacao, sizeof(int), 1, arquivo);
}

/*
    Função para calcular o offset do registro no arquivo binário
    Entrada: número do registro
    Saída: offset do registro
*/
int calcular_offset_registro(int rrn) {
    // Cálculo do offset do registro no arquivo binário
    return TAM_CABECALHO + (rrn * TAM_REGISTRO);
}

/*
    Função para posicionar no registro no arquivo binário
    Entrada: ponteiro para o arquivo, número do registro
    Saída: 1 se o registro foi posicionado com sucesso, 0 caso contrário
*/
int posicionar_no_registro(FILE *arquivo, int rrn) {
    // Posiciona no registro no arquivo binário e verifica se a posição foi feita corretamente
    if (fseek(arquivo, calcular_offset_registro(rrn), SEEK_SET) != 0) {
        return 0;
    }

    return 1;
}
