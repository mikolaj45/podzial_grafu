#include "output.h"
#include <stdio.h>
#include <stdlib.h>


void save_to_text(Graph *graph, int *partition, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", filename);
        return;
    }

    // Zapisujemy liczbę wierzchołków
    fprintf(file, "%d\n", graph->num_vertices);
    
    // Zapisujemy dane o col_index
    for (int i = 0; i < graph->num_vertices; i++) {
        fprintf(file, "%d;", graph->col_index[i]);
    }
    fprintf(file, "\n");
    
    // Zapisujemy dane o row_ptr
    for (int i = 0; i < graph->num_vertices + 1; i++) {
        fprintf(file, "%d;", graph->row_ptr[i]);
    }
    fprintf(file, "\n");
    
    // Zapisujemy dane o group_list
    for (int i = 0; i < graph->num_vertices; i++) {
        fprintf(file, "%d;", graph->group_list[i]);
    }
    fprintf(file, "\n");
    
    // Zapisujemy dane o group_ptr
    for (int i = 0; i < graph->num_vertices + 1; i++) {
        fprintf(file, "%d;", graph->group_ptr[i]);
    }
    fprintf(file, "\n");
    
    fclose(file);
}

void save_to_binary(Graph *graph, int *partition, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", filename);
        return;
    }

    // First determine the actual number of partitions used
    int max_partition = -1;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (partition[i] > max_partition) {
            max_partition = partition[i];
        }
    }
    int actual_partitions = max_partition + 1;

    // Count vertices in each partition
    int *partition_counts = calloc(actual_partitions, sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        partition_counts[partition[i]]++;
    }

    // Calculate number of edges (assuming undirected graph)
    // This might need adjustment based on your actual graph representation
    int num_edges = 0;
    if (graph->row_ptr && graph->col_index) {
        num_edges = graph->row_ptr[graph->num_vertices] / 2; // For undirected
    }

    // Write header
    fwrite(&graph->num_vertices, sizeof(int), 1, file); // n
    fwrite(&actual_partitions, sizeof(int), 1, file);   // k
    fwrite(&num_edges, sizeof(int), 1, file);          // m

    // Write partitions
    for (int i = 0; i < actual_partitions; i++) {
        // Write partition size
        fwrite(&partition_counts[i], sizeof(int), 1, file);
        
        // Write vertices in this partition
        for (int j = 0; j < graph->num_vertices; j++) {
            if (partition[j] == i) {
                fwrite(&j, sizeof(int), 1, file);
            }
        }
    }

    free(partition_counts);
    fclose(file);
}