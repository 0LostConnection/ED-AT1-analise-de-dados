#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/processo.h"

// Função para comparar processos por ID (ordem crescente)
int compararPorId(const void *a, const void *b)
{
    Processo *procA = (Processo *)a;
    Processo *procB = (Processo *)b;

    if (procA->id < procB->id)
        return -1;
    if (procA->id > procB->id)
        return 1;
    return 0;
}

// Função para comparar processos por data (ordem decrescente)
int compararPorData(const void *a, const void *b)
{
    Processo *procA = (Processo *)a;
    Processo *procB = (Processo *)b;

    if (procA->timestamp > procB->timestamp)
        return -1;
    if (procA->timestamp < procB->timestamp)
        return 1;
    return 0;
}

// Função para extrair números de uma string entre chaves
int *extrairNumeros(const char *str, int *quantidade)
{
    int capacidade = 10;
    int *numeros = (int *)malloc(capacidade * sizeof(int));
    *quantidade = 0;

    // Verificar se a string contém chaves
    if (str[0] != '{' || str[strlen(str) - 1] != '}')
    {
        return numeros;
    }

    char *copia = _strdup(str + 1);  // Pular o caractere '{'
    copia[strlen(copia) - 1] = '\0'; // Remover o caractere '}'

    char *token = strtok(copia, ",");
    while (token != NULL)
    {
        if (*quantidade >= capacidade)
        {
            capacidade *= 2;
            numeros = (int *)realloc(numeros, capacidade * sizeof(int));
        }
        numeros[*quantidade] = atoi(token);
        (*quantidade)++;
        token = strtok(NULL, ",");
    }

    free(copia);
    return numeros;
}

// Implementação personalizada de strptime para Windows
time_t converterData(const char *data_str)
{
    struct tm tm = {0};
    char data_copia[30];
    strncpy(data_copia, data_str, sizeof(data_copia) - 1);
    data_copia[sizeof(data_copia) - 1] = '\0';

    // Remover a parte dos milissigundos
    char *ms = strstr(data_copia, ".");
    if (ms)
        *ms = '\0';

    // Formato esperado: YYYY-MM-DD HH:MM:SS
    int ano, mes, dia, hora, min, seg;
    if (sscanf(data_copia, "%d-%d-%d %d:%d:%d", &ano, &mes, &dia, &hora, &min, &seg) != 6)
    {
        return 0;
    }

    tm.tm_year = ano - 1900; // Anos desde 1900
    tm.tm_mon = mes - 1;     // Meses de 0-11
    tm.tm_mday = dia;
    tm.tm_hour = hora;
    tm.tm_min = min;
    tm.tm_sec = seg;
    tm.tm_isdst = -1; // Determinar DST automaticamente

    return mktime(&tm);
}

// Implementação personalizada de strsep para Windows
char *meu_strsep(char **stringp, const char *delim)
{
    char *start = *stringp;
    char *p;

    if (start == NULL)
    {
        return NULL;
    }

    p = strpbrk(start, delim);

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}

// Função para ler os dados do arquivo CSV
int lerDados(const char *nome_arquivo, Processo processos[])
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }

    char linha[MAX_LINE];
    int contador = 0;

    // Pular a linha de cabeçalho
    fgets(linha, MAX_LINE, arquivo);

    while (fgets(linha, MAX_LINE, arquivo) && contador < MAX_PROCESSOS)
    {
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        // Parsing da linha CSV
        char *token;
        char *resto = linha;

        // ID
        token = meu_strsep(&resto, ",");
        processos[contador].id = atoll(token);

        // Número
        token = meu_strsep(&resto, ",");
        // Remover aspas do número
        if (token[0] == '"')
        {
            // Copia sem a primeira aspas
            strncpy(processos[contador].numero, token + 1, sizeof(processos[contador].numero) - 1);
            // Remove a última aspas
            int len = strlen(processos[contador].numero);
            if (len > 0 && processos[contador].numero[len - 1] == '"')
            {
                processos[contador].numero[len - 1] = '\0';
            }
        }
        else
        {
            strncpy(processos[contador].numero, token, sizeof(processos[contador].numero) - 1);
        }
        processos[contador].numero[sizeof(processos[contador].numero) - 1] = '\0';

        // Data de ajuizamento
        token = meu_strsep(&resto, ",");
        strncpy(processos[contador].data_ajuizamento, token, sizeof(processos[contador].data_ajuizamento) - 1);
        processos[contador].data_ajuizamento[sizeof(processos[contador].data_ajuizamento) - 1] = '\0';
        processos[contador].timestamp = converterData(token);

        // Classes
        char classes_str[MAX_LINE] = "";
        int pos = 0;
        int dentro_chaves = 0;

        while (*resto)
        {
            if (*resto == '{')
                dentro_chaves = 1;
            else if (*resto == '}')
                dentro_chaves = 0;

            if (*resto == ',' && !dentro_chaves)
                break;
            classes_str[pos++] = *resto;
            resto++;
        }
        classes_str[pos] = '\0';
        if (*resto == ',')
            resto++;

        processos[contador].id_classes = extrairNumeros(classes_str, &processos[contador].num_classes);

        // Assuntos
        char assuntos_str[MAX_LINE] = "";
        pos = 0;
        dentro_chaves = 0;

        while (*resto)
        {
            if (*resto == '{')
                dentro_chaves = 1;
            else if (*resto == '}')
                dentro_chaves = 0;

            if (*resto == ',' && !dentro_chaves)
                break;
            assuntos_str[pos++] = *resto;
            resto++;
        }
        assuntos_str[pos] = '\0';
        if (*resto == ',')
            resto++;

        processos[contador].id_assuntos = extrairNumeros(assuntos_str, &processos[contador].num_assuntos);

        // Ano eleição
        processos[contador].ano_eleicao = atoi(resto);

        contador++;
    }

    fclose(arquivo);
    return contador;
}

// Função para ordenar processos por ID (crescente)
void ordenarPorId(Processo processos[], int qtd_processos)
{
    qsort(processos, qtd_processos, sizeof(Processo), compararPorId);
}

// Função para ordenar processos por data de ajuizamento (decrescente)
void ordenarPorData(Processo processos[], int qtd_processos)
{
    qsort(processos, qtd_processos, sizeof(Processo), compararPorData);
}

// Função para exibir os primeiros N processos
void exibirProcessos(Processo processos[], int qtd_processos, int limite)
{
    printf("ID\t\tNumero\t\t\tData Ajuizamento\n");
    printf("------------------------------------------------------\n");
    int qtd_exibir = (qtd_processos < limite) ? qtd_processos : limite;

    for (int i = 0; i < qtd_exibir; i++)
    {
        printf("%lld\t%s\t%s\n",
               processos[i].id,
               processos[i].numero,
               processos[i].data_ajuizamento);
    }

    if (qtd_processos > limite)
    {
        printf("... (total: %d processos)\n", qtd_processos);
    }
}

// Função para salvar processos em arquivo CSV
int salvarProcessosCSV(Processo processos[], int qtd_processos, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo)
    {
        printf("Erro ao criar o arquivo %s\n", nome_arquivo);
        return 0;
    }

    // Escrever cabeçalho
    fprintf(arquivo, "ID,Numero,Data Ajuizamento,Classes,Assuntos,Ano Eleicao\n");

    // Escrever dados
    for (int i = 0; i < qtd_processos; i++)
    {
        // Escrever ID, número e data
        fprintf(arquivo, "%lld,%s,%s,",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento);

        // Escrever classes
        fprintf(arquivo, "{");
        for (int j = 0; j < processos[i].num_classes; j++)
        {
            fprintf(arquivo, "%d", processos[i].id_classes[j]);
            if (j < processos[i].num_classes - 1)
                fprintf(arquivo, ",");
        }
        fprintf(arquivo, "},");

        // Escrever assuntos
        fprintf(arquivo, "{");
        for (int j = 0; j < processos[i].num_assuntos; j++)
        {
            fprintf(arquivo, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].num_assuntos - 1)
                fprintf(arquivo, ",");
        }
        fprintf(arquivo, "},");

        // Escrever ano eleição
        fprintf(arquivo, "%d\n", processos[i].ano_eleicao);
    }

    fclose(arquivo);
    return 1;
}

// Função para contar processos vinculados a um determinado id_classe
int contarProcessosPorClasse(Processo processos[], int qtd_processos, int id_classe)
{
    int contador = 0;

    for (int i = 0; i < qtd_processos; i++)
    {
        for (int j = 0; j < processos[i].num_classes; j++)
        {
            if (processos[i].id_classes[j] == id_classe)
            {
                contador++;
                break; // Se encontrou a classe, não precisa verificar outras classes do mesmo processo
            }
        }
    }

    return contador;
}

// Função para adicionar um assunto único à lista de assuntos únicos
void adicionarAssuntoUnico(AssuntosUnicos *assuntos, int id_assunto)
{
    // Verificar se o assunto já existe na lista
    for (int i = 0; i < assuntos->contador; i++)
    {
        if (assuntos->ids[i] == id_assunto)
        {
            return; // O assunto já existe, não adiciona novamente
        }
    }

    // Verificar se precisa expandir o array
    if (assuntos->contador >= assuntos->capacidade)
    {
        assuntos->capacidade = (assuntos->capacidade == 0) ? MAX_ASSUNTOS : assuntos->capacidade * 2;
        assuntos->ids = (int *)realloc(assuntos->ids, assuntos->capacidade * sizeof(int));
    }

    // Adicionar o novo assunto
    assuntos->ids[assuntos->contador++] = id_assunto;
}

// Função para identificar quantos id_assuntos únicos existem
int identificarAssuntosUnicos(Processo processos[], int qtd_processos, AssuntosUnicos *assuntos)
{
    // Liberar memória antiga se existir
    if (assuntos->ids != NULL)
    {
        free(assuntos->ids);
    }

    assuntos->contador = 0;
    assuntos->capacidade = MAX_ASSUNTOS;
    assuntos->ids = (int *)malloc(assuntos->capacidade * sizeof(int));

    for (int i = 0; i < qtd_processos; i++)
    {
        for (int j = 0; j < processos[i].num_assuntos; j++)
        {
            adicionarAssuntoUnico(assuntos, processos[i].id_assuntos[j]);
        }
    }

    return assuntos->contador;
}

// Função para listar processos vinculados a mais de um assunto
void listarProcessosMultiplosAssuntos(Processo processos[], int qtd_processos)
{
    printf("\nProcessos vinculados a mais de um assunto:\n");
    printf("ID\t\tNumero\t\t\tData Ajuizamento\t\tAssuntos\n");
    printf("----------------------------------------------------------------------\n");

    int encontrou = 0;

    for (int i = 0; i < qtd_processos; i++)
    {
        if (processos[i].num_assuntos > 1)
        {
            encontrou = 1;
            printf("%lld\t%s\t%s\t{",
                   processos[i].id,
                   processos[i].numero,
                   processos[i].data_ajuizamento);

            for (int j = 0; j < processos[i].num_assuntos; j++)
            {
                printf("%d", processos[i].id_assuntos[j]);
                if (j < processos[i].num_assuntos - 1)
                    printf(",");
            }

            printf("}\n");
        }
    }

    if (!encontrou)
    {
        printf("Nenhum processo encontrado com multiplos assuntos.\n");
    }
}

// Função para calcular dias em tramitação
int calcularDiasEmTramitacao(Processo processo)
{
    time_t agora;
    time(&agora);

    // Calcular diferença em segundos
    double diferenca = difftime(agora, processo.timestamp);

    // Converter para dias (86400 segundos em um dia)
    return (int)(diferenca / 86400);
}

// Função para exibir dias em tramitação de um processo específico
void exibirDiasEmTramitacao(Processo processos[], int qtd_processos, const char *numero_processo)
{
    for (int i = 0; i < qtd_processos; i++)
    {
        if (strcmp(processos[i].numero, numero_processo) == 0)
        {
            int dias = calcularDiasEmTramitacao(processos[i]);
            printf("O processo %s esta em tramitacao ha %d dias.\n",
                   numero_processo, dias);
            return;
        }
    }

    printf("Processo nao encontrado.\n");
}

// Função para liberar a memória alocada
void liberarMemoria(Processo processos[], int qtd_processos, AssuntosUnicos *assuntos)
{
    for (int i = 0; i < qtd_processos; i++)
    {
        free(processos[i].id_classes);
        free(processos[i].id_assuntos);
    }

    if (assuntos->ids != NULL)
    {
        free(assuntos->ids);
        assuntos->ids = NULL;
    }
}