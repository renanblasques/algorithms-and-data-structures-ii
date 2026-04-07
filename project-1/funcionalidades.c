#include "funcionalidades.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas para funções auxiliares
#include "arquivo.h"      // Funções para manipulação de arquivos binários
#include "busca.h"        // Funções para busca de registros
#include "utilitarios.h"  // Funções fornecidas pelo run.codes
#include "registro.h"     // Funções para manipulação de registros

// Estrutura para armazenar um par de códigos de estação
typedef struct parEstacao {
    int codEstacao;
    int codProxEstacao;
} ParEstacao;

// Obs: as funções static são funções auxiliares que não devem ser acessadas pela função main

/*
    Função para imprimir a mensagem de falha no processamento do arquivo
    Entrada: não há
    Saída na tela: "Falha no processamento do arquivo."
*/
static void imprimir_falha_processamento() {
    printf("Falha no processamento do arquivo.\n");
}

/*
    Função para duplicar uma string simples
    Entrada: string a ser duplicada
    Saída: ponteiro para a string duplicada
*/
static char *duplicar_string_simples(const char *origem) {
    char *copia = (char *)malloc(strlen(origem) + 1);

    if (copia == NULL) {
        return NULL;
    }

    // Copia da string original para a string destino
    strcpy(copia, origem);
    // Retorno da string duplicada
    return copia;
}

/*
    Função para adicionar um nome de estação no vetor de nomes
    Entrada: vetor de nomes, quantidade de nomes, capacidade do vetor de nomes, nome da estação
    Saída: 1 se o nome foi adicionado com sucesso, 0 caso contrário
*/
static int adicionar_nome_unico(char ***nomes, int *quantidade, int *capacidade, const char *nome) {
    int i;
    char **novoVetor;

    for (i = 0; i < *quantidade; i++) {
        if (strcmp((*nomes)[i], nome) == 0) {
            return 1;
        }
    }

    if (*quantidade == *capacidade) {
        // Se a quantidade de nomes é igual à capacidade do vetor de nomes, aumenta a capacidade do vetor de nomes
        *capacidade = (*capacidade == 0) ? 16 : (*capacidade * 2);
        // Alocação do novo vetor de nomes
        novoVetor = (char **)realloc(*nomes, (*capacidade) * sizeof(char *));
        // Verifica se a alocação foi feita corretamente
        if (novoVetor == NULL) {
            return 0;
        }
        *nomes = novoVetor;
    }

    (*nomes)[*quantidade] = duplicar_string_simples(nome);
    if ((*nomes)[*quantidade] == NULL) {
        return 0;
    }

    (*quantidade)++;
    return 1;
}

/*
    Função para adicionar um par de códigos de estação no vetor de pares
    Entrada: vetor de pares, quantidade de pares, capacidade do vetor de pares, código da estação, código da próxima estação
    Saída: 1 se o par foi adicionado com sucesso, 0 caso contrário
*/
static int adicionar_par_unico(ParEstacao **pares, int *quantidade, int *capacidade, int codEstacao, int codProxEstacao) {
    int i;
    ParEstacao *novoVetor;

    if (codProxEstacao == -1) {
        return 1;
    }

    for (i = 0; i < *quantidade; i++) {
        if ((*pares)[i].codEstacao == codEstacao && (*pares)[i].codProxEstacao == codProxEstacao) {
            return 1;
        }
    }

    if (*quantidade == *capacidade) {
        *capacidade = (*capacidade == 0) ? 16 : (*capacidade * 2);
        novoVetor = (ParEstacao *)realloc(*pares, (*capacidade) * sizeof(ParEstacao));
        if (novoVetor == NULL) {
            return 0;
        }
        *pares = novoVetor;
    }

    (*pares)[*quantidade].codEstacao = codEstacao;
    (*pares)[*quantidade].codProxEstacao = codProxEstacao;
    (*quantidade)++;
    return 1;
}

/*
    Função para liberar o vetor de nomes
    Entrada: vetor de nomes, quantidade de nomes
    Saída: não há
*/
static void liberar_nomes(char **nomes, int quantidade) {
    int i;

    for (i = 0; i < quantidade; i++) {
        free(nomes[i]);
    }

    free(nomes);
}

/*
    Função para abrir um arquivo binário para leitura
    Entrada: nome do arquivo binário, ponteiro para arquivo, ponteiro para cabeçalho
    Saída: 1 se o arquivo foi aberto com sucesso, 0 caso contrário
*/
static int abrir_binario_para_leitura(const char *nomeArquivo, FILE **arquivo, Cabecalho *cabecalho) {
    // Abertura do arquivo binário para leitura
    *arquivo = fopen(nomeArquivo, "rb");
    // Verifica se o arquivo foi aberto corretamente
    if (*arquivo == NULL) {
        return 0;
    }

    // Lê o cabeçalho do arquivo binário e verifica se a leitura foi feita corretamente
    if (!ler_cabecalho(*arquivo, cabecalho) || cabecalho->status != '1') {
        // Fecha o arquivo caso a leitura do cabeçalho falhe e atribui NULL ao ponteiro
        fclose(*arquivo);
        *arquivo = NULL;
        return 0;
    }

    return 1;
}

/*
    Funcionalidade 1: Criar arquivo binário a partir de um arquivo CSV
    Entrada na tela: nomeCsv.csv nomeBinario.bin (exemplo: "estacoes.csv estacoes.bin")
    Processamento:
        1. Leitura do arquivo CSV
        2. Criação do arquivo binário
        3. Escrita do cabeçalho no arquivo binário
        4. Leitura da primeira linha do arquivo CSV (cabeçalho)
        5. Leitura de cada linha do arquivo CSV (dados)
            5.1. Escrita do registro no arquivo binário
            5.2. Adição do nome da estação no vetor de nomes
            5.3. Adição do par de códigos de estação no vetor de pares
        6. Verificação se a leitura do arquivo CSV é válida
        7. Escrita do cabeçalho no arquivo binário
    Saída:
        - Se o arquivo binário foi criado com sucesso, o arquivo é impresso na tela.
        - Se o arquivo binário não foi criado com sucesso, é exibida a mensagem "Falha no processamento do arquivo."
*/

void executar_funcionalidade_1() {
    // Declaração de variáveis para entrada de dados
    char nomeCsv[128];
    char nomeBinario[128];

    // Declaração de ponteiros para arquivos
    FILE *csv;
    FILE *binario;

    // Declaração dos registros
    Cabecalho cabecalho; // Registro para armazenar o cabeçalho do arquivo binário
    Dado dado;           // Registro para armazenar os dados de cada estação

    // Declaração de variáveis para processamento de dados
    char linhaCabecalho[512];   // Buffer para leitura da primeira linha do arquivo CSV (cabeçalho)
    char **nomesEstacao = NULL; // Vetor de strings para armazenar os nomes das estações
    ParEstacao *paresEstacao = NULL; // Vetor de pares de códigos de estações (código da estação e código da próxima estação)
    int quantidadeNomes = 0; // Quantidade de nomes de estações
    int quantidadePares = 0; // Quantidade de pares de códigos de estações
    int capacidadeNomes = 0; // Capacidade do vetor de nomes de estações
    int capacidadePares = 0; // Capacidade do vetor de pares de códigos de estações
    int leituraValida = 1; // Variável para verificar se a leitura do arquivo CSV é válida

    // Leitura dos nomes dos arquivos CSV e binário
    // Lê os dois nomes e verifica se a leitura foi feita corretamente
    if (scanf("%127s %127s", nomeCsv, nomeBinario) != 2) {
        imprimir_falha_processamento();
        return;
    }

    // Abertura do arquivo CSV para leitura
    csv = fopen(nomeCsv, "r");

    // Verifica se o arquivo CSV foi aberto corretamente
    if (csv == NULL) {
        imprimir_falha_processamento();
        return;
    }

    // Abertura do arquivo binário para escrita
    binario = fopen(nomeBinario, "wb+");

    // Verifica se o arquivo binário foi aberto corretamente
    if (binario == NULL) {
        // Fechamento do arquivo CSV caso a abertura do binário falhe
        fclose(csv);
        imprimir_falha_processamento();
        return;
    }

    // Inicialização do cabeçalho do arquivo binário
    inicializar_cabecalho(&cabecalho);

    // Escrita do cabeçalho do arquivo binário
    escrever_cabecalho(binario, &cabecalho);

    // Lê a primeira linha do arquivo CSV (cabeçalho) e verifica se a leitura foi feita corretamente
    if (fgets(linhaCabecalho, sizeof(linhaCabecalho), csv) == NULL) {
        // Fechamento dos arquivos CSV e binário caso a leitura da primeira linha falhe
        fclose(csv);
        fclose(binario);
        imprimir_falha_processamento();
        return;
    }

    // Lê cada linha do arquivo CSV (dados) e verifica se a leitura foi feita corretamente
    while (ler_linha_csv(csv, &dado)) {

        // Escrita do registro no arquivo binário
        escrever_registro(binario, &dado);

        // Adição do nome da estação no vetor de nomes
        // Adição do par de códigos de estação no vetor de pares
        // Verificação se a leitura do arquivo CSV é válida
        if (!adicionar_nome_unico(&nomesEstacao, &quantidadeNomes, &capacidadeNomes, dado.nomeEstacao) ||
            !adicionar_par_unico(&paresEstacao, &quantidadePares, &capacidadePares, dado.codEstacao, dado.codProxEstacao)) {
            leituraValida = 0; // Se a leitura do arquivo CSV não for válida, a variável leituraValida é setada para 0
        }

        cabecalho.proxRRN++; // Incrementação do próximo RRN do arquivo binário
        liberar_dado(&dado); // Liberação do dado

        // Verificação se a leitura do arquivo CSV é válida
        if (!leituraValida) {
            break;
        }
    }

    // Verificação se o arquivo CSV foi lido até o final
    if (!feof(csv)) {
        leituraValida = 0;
    }

    // Verificação se a leitura do arquivo CSV é válida
    if (!leituraValida) {
        // Liberação dos vetores de nomes e pares de códigos de estações
        liberar_nomes(nomesEstacao, quantidadeNomes);
        free(paresEstacao);
        // Fechamento dos arquivos CSV e binário
        fclose(csv);
        fclose(binario);
        imprimir_falha_processamento();
        return;
    }

    // Atualização do status do arquivo binário (consistente novamente)
    cabecalho.status = '1';
    // Atualização do número de estações do arquivo binário
    cabecalho.nroEstacoes = quantidadeNomes;
    // Atualização do número de pares de códigos de estações do arquivo binário
    cabecalho.nroParesEstacao = quantidadePares;

    // Posicionamento no início do arquivo binário
    fseek(binario, 0, SEEK_SET);
    // Escrita do cabeçalho do arquivo binário
    escrever_cabecalho(binario, &cabecalho);

    // Liberação dos vetores de nomes e pares de códigos de estações e do vetor de vetores de nomes
    liberar_nomes(nomesEstacao, quantidadeNomes);
    free(paresEstacao);

    // Fechamento dos arquivos CSV e binário
    fclose(csv);
    fclose(binario);

    // Impressão do arquivo binário na tela com a função fornecida no run.codes
    BinarioNaTela(nomeBinario);
}

/*
    Funcionalidade 2: Imprimir registros existentes em um arquivo binário
    Entrada na tela: nomeBinario.bin (exemplo: "estacoes.bin")
    Processamento:
        1. Leitura do nome do arquivo binário
        2. Abertura do arquivo binário para leitura
        3. Leitura de cada registro do arquivo binário
        4. Verificação se o registro está ativo
        5. Impressão do registro se estiver ativo
    Saída:
        - Se o registro foi encontrado, o registro é impresso na tela.
        - Se o registro não foi encontrado, a mensagem "Registro inexistente." é exibida na tela.
*/
void executar_funcionalidade_2() {
    // Declaração de variável para entrada de dados
    char nomeBinario[128];

    // Declaração de ponteiro para arquivo binário
    FILE *binario;

    // Declaração dos registros de cabeçalho e de dados
    Cabecalho cabecalho;
    Dado dado;

    // Contador para percorrer os registros do arquivo binário
    int i;
    // Variável para verificar se o registro foi encontrado
    int encontrou = 0;

    // Lê o nome do arquivo binário e verifica se a leitura foi feita corretamente
    if (scanf("%127s", nomeBinario) != 1) {
        imprimir_falha_processamento();
        return;
    }

    // Abre o arquivo binário para leitura e verifica se a abertura foi feita corretamente
    if (!abrir_binario_para_leitura(nomeBinario, &binario, &cabecalho)) {
        imprimir_falha_processamento();
        return;
    }

    // Percorre todos os registros do arquivo binário e verifica se a leitura foi feita corretamente
    for (i = 0; i < cabecalho.proxRRN; i++) {
        // Lê o registro e verifica se a leitura foi feita corretamente
        if (!ler_registro(binario, &dado)) {
            // Se a leitura do registro falhar, o loop é interrompido
            break;
        }

        // Verifica se o registro está ativo e se estiver, imprime o registro
        if (registro_ativo(&dado)) {
            // Impressão do registro
            // Imprime o registro se estiver ativo
            imprimir_registro(&dado);
            encontrou = 1;
        }

        // Libera o registro para evitar vazamento de memória
        liberar_dado(&dado);
    }

    // Se o registro não foi encontrado, imprime a mensagem "Registro inexistente."
    if (!encontrou) {
        printf("Registro inexistente.\n");
    }

    // Fechamento do arquivo binário
    fclose(binario);
}

/*
    Funcionalidade 3: Buscar registros que satisfazem os filtros em um arquivo binário
    Entrada na tela: nomeBinario.bin quantidadeBuscas (exemplo: "estacoes.bin 2")
    Processamento:
        1. Leitura do nome do arquivo binário e da quantidade de buscas
        2. Abertura do arquivo binário para leitura
        3. Leitura de cada busca
        4. Leitura de cada filtro
        5. Verificação se o registro satisfaz os filtros
        6. Impressão do registro se satisfaz os filtros
        7. Liberação do registro
        8. Fechamento do arquivo binário
    Saída:
        - Se o registro foi encontrado, o registro é impresso na tela.
        - Se o registro não foi encontrado, a mensagem "Registro inexistente" é exibida na tela.
        - Se houve algum erro, a mensagem "Falha no processamento do arquivo" é exibida na tela.
*/
void executar_funcionalidade_3() {
    // Declaração de variável para entrada de dados
    char nomeBinario[128];

    // Declaração de ponteiro para arquivo binário
    FILE *binario;

    // Declaração dos registros de cabeçalho e de dados
    Cabecalho cabecalho;

    // Declaração de variável para processamento de dados
    int quantidadeBuscas;
    int buscaAtual;

    // Lê o nome do arquivo binário e a quantidade de buscas e verifica se a leitura foi feita corretamente
    if (scanf("%127s %d", nomeBinario, &quantidadeBuscas) != 2) {
        imprimir_falha_processamento();
        return;
    }

    // Abre o arquivo binário para leitura e verifica se a abertura foi feita corretamente
    if (!abrir_binario_para_leitura(nomeBinario, &binario, &cabecalho)) {
        imprimir_falha_processamento();
        return;
    }

    // Percorre todas as buscas e verifica se a leitura foi feita corretamente
    for (buscaAtual = 0; buscaAtual < quantidadeBuscas; buscaAtual++) {
        // Declaração de variável para processamento de dados
        int quantidadeFiltros;
        FiltroBusca *filtros;
        int i; // Contador para percorrer os filtros
        int rrn; // Contador para percorrer os registros
        int encontrou = 0; // Variável para verificar se o registro foi encontrado

        // Lê a quantidade de filtros e verifica se a leitura foi feita corretamente
        if (scanf("%d", &quantidadeFiltros) != 1) {
            fclose(binario);
            imprimir_falha_processamento();
            return;
        }

        // Aloca o vetor de filtros e verifica se a alocação foi feita corretamente
        filtros = (FiltroBusca *)malloc(quantidadeFiltros * sizeof(FiltroBusca));
        if (filtros == NULL) {
            fclose(binario);
            imprimir_falha_processamento();
            return;
        }

        // Percorre todos os filtros e verifica se a leitura foi feita corretamente
        for (i = 0; i < quantidadeFiltros; i++) {
            if (!ler_filtro(&filtros[i])) {
                // Liberação do vetor de filtros e fechamento do arquivo binário
                free(filtros);
                fclose(binario);
                imprimir_falha_processamento();
                return;
            }
        }

        // Posicionamento no início do arquivo binário
        fseek(binario, 17, SEEK_SET); // 17 é o tamanho do cabeçalho do arquivo binário

        // Percorre todos os registros do arquivo binário e verifica se a leitura foi feita corretamente
        for (rrn = 0; rrn < cabecalho.proxRRN; rrn++) {
            Dado dado;

            // Lê o registro e verifica se a leitura foi feita corretamente
            if (!ler_registro(binario, &dado)) {
                break;
            }

            // Verificação se o registro está ativo e se satisfaz os filtros
            if (registro_ativo(&dado) && registro_satisfaz_filtros(&dado, quantidadeFiltros, filtros)) {
                imprimir_registro(&dado);
                encontrou = 1;
            }

            // Liberação do registro de dados
            liberar_dado(&dado);
        }

        // Se o registro não foi encontrado, imprime a mensagem "Registro inexistente."
        if (!encontrou) {
            printf("Registro inexistente.\n");
        }

        // Liberação do vetor de filtros
        free(filtros);

        // Adiciona uma quebra de linha no final da busca
        if (buscaAtual < quantidadeBuscas - 1) {
            printf("\n");
        }
    }

    // Fechamento do arquivo binário
    fclose(binario);
}

/*
    Funcionalidade 4: Imprimir um registro específico em um arquivo binário
    Entrada na tela: nomeBinario.bin RRN (exemplo: "estacoes.bin 0")
    Processamento:
        1. Leitura do nome do arquivo binário e do RRN
        2. Abertura do arquivo binário para leitura
        3. Verificação se o RRN é válido
        4. Verificação se o registro está ativo
        5. Impressão do registro se estiver ativo
        6. Liberação do registro
        7. Fechamento do arquivo binário
    Saída:
        - Se o registro foi encontrado, o registro é impresso na tela.
        - Se o registro não foi encontrado, a mensagem "Registro inexistente." é exibida na tela.
*/
void executar_funcionalidade_4() {
    // Declaração de variável para entrada de dados
    char nomeBinario[128];

    // Declaração de ponteiro para arquivo binário
    FILE *binario;

    // Declaração dos registros de cabeçalho e de dados
    Cabecalho cabecalho;
    Dado dado;

    // Declaração de variável para processamento de dados
    int rrn;

    // Lê o nome do arquivo binário e o RRN e verifica se a leitura foi feita corretamente
    if (scanf("%127s %d", nomeBinario, &rrn) != 2) {
        imprimir_falha_processamento();
        return;
    }

    // Abre o arquivo binário para leitura e verifica se a abertura foi feita corretamente
    if (!abrir_binario_para_leitura(nomeBinario, &binario, &cabecalho)) {
        imprimir_falha_processamento();
        return;
    }

    // Verifica se o RRN é válido e se não é, imprime a mensagem "Registro inexistente."
    if (rrn < 0 || rrn >= cabecalho.proxRRN || !posicionar_no_registro(binario, rrn) || !ler_registro(binario, &dado)) {
        fclose(binario);
        printf("Registro inexistente.\n");
        return;
    }

    // Verifica se o registro está ativo e se não é, imprime a mensagem "Registro inexistente."
    if (!registro_ativo(&dado)) {

        // Liberação do registro e fechamento do arquivo binário
        liberar_dado(&dado);
        fclose(binario);
        printf("Registro inexistente.\n");
        return;
    }

    // Imprime o registro existente, libera o registro e fecha o arquivo binário
    imprimir_registro(&dado);
    liberar_dado(&dado);
    fclose(binario);
}
