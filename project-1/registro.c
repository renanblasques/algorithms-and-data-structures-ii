#include "registro.h"

#include <stdlib.h>
#include <string.h>

/*
    Função para definir uma string
    Entrada: ponteiro para a string destino, ponteiro para o tamanho da string, string original
    Saída: não há
*/
static void definir_string(char **destino, int *tamanho, char *origem) {
    // Cálculo do tamanho da string original
    *tamanho = strlen(origem);
    // Alocação da string destino
    *destino = (char *)malloc(*tamanho + 1);
    // Verifica se a alocação foi feita corretamente
    if (*destino == NULL) {
        // Se a alocação falhar, o tamanho da string destino é setado para 0
        *tamanho = 0;
        return;
    }

    // Cópia da string original para a string destino
    strcpy(*destino, origem);
}

/*
    Função para verificar se um campo está vazio
    Entrada: ponteiro para o campo
    Saída: 1 se o campo está vazio, 0 caso contrário
*/
static int campo_vazio(char *campo) {
    // Verifica se o campo é nulo ou se o primeiro caractere é o caractere nulo
    if (campo == NULL || campo[0] == '\0') {
        return 1;
    }

    return 0;
}

/*
    Função para preencher um arquivo com cifrao
    Entrada: ponteiro para o arquivo, quantidade de cifras
    Saída: não há
*/
static void preencher_com_cifrao(FILE *arquivo, int quantidade) {
    char lixo = '$';
    int i;

    for (i = 0; i < quantidade; i++) {
        fwrite(&lixo, sizeof(char), 1, arquivo);
    }
}

/*
    Função para imprimir um inteiro
    Entrada: valor inteiro
    Saída: não há
*/
static void imprimir_inteiro(int valor) {
    if (valor == -1) {
        printf("NULO");
    } else {
        printf("%d", valor);
    }
}

/*
    Função para imprimir uma string
    Entrada: ponteiro para a string, tamanho da string
    Saída: não há
*/
static void imprimir_string(const char *string, int tamanho) {
    if (tamanho == 0) {
        printf("NULO");
    } else {
        printf("%s", string);
    }
}

/*
    Função para inicializar um dado
    Entrada: ponteiro para o dado
    Saída: não há
*/
void inicializar_dado(Dado *dado) {
    dado->removido = '0';
    dado->proximo = -1;
    dado->codEstacao = -1;
    dado->codLinha = -1;
    dado->codProxEstacao = -1;
    dado->distProxEstacao = -1;
    dado->codLinhaIntegra = -1;
    dado->codEstIntegra = -1;
    dado->tamNomeEstacao = 0;
    dado->nomeEstacao = NULL;
    dado->tamNomeLinha = 0;
    dado->nomeLinha = NULL;
}

/*
    Função para liberar um dado
    Entrada: ponteiro para o dado
    Saída: não há
*/
void liberar_dado(Dado *dado) {
    if (dado->nomeEstacao != NULL) {
        free(dado->nomeEstacao);
    }

    if (dado->nomeLinha != NULL) {
        free(dado->nomeLinha);
    }

    dado->nomeEstacao = NULL;
    dado->nomeLinha = NULL;
    dado->tamNomeEstacao = 0;
    dado->tamNomeLinha = 0;
}

/*
    Função para ler uma linha do arquivo CSV
    Entrada: ponteiro para o arquivo CSV, ponteiro para o dado
    Saída: 1 se a linha foi lida com sucesso, 0 caso contrário
*/
int ler_linha_csv(FILE *csv, Dado *dado) {
    char linha[512];
    char *campos[8];
    char *inicio;
    char *ponteiroLinha;
    int indice;

    if (fgets(linha, sizeof(linha), csv) == NULL) {
        return 0;
    }

    inicio = linha;
    campos[0] = inicio;
    indice = 1;

    for (ponteiroLinha = linha; *ponteiroLinha != '\0'; ponteiroLinha++) {
        if (*ponteiroLinha == ',' && indice < 8) {
            *ponteiroLinha = '\0';
            campos[indice] = ponteiroLinha + 1;
            indice++;
        } else if (*ponteiroLinha == '\n' || *ponteiroLinha == '\r') {
            *ponteiroLinha = '\0';
        }
    }

    if (indice != 8) {
        return 0;
    }

    inicializar_dado(dado);

    dado->codEstacao = atoi(campos[0]);
    definir_string(&dado->nomeEstacao, &dado->tamNomeEstacao, campos[1]);
    dado->codLinha = campo_vazio(campos[2]) ? -1 : atoi(campos[2]);
    definir_string(&dado->nomeLinha, &dado->tamNomeLinha, campos[3]);
    dado->codProxEstacao = campo_vazio(campos[4]) ? -1 : atoi(campos[4]);
    dado->distProxEstacao = campo_vazio(campos[5]) ? -1 : atoi(campos[5]);
    dado->codLinhaIntegra = campo_vazio(campos[6]) ? -1 : atoi(campos[6]);
    dado->codEstIntegra = campo_vazio(campos[7]) ? -1 : atoi(campos[7]);

    if (dado->nomeEstacao == NULL || dado->nomeLinha == NULL) {
        liberar_dado(dado);
        return 0;
    }

    return 1;
}

/*
    Função para escrever um registro no arquivo binário
    Entrada: ponteiro para o arquivo, ponteiro para o dado
    Saída: não há
*/
void escrever_registro(FILE *arquivo, Dado *dado) {
    // Declaração de variável para armazenamento de quantidade de lixo
    int quantidadeLixo;

    // Escrita de cada campo do registro
    fwrite(&dado->removido, sizeof(char), 1, arquivo);
    fwrite(&dado->proximo, sizeof(int), 1, arquivo);
    fwrite(&dado->codEstacao, sizeof(int), 1, arquivo);
    fwrite(&dado->codLinha, sizeof(int), 1, arquivo);
    fwrite(&dado->codProxEstacao, sizeof(int), 1, arquivo);
    fwrite(&dado->distProxEstacao, sizeof(int), 1, arquivo);
    fwrite(&dado->codLinhaIntegra, sizeof(int), 1, arquivo);
    fwrite(&dado->codEstIntegra, sizeof(int), 1, arquivo);
    fwrite(&dado->tamNomeEstacao, sizeof(int), 1, arquivo);

    // Escrita do nome da estação
    if (dado->tamNomeEstacao > 0) {
        fwrite(dado->nomeEstacao, sizeof(char), dado->tamNomeEstacao, arquivo);
    }

    // Escrita do nome da linha
    fwrite(&dado->tamNomeLinha, sizeof(int), 1, arquivo);

    // Escrita do nome da linha (se existir)
    if (dado->tamNomeLinha > 0) {
        fwrite(dado->nomeLinha, sizeof(char), dado->tamNomeLinha, arquivo);
    }

    // Cálculo da quantidade de lixo para preencher o registro
    quantidadeLixo = 80 - (37 + dado->tamNomeEstacao + dado->tamNomeLinha); // 80 é o tamanho do registro e 37 é o tamanho dos campos fixos

    // Preencimento do registro com cifrão (lixo)
    preencher_com_cifrao(arquivo, quantidadeLixo);
}

/*
    Função para ler um registro no arquivo binário
    Entrada: ponteiro para o arquivo, ponteiro para o dado
    Saída: 1 se o registro foi lido com sucesso, 0 caso contrário
*/
int ler_registro(FILE *arquivo, Dado *dado) {
    // Declaração de variável para armazenamento do buffer
    char buffer[80];

    // Declaração de variável para armazenamento do deslocamento
    int deslocamento;

    // Leitura do registro e verifica se a leitura foi feita corretamente
    if (fread(buffer, sizeof(char), 80, arquivo) != 80) {
        return 0;
    }

    // Inicialização do dado
    inicializar_dado(dado);

    // Inicialização do deslocamento    
    deslocamento = 0;

    // Leitura do campo removido e incrementação do deslocamento
    dado->removido = buffer[deslocamento];
    deslocamento += 1;

    // Leitura do campo proximo e incrementação do deslocamento
    memcpy(&dado->proximo, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->codEstacao, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->codLinha, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->codProxEstacao, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->distProxEstacao, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->codLinhaIntegra, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->codEstIntegra, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);
    memcpy(&dado->tamNomeEstacao, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);

    // Alocação da string nomeEstacao
    dado->nomeEstacao = (char *)malloc(dado->tamNomeEstacao + 1);
    if (dado->nomeEstacao == NULL) {
        return 0;
    }
    // Cópia da string nomeEstacao para o dado
    memcpy(dado->nomeEstacao, buffer + deslocamento, dado->tamNomeEstacao);
    // Adição do caractere nulo ao final da string
    dado->nomeEstacao[dado->tamNomeEstacao] = '\0';
    // Incrementação do deslocamento
    deslocamento += dado->tamNomeEstacao;

    // Leitura do campo tamNomeLinha e incrementação do deslocamento
    memcpy(&dado->tamNomeLinha, buffer + deslocamento, sizeof(int));
    deslocamento += sizeof(int);

    // Alocação da string nomeLinha
    dado->nomeLinha = (char *)malloc(dado->tamNomeLinha + 1);
    if (dado->nomeLinha == NULL) {
        free(dado->nomeEstacao);
        dado->nomeEstacao = NULL;
        return 0;
    }
    // Cópia da string nomeLinha para o dado
    memcpy(dado->nomeLinha, buffer + deslocamento, dado->tamNomeLinha);
    // Adição do caractere nulo ao final da string
    dado->nomeLinha[dado->tamNomeLinha] = '\0';
    // Incrementação do deslocamento
    deslocamento += dado->tamNomeLinha;

    return 1;
}

/*
    Função para imprimir um registro
    Entrada: ponteiro para o dado
    Saída: não há
*/
void imprimir_registro(Dado *dado) {
    // Impressão de cada campo do registro
    printf("%d ", dado->codEstacao);
    imprimir_string(dado->nomeEstacao, dado->tamNomeEstacao);
    printf(" ");
    imprimir_inteiro(dado->codLinha);
    printf(" ");
    imprimir_string(dado->nomeLinha, dado->tamNomeLinha);
    printf(" ");
    imprimir_inteiro(dado->codProxEstacao);
    printf(" ");
    imprimir_inteiro(dado->distProxEstacao);
    printf(" ");
    imprimir_inteiro(dado->codLinhaIntegra);
    printf(" ");
    imprimir_inteiro(dado->codEstIntegra);
    printf("\n");
}

/*
    Função para verificar se um registro está ativo
    Entrada: ponteiro para o dado
    Saída: 1 se o registro está ativo, 0 caso contrário
*/
int registro_ativo(Dado *dado) {
    // Verifica se o registro está ativo
    if (dado->removido == '0') {
        return 1;
    }

    return 0;
}
