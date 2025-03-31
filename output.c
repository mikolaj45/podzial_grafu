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

    // Zapisz nagłówek: liczba wierzchołków (n), liczba podziałów (k) i liczba krawędzi (m)
    fwrite(&graph->num_vertices, sizeof(int), 1, file); // n - liczba wierzchołków
    fwrite(&graph->num_partitions, sizeof(int), 1, file); // k - liczba części
    fwrite(&graph->num_edges, sizeof(int), 1, file); // m - liczba krawędzi

    // Zapisz listę wierzchołków w każdej części
    for (int i = 0; i < graph->num_partitions; i++) {
        // Liczba wierzchołków w części i
        int part_size = 0;
        for (int j = 0; j < graph->num_vertices; j++) {
            if (partition[j] == i) {
                part_size++;
            }
        }
        fwrite(&part_size, sizeof(int), 1, file); // Liczba wierzchołków w części

        // Zapisz wierzchołki należące do części i
        for (int j = 0; j < graph->num_vertices; j++) {
            if (partition[j] == i) {
                fwrite(&j, sizeof(int), 1, file); // Zapisz wierzchołek
            }
        }
    }

    fclose(file);
}
