typedef struct cabecalho {
  int status;   // indica consistência - atualiza para 0 ao abrir e 1 ao fechar
  int topo;     // registra o byte offset
  int proxRRN;  // armazena o valor do próximo RRN disponível
  int nroEstacoes;      // mesmo nome - mesma estação
  int nroParesEstacao;  // quantidade de pares (codEstacao e codProxEstacao)
} cabecalho;

typedef struct dado {  // max 80 bytes (37 bytes fixos e 43 bytes sobrando para
                       // variáveis)
  char removido;       // indica se registro foi removido - 1 byte
  int proximo;         // indica próximo registro - 4 bytes

  int codEstacao;       // campo de tamanho fixo - 4 bytes (not null)
  int codLinha;         // campo de tamanho fixo - 4 bytes (null = -1)
  int codProxEstacao;   // campo de tamanho fixo - 4 bytes (null = -1)
  int distProxEstacao;  // campo de tamanho fixo - 4 bytes (null = -1)
  int codLinhaIntegra;  // campo de tamanho fixo - 4 bytes (null = -1)
  int codEstIntegra;    // campo de tamanho fixo - 4 bytes (null = -1)

  int tamNomeEstacao;  // delimitador de tamanho - 4 bytes
  char* nomeEstacao;   // campo de tamanho variável (not null)
  int tamNomeLinha;    // delimitador de tamanho - 4 bytes (nomeLinha null = 0)
  char* nomeLinha;     // campo de tamanho variável
} dado;

int main() {
  // programa principal aqui

  return 0;
}