#ifndef GRAF2_H
#define GRAF2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 1000
#define MAX_NEIGHBORS 50
#define MAX_LINE_LENGTH 2048
#define INF INT_MAX

typedef struct {
    // Lista sąsiedztwa
    int neighbors[MAX_VERTICES][MAX_NEIGHBORS];
    int neighbor_count[MAX_VERTICES];
    
    // Potrzebne dla partitions.c
    int max_distances[MAX_VERTICES];
    int group_assignment[MAX_VERTICES];
    int num_vertices;
    int max_vertices;
    
    // Wskaźniki dla struktur CSR (dla kompatybilności)
    int *col_index;
    int *row_ptr;
    int *group_list;
    int *group_ptr;
} Graph;

void init_graph(Graph *graph);
void add_edge(Graph *graph, int u, int v);
int load_graph_from_adjlist(Graph *graph, const char *filename);
void print_graph(const Graph *graph);
void free_graph(Graph *graph);

#endif