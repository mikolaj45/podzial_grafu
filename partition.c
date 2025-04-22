#include "partition.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

static void dfs_visit(const Graph *graph, int v, bool visited[]) {
    visited[v] = true;
    for (int i = 0; i < graph->neighbor_count[v]; i++) {
        int neighbor = graph->neighbors[v][i];
        if (!visited[neighbor]) {
            dfs_visit(graph, neighbor, visited);
        }
    }
}

void dijkstra(Graph *graph, int start) {
    int dist[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};

    for (int i = 0; i < graph->num_vertices; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    for (int count = 0; count < graph->num_vertices - 1; count++) {
        int u = -1;
        int min_dist = INF;
        for (int v = 0; v < graph->num_vertices; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        for (int i = 0; i < graph->neighbor_count[u]; i++) {
            int v = graph->neighbors[u][i];
            if (!visited[v] && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
            }
        }
    }

    int max_dist = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (dist[i] != INF && dist[i] > max_dist) {
            max_dist = dist[i];
        }
    }
    graph->max_distances[start] = max_dist;
}

int find_central_vertex(Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        dijkstra(graph, i);
    }

    int min_vertex = 0;
    int min_value = graph->max_distances[0];

    for (int i = 1; i < graph->num_vertices; i++) {
        if (graph->max_distances[i] < min_value) {
            min_value = graph->max_distances[i];
            min_vertex = i;
        }
    }

    return min_vertex;
}

void dfs_select_group1(Graph *graph, int start, int group1[], int *group1_size) {
    bool visited[MAX_VERTICES] = {false};
    int stack[MAX_VERTICES];
    int stack_size = 0;

    stack[stack_size++] = start;
    visited[start] = true;
    group1[(*group1_size)++] = start;
    graph->group_assignment[start] = 1;

    while (stack_size > 0) {
        int current = stack[--stack_size];

        int max_dist = -1;
        int next_vertex = -1;

        for (int i = 0; i < graph->neighbor_count[current]; i++) {
            int neighbor = graph->neighbors[current][i];
            if (!visited[neighbor] && graph->max_distances[neighbor] > max_dist) {
                max_dist = graph->max_distances[neighbor];
                next_vertex = neighbor;
            }
        }

        if (next_vertex != -1) {
            stack[stack_size++] = current;
            stack[stack_size++] = next_vertex;
            visited[next_vertex] = true;
            group1[(*group1_size)++] = next_vertex;
            graph->group_assignment[next_vertex] = 1;
        }
    }
}

bool is_group_connected(const Graph *graph, int group_id) {
    int start = -1;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->group_assignment[i] == group_id) {
            start = i;
            break;
        }
    }
    if (start == -1) return true;

    bool visited[MAX_VERTICES] = {false};
    int stack[MAX_VERTICES];
    int stack_size = 0;
    int visited_count = 0;

    stack[stack_size++] = start;
    visited[start] = true;

    while (stack_size > 0) {
        int current = stack[--stack_size];
        visited_count++;

        for (int i = 0; i < graph->neighbor_count[current]; i++) {
            int neighbor = graph->neighbors[current][i];
            if (graph->group_assignment[neighbor] == group_id && !visited[neighbor]) {
                visited[neighbor] = true;
                stack[stack_size++] = neighbor;
            }
        }
    }

    int group_size = 0;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->group_assignment[i] == group_id) group_size++;
    }

    return visited_count == group_size;
}

void partition_graph(Graph *graph, int group1[], int *group1_size, int group2[], int *group2_size, int margin) {
    // Inicjalizacja grup
    *group1_size = 0;
    *group2_size = 0;
    
    // Inicjalizacja przypisania do grup
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->group_assignment[i] = 0;
    }

    int center = find_central_vertex(graph);
    
    // Wybierz grupę 1 poprzez DFS
    dfs_select_group1(graph, center, group1, group1_size);
    
    // Pozostałe wierzchołki do grupy 2
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->group_assignment[i] != 1) {
            group2[(*group2_size)++] = i;
            graph->group_assignment[i] = 2;
        }
    }

    // --- NOWA CZĘŚĆ: Naprawianie niespójności grupy 2 ---
    if (!is_group_connected(graph, 2)) {
        // Znajdź największy spójny komponent w grupie 2
        int largest_component_size = 0;
        bool in_largest_component[MAX_VERTICES] = {false};
        bool visited[MAX_VERTICES] = {false};
        
        for (int i = 0; i < graph->num_vertices; i++) {
            if (graph->group_assignment[i] == 2 && !visited[i]) {
                bool current_visited[MAX_VERTICES] = {false};
                int component_size = 0;
                
                // Wykonaj DFS dla tego komponentu
                int stack[MAX_VERTICES];
                int stack_size = 0;
                stack[stack_size++] = i;
                current_visited[i] = true;
                
                while (stack_size > 0) {
                    int current = stack[--stack_size];
                    component_size++;
                    
                    for (int j = 0; j < graph->neighbor_count[current]; j++) {
                        int neighbor = graph->neighbors[current][j];
                        if (graph->group_assignment[neighbor] == 2 && !current_visited[neighbor]) {
                            current_visited[neighbor] = true;
                            stack[stack_size++] = neighbor;
                        }
                    }
                }
                
                // Aktualizuj największy komponent
                if (component_size > largest_component_size) {
                    largest_component_size = component_size;
                    memcpy(in_largest_component, current_visited, sizeof(in_largest_component));
                }
                
                // Oznacz jako odwiedzone
                for (int j = 0; j < graph->num_vertices; j++) {
                    if (current_visited[j]) visited[j] = true;
                }
            }
        }
        
        // Przenieś wierzchołki spoza największego komponentu do grupy 1
        int new_group2_size = 0;
        for (int i = 0; i < graph->num_vertices; i++) {
            if (graph->group_assignment[i] == 2) {
                if (in_largest_component[i]) {
                    group2[new_group2_size++] = i;
                } else {
                    graph->group_assignment[i] = 1;
                    group1[(*group1_size)++] = i;
                }
            }
        }
        *group2_size = new_group2_size;
    }

    // Sprawdź margines błędu
    int size_diff = abs(*group1_size - *group2_size);
    if (size_diff > margin) {
        balance_groups(graph, group1, group1_size, group2, group2_size, margin);
    }
}

bool balance_groups(Graph *graph, int group1[], int *group1_size, int group2[], int *group2_size, int margin) {
    // Przygotuj posortowaną listę wierzchołków z grupy 1 według max_distance (rosnąco)
    typedef struct {
        int vertex;
        int max_dist;
    } VertexInfo;
    
    VertexInfo vertices[MAX_VERTICES];
    int count = 0;
    
    for (int i = 0; i < *group1_size; i++) {
        int v = group1[i];
        vertices[count].vertex = v;
        vertices[count].max_dist = graph->max_distances[v];
        count++;
    }
    
    // Sortowanie przez wstawianie (stabilne i efektywne dla małych/średnich tablic)
    for (int i = 1; i < count; i++) {
        VertexInfo key = vertices[i];
        int j = i - 1;
        while (j >= 0 && vertices[j].max_dist > key.max_dist) {
            vertices[j+1] = vertices[j];
            j--;
        }
        vertices[j+1] = key;
    }

    bool progress;
    do {
        progress = false;
        
        // Próbuj przenosić wierzchołki w kolejności od najmniejszego max_distance
        for (int i = 0; i < count; i++) {
            int v = vertices[i].vertex;
            
            // Sprawdź czy wierzchołek nadal należy do grupy 1
            bool in_group1 = false;
            for (int j = 0; j < *group1_size; j++) {
                if (group1[j] == v) {
                    in_group1 = true;
                    break;
                }
            }
            if (!in_group1) continue;
            
            // 1. Sprawdź czy grupa 1 pozostanie spójna bez tego wierzchołka
            bool group1_remains_connected = true;
            
            // Tymczasowo usuń wierzchołek
            graph->group_assignment[v] = 0;
            
            if (*group1_size > 1) { // Grupa 1-elementowa jest zawsze spójna
                // Znajdź pierwszego sąsiada w grupie 1 jako punkt startowy DFS
                int start = -1;
                for (int j = 0; j < graph->neighbor_count[v]; j++) {
                    int neighbor = graph->neighbors[v][j];
                    if (graph->group_assignment[neighbor] == 1) {
                        start = neighbor;
                        break;
                    }
                }
                
                if (start != -1) {
                    // Wykonaj DFS na grupie 1 bez tego wierzchołka
                    bool visited[MAX_VERTICES] = {false};
                    int stack[MAX_VERTICES];
                    int stack_size = 0;
                    int visited_count = 0;
                    
                    stack[stack_size++] = start;
                    visited[start] = true;
                    
                    while (stack_size > 0) {
                        int current = stack[--stack_size];
                        visited_count++;
                        
                        for (int j = 0; j < graph->neighbor_count[current]; j++) {
                            int neighbor = graph->neighbors[current][j];
                            if (graph->group_assignment[neighbor] == 1 && !visited[neighbor]) {
                                visited[neighbor] = true;
                                stack[stack_size++] = neighbor;
                            }
                        }
                    }
                    
                    // Porównaj z rzeczywistym rozmiarem grupy 1 minus 1 (usunięty wierzchołek)
                    int expected_count = *group1_size - 1;
                    group1_remains_connected = (visited_count == expected_count);
                } else {
                    // Brak sąsiadów - grupa rozpadnie się
                    group1_remains_connected = false;
                }
            }
            
            // Przywróć wierzchołek do grupy 1
            graph->group_assignment[v] = 1;
            
            if (!group1_remains_connected) continue;
            
            // 2. Sprawdź czy grupa 2 będzie spójna z dodanym wierzchołkiem
            bool group2_will_be_connected = true;
            
            // Tymczasowo dodaj wierzchołek do grupy 2
            graph->group_assignment[v] = 2;
            
            if (*group2_size > 0) { // Jeśli grupa 2 nie jest pusta
                // Znajdź pierwszego sąsiada w grupie 2 jako punkt startowy DFS
                int start = -1;
                for (int j = 0; j < graph->neighbor_count[v]; j++) {
                    int neighbor = graph->neighbors[v][j];
                    if (graph->group_assignment[neighbor] == 2) {
                        start = neighbor;
                        break;
                    }
                }
                
                if (start != -1) {
                    // Wykonaj DFS na grupie 2 z nowym wierzchołkiem
                    bool visited[MAX_VERTICES] = {false};
                    int stack[MAX_VERTICES];
                    int stack_size = 0;
                    int visited_count = 0;
                    
                    stack[stack_size++] = start;
                    visited[start] = true;
                    
                    while (stack_size > 0) {
                        int current = stack[--stack_size];
                        visited_count++;
                        
                        for (int j = 0; j < graph->neighbor_count[current]; j++) {
                            int neighbor = graph->neighbors[current][j];
                            if (graph->group_assignment[neighbor] == 2 && !visited[neighbor]) {
                                visited[neighbor] = true;
                                stack[stack_size++] = neighbor;
                            }
                        }
                    }
                    
                    // Oczekiwana liczba wierzchołków w grupie 2 po dodaniu
                    int expected_count = *group2_size + 1;
                    group2_will_be_connected = (visited_count == expected_count);
                } else {
                    // Nowy wierzchołek nie ma sąsiadów w grupie 2
                    group2_will_be_connected = (*group2_size == 0); // Tylko jeśli grupa 2 była pusta
                }
            }
            
            // Przywróć oryginalny stan przed podjęciem decyzji
            graph->group_assignment[v] = 1;
            
            if (group1_remains_connected && group2_will_be_connected) {
                // Wykonaj przeniesienie
                graph->group_assignment[v] = 2;
                group2[(*group2_size)++] = v;
                
                // Usuń z grupy 1
                for (int j = 0; j < *group1_size; j++) {
                    if (group1[j] == v) {
                        for (int k = j; k < *group1_size-1; k++) {
                            group1[k] = group1[k+1];
                        }
                        (*group1_size)--;
                        break;
                    }
                }
                
                progress = true;
                
                // Sprawdź warunek marginesu
                int size_diff = abs(*group1_size - *group2_size);
                if (size_diff <= margin) {
                    return true;
                }
                
                // Zacznij przeszukiwanie od nowa, bo zmieniły się grupy
                break;
            }
        }
    } while (progress);
    
    return (abs(*group1_size - *group2_size) <= margin);
}