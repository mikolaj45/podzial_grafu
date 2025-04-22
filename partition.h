#ifndef PARTITION_H
#define PARTITION_H

#include "graph.h"  // Zakładam, że struktura Graph i stałe są zdefiniowane tutaj

#define MAX_VERTICES 1000
#define INF 1000000

// Znajduje centralny wierzchołek grafu (ten o najmniejszej maksymalnej odległości do innych)
int find_central_vertex(Graph *graph);

// Algorytm Dijkstry – uzupełnia tablicę max_distances dla podanego wierzchołka
void dijkstra(Graph *graph, int start);

// DFS wybierający wierzchołki do grupy 1 zaczynając od wierzchołka centralnego
void dfs_select_group1(Graph *graph, int start, int group1[], int *group1_size);

// Sprawdza, czy dana grupa w grafie jest spójna
bool is_group_connected(const Graph *graph, int group_id);

// Dzieli graf na dwie spójne grupy z uwzględnieniem marginesu błędu w wielkości grup
void partition_graph(Graph *graph, int group1[], int *group1_size, int group2[], int *group2_size, int margin);

// Balansuje grupy, zachowując ich spójność i przestrzegając marginesu
bool balance_groups(Graph *graph, int group1[], int *group1_size, int group2[], int *group2_size, int margin);

#endif // PARTITIONS_H
