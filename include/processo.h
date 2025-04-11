#ifndef PROCESSO_H
#define PROCESSO_H

#include <time.h>

// Constantes
#define MAX_LINE 1024
#define MAX_ASSUNTOS 50
#define MAX_PROCESSOS 20000
#define DATE_FORMAT "%Y-%m-%d %H:%M:%S"

// Estrutura para armazenar dados de um processo
typedef struct {
    long long id;
    char numero[30];
    char data_ajuizamento[30];
    time_t timestamp;
    int *id_classes;
    int num_classes;
    int *id_assuntos;
    int num_assuntos;
    int ano_eleicao;
} Processo;

// Estrutura para armazenar conjunto de assuntos únicos
typedef struct {
    int *ids;
    int contador;
    int capacidade;
} AssuntosUnicos;

// Funções de comparação para ordenação
int compararPorId(const void *a, const void *b);
int compararPorData(const void *a, const void *b);

// Funções auxiliares
int* extrairNumeros(const char *str, int *quantidade);
time_t converterData(const char *data_str);

// Função para ler os dados do arquivo CSV
int lerDados(const char *nome_arquivo, Processo processos[]);

// Funções de ordenação
void ordenarPorId(Processo processos[], int qtd_processos);
void ordenarPorData(Processo processos[], int qtd_processos);

// Função para exibir processos
void exibirProcessos(Processo processos[], int qtd_processos, int limite);

// Função para salvar processos em arquivo CSV
int salvarProcessosCSV(Processo processos[], int qtd_processos, const char *nome_arquivo);

// Função para contar processos por classe
int contarProcessosPorClasse(Processo processos[], int qtd_processos, int id_classe);

// Funções para assuntos únicos
void adicionarAssuntoUnico(AssuntosUnicos *assuntos, int id_assunto);
int identificarAssuntosUnicos(Processo processos[], int qtd_processos, AssuntosUnicos *assuntos);

// Função para listar processos com múltiplos assuntos
void listarProcessosMultiplosAssuntos(Processo processos[], int qtd_processos);

// Funções para cálculo de dias em tramitação
int calcularDiasEmTramitacao(Processo processo);
void exibirDiasEmTramitacao(Processo processos[], int qtd_processos, const char *numero_processo);

// Função para liberar memória
void liberarMemoria(Processo processos[], int qtd_processos, AssuntosUnicos *assuntos);

#endif // PROCESSO_H