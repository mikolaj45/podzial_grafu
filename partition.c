#include "partition.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ------------------------------
// Funkcja Dijkstry (uproszczona)
// ------------------------------
static void dijkstra(Graph *graph, int source, int *dist) {
    int n = graph->num_vertices;
    int *visited = (int *)calloc(n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Błąd alokacji pamięci w dijkstra.\n");
        return;
    }
    int successful_splits = 0;//dodac obliczanie udanych podzialow
    for (int i = 0; i < n; i++) {
        dist[i] = (i == source) ? 0 : INT_MAX;
    }

    for (int count = 0; count < n; count++) {
        int u = -1;
        int min = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }
        if (u == -1) break;

        visited[u] = 1;

        for (int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; i++) {
            int v = graph->col_index[i];
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
            }
        }
    }
    
    free(visited);
}

// --------------------------------------------
// Wybór wierzchołka centralnego (find_center)
// --------------------------------------------
int find_center(Graph *graph) {
    int n = graph->num_vertices;
    if(n <= 0) return -1;

    int best_vertex = -1;
    long best_sum = LONG_MAX;
    int *dist = (int *)malloc(n * sizeof(int));
    if (!dist) {
        fprintf(stderr, "Błąd alokacji pamięci w find_center.\n");
        return -1;
    }

    for (int v = 0; v < n; v++) {
        dijkstra(graph, v, dist);
        long sum = 0;
        for (int i = 0; i < n; i++) {
            if (dist[i] == INT_MAX) continue;
            sum += dist[i];
        }
        if (sum < best_sum) {
            best_sum = sum;
            best_vertex = v;
        }
    }

    free(dist);
    return best_vertex;
}

// ------------------------------
// DFS rekurencyjny
// ------------------------------
void dfs(Graph *graph, int v, int *visited, int part_id, int *partition) {
    visited[v] = 1;
    partition[v] = part_id;

    for (int i = graph->row_ptr[v]; i < graph->row_ptr[v + 1]; i++) {
        int neighbor = graph->col_index[i];
        if (!visited[neighbor]) {
            dfs(graph, neighbor, visited, part_id, partition);
        }
    }
}

// --------------------------------------------------------
// Korekta spójności części – zapewnienie, że każda część
// jest spójnym podgrafem.
// Jeśli w danej części występują rozdzielone składowe,
// próbuje przypisać "odłączone" wierzchołki do sąsiedniej części.
// --------------------------------------------------------
static void correct_partition_connectivity(Graph *graph, int num_parts, int *partition) {
    int n = graph->num_vertices;
    int *visited = (int *)calloc(n, sizeof(int));
    if (!visited) return;

    // Dla każdej części sprawdzamy spójność
    for (int part = 0; part < num_parts; part++) {
        int start = -1;
        for (int i = 0; i < n; i++) {
            if (partition[i] == part) {
                start = i;
                break;
            }
        }
        if (start == -1) continue;

        // DFS w obrębie danej części
        int *stack = (int *)malloc(n * sizeof(int));
        if (!stack) continue;
        int stack_size = 0;
        stack[stack_size++] = start;
        visited[start] = 1;
        while (stack_size > 0) {
            int cur = stack[--stack_size];
            for (int j = graph->row_ptr[cur]; j < graph->row_ptr[cur + 1]; j++) {
                int neighbor = graph->col_index[j];
                if (partition[neighbor] == part && !visited[neighbor]) {
                    visited[neighbor] = 1;
                    stack[stack_size++] = neighbor;
                }
            }
        }
        free(stack);

        // Wykrycie odłączonych składowych – przypisanie ich do pierwszej napotkanej innej części.
        for (int i = 0; i < n; i++) {
            if (partition[i] == part && !visited[i]) {
                for (int j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
                    int neighbor = graph->col_index[j];
                    if (partition[neighbor] != part) {
                        partition[i] = partition[neighbor];
                        break;
                    }
                }
            }
        }
        // Resetowanie visited dla kolejnej części
        for (int i = 0; i < n; i++) {
            if (partition[i] == part) {
                visited[i] = 0;
            }
        }
    }
    free(visited);
}

// --------------------------------------------------------
// Korekta równowagi części – próba wyrównania liczby
// wierzchołków w poszczególnych częściach na podstawie max_diff.
// W tej uproszczonej wersji, dla wierzchołków należących do
// zbyt małej części, sprawdzamy czy któryś z sąsiadów należy
// do części o znacznie większej liczbie wierzchołków i dokonujemy reasignacji.
// --------------------------------------------------------
static void correct_partition_balance(Graph *graph, int num_parts, int max_diff, int *partition) {
    int n = graph->num_vertices;
    int *sizes = (int *)calloc(num_parts, sizeof(int));
    if (!sizes) return;

    // Obliczenie rozmiarów poszczególnych części
    for (int i = 0; i < n; i++) {
        int part = partition[i];
        if (part >= 0 && part < num_parts)
            sizes[part]++;
    }

    // Obliczenie średniej liczby wierzchołków (jako przybliżenie równowagi)
    int avg = n / num_parts;

    // Przegląd wierzchołków – próba reasignacji wierzchołka do innej części
    // jeśli jego część jest "za mała" i sąsiadująca część "za duża".
    for (int i = 0; i < n; i++) {
        int current = partition[i];
        if (sizes[current] < avg - max_diff) {
            // Sprawdzamy sąsiadów wierzchołka
            for (int j = graph->row_ptr[i]; j < graph->row_ptr[i + 1]; j++) {
                int neighbor = graph->col_index[j];
                int neighbor_part = partition[neighbor];
                if (sizes[neighbor_part] > avg + max_diff) {
                    // Przypisujemy wierzchołek do części sąsiada
                    sizes[current]--;
                    partition[i] = neighbor_part;
                    sizes[neighbor_part]++;
                    break;
                }
            }
        }
    }
    free(sizes);
}

// --------------------------------------------------------
// Główna funkcja realizująca podział grafu
// --------------------------------------------------------
void dfs_partition(Graph *graph, int num_parts, int max_diff, int *partition) {
    int n = graph->num_vertices;
    int *visited = (int *)calloc(n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Błąd alokacji pamięci w dfs_partition.\n");
        return;
    }
    
    // Inicjalizacja tablicy partition wartością -1 (nieprzypisane)
    for (int i = 0; i < n; i++) {
        partition[i] = -1;
    }
    
    // 1. Wybór punktu startowego przy użyciu find_center()
    int center = find_center(graph);
    if (center == -1) {
        fprintf(stderr, "Błąd: Nie udało się wyznaczyć wierzchołka centralnego.\n");
        free(visited);
        return;
    }
    
    // 2. DFS od wierzchołka centralnego – przydział części 0
    dfs(graph, center, visited, 0, partition);
    
    // 3. Dla pozostałych nieodwiedzonych wierzchołków, DFS i przypisanie kolejnych części
    int current_part = 1;
    for (int v = 0; v < n; v++) {
        if (!visited[v]) {
            dfs(graph, v, visited, current_part, partition);
            current_part = (current_part + 1) % num_parts;
        }
    }
    
    // 4. Korekta – zapewnienie spójności każdej części
    correct_partition_connectivity(graph, num_parts, partition);
    // 5. Korekta – kontrola równowagi między częściami wg max_diff
    correct_partition_balance(graph, num_parts, max_diff, partition);

    free(visited);
}
