#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "partition.h"
#include "utils.h"
#include "output.h"

/* Funkcja wyświetlająca poprawne wywołanie programu. */
void print_usage(char *prog_name) {
    printf("Użycie: %s -i input_file -o output_file -p num_cuts -m max_diff [-b] [-t]\n", prog_name);
    printf("   -i : Plik wejściowy z grafem\n");
    printf("   -o : Plik wyjściowy z podziałem grafu\n");
    printf("   -p : Liczba przecięć aby podzielić graf\n");
    printf("   -m : Maksymalna dopuszczalna różnica rozmiarów między częściami\n");
    printf("   -b : (Opcjonalnie) Zapis do pliku binarnego\n");
    printf("   -t : (Opcjonalnie) Wypisanie wyniku w terminalu\n");
}

int main(int argc, char **argv) {
    char *input_file = NULL;
    char *output_file = NULL;
    int num_cuts = 0;
    int max_diff = 0;
    int binary_output = 0;
    int terminal = 0;
    
    // Prosty parser argumentów z linii poleceń
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            num_cuts = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            max_diff = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            binary_output = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            terminal = 1;
        } else {
            print_usage(argv[0]);
            return 1;
        }
    }
    
    if (!input_file || !output_file || num_cuts <= 0) {
        print_usage(argv[0]);
        return 1;
    }
    // Przekształcenie num_cuts na num_parts (np. przy założeniu, że podział to num_cuts + 1)
    int num_parts = num_cuts + 1;  // Zakładam, że liczba części = liczba przecięć + 1

    // Wczytanie grafu
    Graph graph;
    if (load_graph_from_file(&graph, input_file) != 0) {
        fprintf(stderr, "Błąd: Nie udało się wczytać grafu z pliku %s\n", input_file);
        return 1;
    }
    
    // Alokacja pamięci dla tablicy przechowującej podział wierzchołków
    int *partition = (int *)malloc(graph.num_vertices * sizeof(int));
    if (!partition) {
        fprintf(stderr, "Błąd: Nie udało się alokować pamięci dla partition.\n");
        free_graph(&graph);
        return 1;
    }
    //int successful_splits = 0;
    // Podział grafu z wykorzystaniem DFS oraz korekty spójności i równowagi
    dfs_partition(&graph, num_parts, max_diff, partition);
    
 
    
    // Sprawdzenie spójności poszczególnych części
    for (int i = 0; i < num_parts; i++) {
        if (!check_partition_connectivity(&graph, partition, i)) {
            printf("Część %d nie jest spójna.\n", i);
        } else {
            printf("Część %d jest spójna.\n", i);
        }
    }
    if (terminal) {
        //printf(successful_splits);
        print_graph(&graph);  // Funkcja wypisująca graf w terminalu
    }
    // Zapis wyniku – wykorzystujemy funkcje z output.c
    if (binary_output) {
        // Przykładowe wywołanie funkcji zapisu do pliku binarnego
        save_to_binary(&graph, partition, output_file);
    } else {
        // Przykładowe wywołanie funkcji zapisu do pliku tekstowego
        save_to_text(&graph, partition, output_file);
    }
    
   
    free(partition);
    free_graph(&graph);
    
    return 0;
}
