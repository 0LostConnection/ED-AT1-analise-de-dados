#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

int main() {
    Processo processos[MAX_PROCESSOS];
    AssuntosUnicos assuntos = {NULL, 0, 0};
    char nome_arquivo[100];
    int id_classe_pesquisa;
    int qtd_processos;
    char numero_processo[30];
    int opcao;
    
    printf("=== Sistema de Processamento de Dados Judiciais ===\n\n");
    
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome_arquivo);
    
    // Carrega os dados do arquivo
    qtd_processos = lerDados(nome_arquivo, processos);
    
    if (qtd_processos == 0) {
        printf("Nenhum processo foi carregado. Verifique o arquivo.\n");
        return 1;
    }
    
    printf("Foram carregados %d processos com sucesso!\n\n", qtd_processos);
    
    // Loop do menu principal
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Ordenar processos por ID (crescente)\n");
        printf("2. Ordenar processos por data de ajuizamento (decrescente)\n");
        printf("3. Contar processos por classe\n");
        printf("4. Identificar quantidade de assuntos únicos\n");
        printf("5. Listar processos com múltiplos assuntos\n");
        printf("6. Verificar dias em tramitação de um processo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                // Ordenar por ID (crescente)
                printf("\n1. Processos ordenados por ID (crescente):\n");
                ordenarPorId(processos, qtd_processos);
                exibirProcessos(processos, qtd_processos, 10);
                break;
                
            case 2:
                // Ordenar por data de ajuizamento (decrescente)
                printf("\n2. Processos ordenados por data de ajuizamento (decrescente):\n");
                ordenarPorData(processos, qtd_processos);
                exibirProcessos(processos, qtd_processos, 10);
                break;
                
            case 3:
                // Contar processos por classe
                printf("\n3. Contar processos por classe\n");
                printf("Digite o ID da classe que deseja pesquisar: ");
                scanf("%d", &id_classe_pesquisa);
                
                int qtd_por_classe = contarProcessosPorClasse(processos, qtd_processos, id_classe_pesquisa);
                printf("Existem %d processos vinculados à classe %d.\n", qtd_por_classe, id_classe_pesquisa);
                break;
                
            case 4:
                // Identificar quantidade de assuntos únicos
                printf("\n4. Identificar quantidade de assuntos únicos\n");
                int qtd_assuntos_unicos = identificarAssuntosUnicos(processos, qtd_processos, &assuntos);
                printf("Existem %d assuntos distintos nos processos.\n", qtd_assuntos_unicos);
                
                // Opção para exibir a lista de assuntos
                char listar;
                printf("Deseja listar os assuntos? (S/N): ");
                scanf(" %c", &listar);
                
                if (listar == 'S' || listar == 's') {
                    printf("Lista de assuntos únicos:\n");
                    for (int i = 0; i < assuntos.contador; i++) {
                        printf("- %d\n", assuntos.ids[i]);
                    }
                }
                break;
                
            case 5:
                // Listar processos com múltiplos assuntos
                printf("\n5. Listar processos com múltiplos assuntos\n");
                listarProcessosMultiplosAssuntos(processos, qtd_processos);
                break;
                
            case 6:
                // Verificar dias em tramitação
                printf("\n6. Verificar dias em tramitação de um processo\n");
                printf("Digite o número do processo: ");
                scanf("%s", numero_processo);
                exibirDiasEmTramitacao(processos, qtd_processos, numero_processo);
                break;
                
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
                
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    // Liberar memória alocada
    liberarMemoria(processos, qtd_processos, &assuntos);
    
    return 0;
}