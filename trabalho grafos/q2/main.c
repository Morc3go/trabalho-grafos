#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CIDADES 100
#define MAX_NOME 50

typedef struct Rota {
    int destino;
    int distancia;
    struct Rota* proxima;
} Rota;

typedef struct Cidade {
    char nome[MAX_NOME];
    Rota* rotas;
} Cidade;

typedef struct Grafo {
    Cidade cidades[MAX_CIDADES];
    int numCidades;
} Grafo;

Grafo grafo;

void inicializarGrafo() {
    grafo.numCidades = 0;
    for (int i = 0; i < MAX_CIDADES; i++) {
        grafo.cidades[i].rotas = NULL;
    }
}

int adicionarCidade(char nome[]) {
    if (grafo.numCidades >= MAX_CIDADES) {
        return -1;
    }
    
    for (int i = 0; i < grafo.numCidades; i++) {
        if (strcmp(grafo.cidades[i].nome, nome) == 0) {
            return -2;
        }
    }
    
    strcpy(grafo.cidades[grafo.numCidades].nome, nome);
    grafo.numCidades++;
    return grafo.numCidades - 1;
}

void adicionarRota(int origem, int destino, int distancia) {
    if (origem < 0 || origem >= grafo.numCidades || 
        destino < 0 || destino >= grafo.numCidades || 
        origem == destino) {
        return;
    }
    
    Rota* novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = destino;
    novaRota->distancia = distancia;
    novaRota->proxima = grafo.cidades[origem].rotas;
    grafo.cidades[origem].rotas = novaRota;
    
    novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = origem;
    novaRota->distancia = distancia;
    novaRota->proxima = grafo.cidades[destino].rotas;
    grafo.cidades[destino].rotas = novaRota;
}

int encontrarIndiceCidade(char nome[]) {
    for (int i = 0; i < grafo.numCidades; i++) {
        if (strcmp(grafo.cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void dijkstra(int origem, int destino) {
    if (origem < 0 || origem >= grafo.numCidades || 
        destino < 0 || destino >= grafo.numCidades) {
        printf("Cidades invalidas.\n");
        return;
    }
    
    int distancia[MAX_CIDADES];
    int visitado[MAX_CIDADES];
    int anterior[MAX_CIDADES];
    
    for (int i = 0; i < grafo.numCidades; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = 0;
        anterior[i] = -1;
    }
    
    distancia[origem] = 0;
    
    for (int count = 0; count < grafo.numCidades - 1; count++) {
        int min = INT_MAX, min_index;
        
        for (int v = 0; v < grafo.numCidades; v++) {
            if (!visitado[v] && distancia[v] <= min) {
                min = distancia[v];
                min_index = v;
            }
        }
        
        int u = min_index;
        visitado[u] = 1;
        
        Rota* rota = grafo.cidades[u].rotas;
        while (rota != NULL) {
            int v = rota->destino;
            if (!visitado[v] && distancia[u] != INT_MAX && 
                distancia[u] + rota->distancia < distancia[v]) {
                distancia[v] = distancia[u] + rota->distancia;
                anterior[v] = u;
            }
            rota = rota->proxima;
        }
    }
    
    if (distancia[destino] == INT_MAX) {
        printf("Nao ha caminho entre %s e %s.\n", grafo.cidades[origem].nome, grafo.cidades[destino].nome);
        return;
    }
    
    printf("Distancia minima de %s para %s: %d km\n", 
           grafo.cidades[origem].nome, grafo.cidades[destino].nome, distancia[destino]);
    
    int caminho[MAX_CIDADES];
    int atual = destino;
    int tamanhoCaminho = 0;
    
    while (anterior[atual] != -1) {
        caminho[tamanhoCaminho++] = atual;
        atual = anterior[atual];
    }
    caminho[tamanhoCaminho++] = origem;
    
    printf("Caminho: ");
    for (int i = tamanhoCaminho - 1; i >= 0; i--) {
        printf("%s", grafo.cidades[caminho[i]].nome);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

void listarCidades() {
    printf("\nCidades cadastradas:\n");
    for (int i = 0; i < grafo.numCidades; i++) {
        printf("%d: %s\n", i, grafo.cidades[i].nome);
    }
    printf("\n");
}

void menu() {
    int opcao;
    char nome1[MAX_NOME];
    int distancia;
    
    do {
        printf("\n=== Sistema de Rotas Otimizadas ===\n");
        printf("1. Cadastrar cidade\n");
        printf("2. Cadastrar rota entre cidades\n");
        printf("3. Listar cidades\n");
        printf("4. Calcular menor caminho\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                printf("Nome da cidade: ");
                scanf("%s", nome1);
                if (adicionarCidade(nome1) >= 0) {
                    printf("Cidade cadastrada com sucesso.\n");
                } else {
                    printf("Erro ao cadastrar cidade.\n");
                }
                break;
                
            case 2:
                listarCidades();
                printf("Indice da cidade de origem: ");
                int origem, destino;
                scanf("%d", &origem);
                printf("Indice da cidade de destino: ");
                scanf("%d", &destino);
                printf("Distancia em km: ");
                scanf("%d", &distancia);
                adicionarRota(origem, destino, distancia);
                printf("Rota cadastrada com sucesso.\n");
                break;
                
            case 3:
                listarCidades();
                break;
                
            case 4:
                listarCidades();
                printf("Indice da cidade de origem: ");
                scanf("%d", &origem);
                printf("Indice da cidade de destino: ");
                scanf("%d", &destino);
                dijkstra(origem, destino);
                break;
                
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

int main() {
    inicializarGrafo();
    menu();
    return 0;
}