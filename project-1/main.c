/*
 * Nome: Renan Silva Blasques                | NUSP: 9784057
 * Nome: Rodrigo de Jesus Ferreira Gonçalves | NUSP: 16899823
 */

#include <stdio.h>
#include "funcionalidades.h"

int main() {
    int funcionalidade;

    // Leitura da funcionalidade escolhida pelo usuário (1, 2, 3 ou 4)
    // Se a leitura falhar, o programa termina
    if (scanf("%d", &funcionalidade) != 1) {
        return 0;
    }

    // Execução da funcionalidade escolhida pelo usuário
    switch (funcionalidade) {
        case 1:
            // Funcionalidade 1: Criar arquivo binário a partir de um arquivo CSV
            // Entrada na tela: nomeCsv.csv nomeBinario.bin (exemplo: "estacoes.csv estacoes.bin")
            executar_funcionalidade_1();
            break;
        case 2:
            // Funcionalidade 2: Imprimir registros existentes em um arquivo binário
            // Entrada na tela: nomeBinario.bin (exemplo: "estacoes.bin")
            executar_funcionalidade_2();
            break;
        case 3:
            // Funcionalidade 3: Buscar registros que satisfazem os filtros em um arquivo binário
            // Entrada na tela: nomeBinario.bin quantidadeBuscas (exemplo: "estacoes.bin 2")
            executar_funcionalidade_3();
            break;
        case 4:
            // Funcionalidade 4: Imprimir um registro específico em um arquivo binário
            // Entrada na tela: nomeBinario.bin RRN (exemplo: "estacoes.bin 0")
            executar_funcionalidade_4();
            break;
        default:
            // Se a funcionalidade escolhida não for válida, o programa termina
            break;
    }

    return 0;
}