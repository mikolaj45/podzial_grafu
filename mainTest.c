#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graf.h"
#include "partitions.h"

#define MAX_FILENAME_LENGTH 100

int main() {
    printf("start\n");
    // Inicjalizacja grafu
    Graph g;
    init_graph(&g);

    // Wczytywanie nazwy pliku
    char filename[MAX_FILENAME_LENGTH];
    printf("Podaj nazwę pliku z grafem: ");
    if (fgets(filename, MAX_FILENAME_LENGTH, stdin) == NULL) {
        fprintf(stderr, "Błąd wczytywania nazwy pliku\n");
        return EXIT_FAILURE;
    }

    // Usuwanie znaku nowej linii z nazwy pliku
    filename[strcspn(filename, "\n")] = '\0';

    // Wczytanie grafu z pliku
    if (!load_graph_from_csrrg(&g, filename)) {
        fprintf(stderr, "Nie można wczytać pliku lub błędny format\n");
        return EXIT_FAILURE;
    }
    // Wyświetlenie informacji o grafie
    printf("\nWczytano graf o %d wierzchołkach (max %d)\n", g.num_vertices, g.max_vertices);
    print_graph(&g);

    // Podział grafu
    int group1[MAX_VERTICES], group2[MAX_VERTICES];
    int group1_size, group2_size;
    int margin;

    printf("\nPodaj dopuszczalny margines różnicy wielkości grup: ");
    scanf("%d", &margin);

    partition_graph(&g, group1, &group1_size, group2, &group2_size, margin);

    // Wyświetlenie wyników
    printf("\nWynik podziału:\n");
    printf("Grupa 1 (%d wierzchołków): ", group1_size);
    for (int i = 0; i < group1_size; i++) printf("%d ", group1[i]);
    
    printf("\nGrupa 2 (%d wierzchołków): ", group2_size);
    for (int i = 0; i < group2_size; i++) printf("%d ", group2[i]);

    printf("\n\nRóżnica rozmiarów: %d (margines: %d)", 
           abs(group1_size - group2_size), margin);
    printf("\nCzy grupa 1 spójna: %s", is_group_connected(&g, 1) ? "TAK" : "NIE");
    printf("\nCzy grupa 2 spójna: %s\n", is_group_connected(&g, 2) ? "TAK" : "NIE");

    return EXIT_SUCCESS;
}