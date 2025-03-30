#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Oblicza liczbę przeciętych krawędzi w grafie.
 */
int count_cut_edges(Graph *graph, int *partition) {
    int count = 0;
    int n = graph->num_vertices;
    for (int u = 0; u < n; u++) {
        for (int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; i++) {
            int v = graph->col_index[i];
            if (partition[u] != partition[v]) {
                count++;
            }
        }
    }
    // Każda krawędź w grafie nieskierowanym została policzona dwa razy.
    return count / 2;
}

/* Funkcja pomocnicza – DFS w celu sprawdzenia spójności części. */
static void dfs_check(Graph *graph, int v, int part_id, int *partition, int *visited) {
    visited[v] = 1;
    for (int i = graph->row_ptr[v]; i < graph->row_ptr[v + 1]; i++) {
        int neighbor = graph->col_index[i];
        if (partition[neighbor] == part_id && !visited[neighbor]) {
            dfs_check(graph, neighbor, part_id, partition, visited);
        }
    }
}

/**
 * Sprawdza, czy wszystkie wierzchołki danej części są spójne.
 */
int check_partition_connectivity(Graph *graph, int *partition, int part_id) {
    int n = graph->num_vertices;
    int *visited = (int *)calloc(n, sizeof(int));
    if (!visited) return 0;
    
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (partition[i] == part_id) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        free(visited);
        // Jeśli w danej części nie ma żadnych wierzchołków, przyjmujemy, że jest spójna.
        return 1;
    }
    dfs_check(graph, start, part_id, partition, visited);
    
    for (int i = 0; i < n; i++) {
        if (partition[i] == part_id && !visited[i]) {
            free(visited);
            return 0;
        }
    }
    free(visited);
    return 1;
}
