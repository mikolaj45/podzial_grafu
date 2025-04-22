#include "graph.h"
#include "partition.h"
#include <stdio.h>
#include <time.h>

void test_dijkstra() {
    printf("=== Test Dijkstry ===\n");
    Graph g;
    init_graph(&g);
    
    // Przykładowy graf
    add_edge(&g, 0, 1);
    add_edge(&g, 1, 2);
    add_edge(&g, 2, 3);
    add_edge(&g, 3, 4);
    add_edge(&g, 0, 4);
    
    dijkstra(&g, 0);
    
    printf("Odległości od wierzchołka 0:\n");
    for (int i = 0; i < g.num_vertices; i++) {
        printf("Wierzchołek %d: max_distance = %d\n", i, g.max_distances[i]);
    }
    
    free_graph(&g);
}

void test_find_central() {
    printf("\n=== Test znajdowania centrum ===\n");
    Graph g;
    init_graph(&g);
    
    // Graf w kształcie gwiazdy
    for (int i = 1; i < 5; i++) {
        add_edge(&g, 0, i);
    }
    
    int center = find_central_vertex(&g);
    printf("Centralny wierzchołek: %d (oczekiwany: 0)\n", center);
    
    free_graph(&g);
}

void test_partition() {
    printf("\n=== Test partycjonowania ===\n");
    Graph g;
    init_graph(&g);
    
    // Graf z dwoma wyraźnymi grupami
    for (int i = 0; i < 5; i++) {
        for (int j = i+1; j < 5; j++) {
            add_edge(&g, i, j);
        }
    }
    for (int i = 5; i < 10; i++) {
        for (int j = i+1; j < 10; j++) {
            add_edge(&g, i, j);
        }
    }
    add_edge(&g, 4, 5); // Łącznik między grupami
    
    int group1[MAX_VERTICES], group2[MAX_VERTICES];
    int group1_size, group2_size;
    
    partition_graph(&g, group1, &group1_size, group2, &group2_size, 1);
    
    printf("Grupa 1 (%d): ", group1_size);
    for (int i = 0; i < group1_size; i++) printf("%d ", group1[i]);
    printf("\nGrupa 2 (%d): ", group2_size);
    for (int i = 0; i < group2_size; i++) printf("%d ", group2[i]);
    
    printf("\nSpójność: G1=%d, G2=%d\n", 
           is_group_connected(&g, 1), 
           is_group_connected(&g, 2));
    
    free_graph(&g);
}

void test_balance() {
    printf("\n=== Test balansowania ===\n");
    Graph g;
    init_graph(&g);
    
    // Celowo niezbalansowany graf
    for (int i = 0; i < 8; i++) {
        for (int j = i+1; j < 8; j++) {
            add_edge(&g, i, j);
        }
    }
    
    int group1[MAX_VERTICES] = {0,1,2,3,4,5,6,7};
    int group2[MAX_VERTICES] = {};
    int group1_size = 8, group2_size = 0;
    
    printf("Przed balansowaniem: G1=%d, G2=%d\n", group1_size, group2_size);
    
    clock_t start = clock();
    bool success = balance_groups(&g, group1, &group1_size, group2, &group2_size, 1);
    clock_t end = clock();
    
    printf("Po balansowaniu: G1=%d, G2=%d (sukces=%d)\n", 
           group1_size, group2_size, success);
    printf("Czas wykonania: %.2f ms\n", (double)(end-start)*1000/CLOCKS_PER_SEC);
    
    free_graph(&g);
}

void test_edge_cases() {
    printf("\n=== Testy skrajne ===\n");
    
    // Pusty graf
    Graph g1;
    init_graph(&g1);
    int g1_1[MAX_VERTICES], g1_2[MAX_VERTICES];
    int g1_s1 = 0, g1_s2 = 0;
    partition_graph(&g1, g1_1, &g1_s1, g1_2, &g1_s2, 1);
    printf("Pusty graf: G1=%d, G2=%d\n", g1_s1, g1_s2);
    free_graph(&g1);
    
    // Graf z jednym wierzchołkiem
    Graph g2;
    init_graph(&g2);
    add_edge(&g2, 0, 0);
    int g2_1[MAX_VERTICES], g2_2[MAX_VERTICES];
    int g2_s1 = 0, g2_s2 = 0;
    partition_graph(&g2, g2_1, &g2_s1, g2_2, &g2_s2, 0);
    printf("1-wierzchołkowy: G1=%d, G2=%d\n", g2_s1, g2_s2);
    free_graph(&g2);
}

int main() {
    test_dijkstra();
    test_find_central();
    test_partition();
    test_balance();
    test_edge_cases();
    return 0;
}