#include "graf2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_LINE_LENGTH 2048

void init_graph(Graph *graph) {
    graph->num_vertices = 0;
    graph->max_vertices = 0;
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->neighbor_count[i] = 0;
        graph->group_assignment[i] = 0;
        graph->max_distances[i] = 0;
        for (int j = 0; j < MAX_NEIGHBORS; j++) {
            graph->neighbors[i][j] = -1;
        }
    }
    
    // Inicjalizacja struktur CSR (puste, ale potrzebne dla partitions.c)
    graph->col_index = NULL;
    graph->row_ptr = NULL;
    graph->group_list = NULL;
    graph->group_ptr = NULL;
}

void add_edge(Graph *graph, int u, int v) {
    if (u >= MAX_VERTICES || v >= MAX_VERTICES) {
        fprintf(stderr, "Błąd: Przekroczono maksymalną liczbę wierzchołków\n");
        return;
    }

    // Dodaj v do sąsiadów u
    if (graph->neighbor_count[u] < MAX_NEIGHBORS) {
        bool exists = false;
        for (int i = 0; i < graph->neighbor_count[u]; i++) {
            if (graph->neighbors[u][i] == v) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            graph->neighbors[u][graph->neighbor_count[u]++] = v;
        }
    }

    // Dodaj u do sąsiadów v (graf nieskierowany)
    if (graph->neighbor_count[v] < MAX_NEIGHBORS) {
        bool exists = false;
        for (int i = 0; i < graph->neighbor_count[v]; i++) {
            if (graph->neighbors[v][i] == u) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            graph->neighbors[v][graph->neighbor_count[v]++] = u;
        }
    }

    // Aktualizacja liczby wierzchołków
    if (u + 1 > graph->num_vertices) graph->num_vertices = u + 1;
    if (v + 1 > graph->num_vertices) graph->num_vertices = v + 1;
}

int load_graph_from_adjlist(Graph *graph, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Nie można otworzyć pliku");
        return -1;
    }

    init_graph(graph);

    char line[MAX_LINE_LENGTH];
    int line_num = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        
        // Pomijaj puste linie i komentarze
        if (line[0] == '#' || line[0] == '\n') continue;
        
        char *token = strtok(line, " \t\n");
        if (!token) continue;
        
        int u = atoi(token);
        
        while ((token = strtok(NULL, " \t\n")) != NULL) {
            int v = atoi(token);
            add_edge(graph, u, v);
        }
    }

    fclose(fp);
    return 0;
}

void print_graph(const Graph *graph) {
    printf("Graf (wierzchołki: %d):\n", graph->num_vertices);
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%d: ", i);
        for (int j = 0; j < graph->neighbor_count[i]; j++) {
            printf("%d ", graph->neighbors[i][j]);
        }
        printf("\n");
    }
}

void free_graph(Graph *graph) {
    // Zwolnij pamięć dla struktur CSR (jeśli były alokowane)
    if (graph->col_index) free(graph->col_index);
    if (graph->row_ptr) free(graph->row_ptr);
    if (graph->group_list) free(graph->group_list);
    if (graph->group_ptr) free(graph->group_ptr);
    
    graph->col_index = NULL;
    graph->row_ptr = NULL;
    graph->group_list = NULL;
    graph->group_ptr = NULL;
}