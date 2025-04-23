#include "graf.h"
#include "partitions.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Użycie: %s <plik_z_listą_sąsiedztwa>\n", argv[0]);
        return 1;
    }

    Graph graph;
    
    /*if (load_graph_from_adjlist(&graph, argv[1]) != 0) {
        fprintf(stderr, "Błąd wczytywania grafu\n");
        return 1;
    }*/
    
    print_graph(&graph);
    
    int group1[MAX_VERTICES], group2[MAX_VERTICES];
    int group1_size = 0, group2_size = 0;
    int margin = 2;
    
    partition_graph(&graph, group1, &group1_size, group2, &group2_size, margin);
    
    printf("\nPodział:\nGrupa 1 (%d): ", group1_size);
    for (int i = 0; i < group1_size; i++) printf("%d ", group1[i]);
    
    printf("\nGrupa 2 (%d): ", group2_size);
    for (int i = 0; i < group2_size; i++) printf("%d ", group2[i]);
    printf("Graf jest %sspójny\n", is_connected(&graph) ? "" : "nie ");
    printf("Grupa 1 jest %sspójna\n", is_group_connected(&graph, 1) ? "" : "nie ");
    printf("Grupa 2 jest %sspójna\n", is_group_connected(&graph, 2) ? "" : "nie ");
    printf("\n");
    
    free_graph(&graph);
    return 0;
}