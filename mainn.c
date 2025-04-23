#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include "graf.h"
#include "partitions.h"
=======
#include "graph.h"
#include "partition.h"
>>>>>>> emilk

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Użycie: %s <plik_grafu> [margines%%]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int margin = 10; // domyślny margines
    if (argc >= 3) {
        margin = atoi(argv[2]);
    }

    Graph graph;
    if (load_graph_from_csrrg(&graph, filename) != 0) {
        fprintf(stderr, "Błąd podczas wczytywania grafu.\n");
        return 1;
    }

    int group1[MAX_VERTICES];
    int group2[MAX_VERTICES];
    int group1_size = 0;
    int group2_size = 0;

    partition_graph(&graph, group1, &group1_size, group2, &group2_size, margin);

    printf("Grupa 1 (rozmiar %d):\n", group1_size);
    for (int i = 0; i < group1_size; i++) {
        printf("%d ", group1[i]);
    }
    printf("\n");

    printf("Grupa 2 (rozmiar %d):\n", group2_size);
    for (int i = 0; i < group2_size; i++) {
        printf("%d ", group2[i]);
    }
    printf("\n");

    free_graph(&graph);
    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> emilk
