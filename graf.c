#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define INITIAL_ALLOC_SIZE 100

// Funkcje pomocnicze do wczytywania grafu w formacie .csrrg i .txt
static int load_graph_csrrg(Graph *graph, FILE *fp);
static int load_graph_txt(Graph *graph, FILE *fp);

<<<<<<< Updated upstream
int load_graph_from_file(Graph *graph, const char *filename) {
=======
void add_edge(Graph *graph, int u, int v) {
    // Sprawdź czy można dodać krawędź
    if (u >= MAX_VERTICES || v >= MAX_VERTICES) {
        fprintf(stderr, "Błąd: Przekroczono maksymalną liczbę wierzchołków u=%d, v=%d\n", u, v);
        return;
    }

    // Dodaj v do sąsiadów u (jeśli jest miejsce)
    if (graph->neighbor_count[u] < MAX_NEIGHBORS) {
        bool exists = false;
        for (int i = 0; i < graph->neighbor_count[u]; i++) {
            if (graph->neighbors[u][i] == v) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            graph->neighbors[u][graph->neighbor_count[u]++] = v;
        }
    } else {
        fprintf(stderr, "Ostrzeżenie: Osiągnięto maksymalną liczbę sąsiadów dla wierzchołka %d\n", u);
    }

    // Dodaj u do sąsiadów v (graf nieskierowany)
    if (graph->neighbor_count[v] < MAX_NEIGHBORS) {
        bool exists = false;
        for (int i = 0; i < graph->neighbor_count[v]; i++) {
            if (graph->neighbors[v][i] == u) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            graph->neighbors[v][graph->neighbor_count[v]++] = u;
        }
    }

    // Aktualizacja liczby wierzchołków
    if (u + 1 > graph->num_vertices) graph->num_vertices = u + 1;
    if (v + 1 > graph->num_vertices) graph->num_vertices = v + 1;
}

void convert_csr_to_neighbors(Graph *graph) {
    // 1. Wyczyść istniejące połączenia
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->neighbor_count[i] = 0;
    }

    // 2. Konwersja CSR → lista sąsiedztwa
    for (int u = 0; u < graph->num_vertices; u++) {
        int start = graph->row_ptr[u];
        int end = graph->row_ptr[u + 1];
        
        for (int idx = start; idx < end; idx++) {
            int v = graph->col_index[idx];
            
            // Pomijaj pętle własne
            if (u == v) continue;
            
            // Dodaj krawędź tylko jeśli jest miejsce i nie istnieje
            if (graph->neighbor_count[u] < MAX_NEIGHBORS) {
                bool exists = false;
                for (int i = 0; i < graph->neighbor_count[u]; i++) {
                    if (graph->neighbors[u][i] == v) {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists) {
                    graph->neighbors[u][graph->neighbor_count[u]++] = v;
                    // W grafie nieskierowanym dodaj też krawędź w drugą stronę
                    if (graph->neighbor_count[v] < MAX_NEIGHBORS) {
                        graph->neighbors[v][graph->neighbor_count[v]++] = u;
                    }
                }
            }
        }
    }
}
int load_graph_from_csrrg(Graph *graph, const char *filename) {
>>>>>>> Stashed changes
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Błąd: Nie można otworzyć pliku %s\n", filename);
        return -1;
    }
    
    // Rozpoznanie formatu pliku na podstawie rozszerzenia
    const char *dot = strrchr(filename, '.');
    int result = 0;
    if (dot && strcmp(dot, ".csrrg") == 0) {
        result = load_graph_csrrg(graph, fp);
    } else if (dot && strcmp(dot, ".txt") == 0) {
        result = load_graph_txt(graph, fp);
    } else {
        fprintf(stderr, "Błąd: Nieznany format pliku %s\n", filename);
        result = -1;
    }
    
    fclose(fp);
    return result;
}

// Wczytywanie grafu z pliku .csrrg
static int load_graph_csrrg(Graph *graph, FILE *fp) {
    char line[MAX_LINE_LENGTH];

    // Sekcja 1: Rozmiar grafu
    if (fgets(line, sizeof(line), fp) == NULL) return -1;
    graph->num_vertices = atoi(line);

    // Alokacja pamięci dla sekcji 2, 3, 4, 5
    graph->col_index = (int *)malloc(INITIAL_ALLOC_SIZE * sizeof(int));
    graph->row_ptr   = (int *)malloc((graph->num_vertices + 1) * sizeof(int));
    graph->group_list = (int *)malloc(INITIAL_ALLOC_SIZE * sizeof(int));
    graph->group_ptr = (int *)malloc((graph->num_vertices + 1) * sizeof(int));

    if (!graph->col_index || !graph->row_ptr || !graph->group_list || !graph->group_ptr) {
        fprintf(stderr, "Błąd: Problemy z alokacją pamięci.\n");
        return -1;
    }
    
    // Sekcja 2: Układ wierzchołków (col_index)
    if (fgets(line, sizeof(line), fp) == NULL) return -1;
    {
        int idx = 0;
        char *token = strtok(line, ";");
        while (token != NULL) {
            if (idx >= INITIAL_ALLOC_SIZE) {
                graph->col_index = realloc(graph->col_index, (idx + 1) * sizeof(int));
                if (!graph->col_index) return -1;
            }
            graph->col_index[idx++] = atoi(token);
            token = strtok(NULL, ";");
        }
    }
    
    // Sekcja 3: Rozkład wierszy (row_ptr)
    if (fgets(line, sizeof(line), fp) == NULL) return -1;
    {
        int idx = 0;
        char *token = strtok(line, ";");
        while (token != NULL && idx < graph->num_vertices + 1) {
            graph->row_ptr[idx++] = atoi(token);
            token = strtok(NULL, ";");
        }
    }
    
    // Sekcja 4: Lista grup połączonych wierzchołków (group_list)
    if (fgets(line, sizeof(line), fp) == NULL) return -1;
    {
        int idx = 0;
        char *token = strtok(line, ";");
        while (token != NULL) {
            if (idx >= INITIAL_ALLOC_SIZE) {
                graph->group_list = realloc(graph->group_list, (idx + 1) * sizeof(int));
                if (!graph->group_list) return -1;
            }
            graph->group_list[idx++] = atoi(token);
            token = strtok(NULL, ";");
        }
    }
    
    // Sekcja 5: Wskaźniki na pierwsze węzły w grupach (group_ptr)
    if (fgets(line, sizeof(line), fp) == NULL) return -1;
    {
        int idx = 0;
        char *token = strtok(line, ";");
        while (token != NULL && idx < graph->num_vertices + 1) {
            graph->group_ptr[idx++] = atoi(token);
            token = strtok(NULL, ";");
        }
    }
    
    return 0;
}

// Wczytywanie grafu z pliku .txt
static int load_graph_txt(Graph *graph, FILE *fp) {
    int rows, cols;
    
    // Odczytujemy wymiary macierzy
    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Błąd: Nie można odczytać wymiarów grafu.\n");
        return -1;
    }

    // Alokujemy miejsce na macierz grafu (0 i 1)
    int **map = malloc(rows * sizeof(int *));
    if (!map) return -1;
    for (int i = 0; i < rows; i++) {
        map[i] = malloc(cols * sizeof(int));
        if (!map[i]) return -1;
    init_graph(graph);

    char line[MAX_LINE_LENGTH];
    int *temp_array = NULL;
    int count = 0;

    // Sekcja 1: Maksymalna liczba wierzchołków
    if (!fgets(line, sizeof(line), fp)) goto error;
    graph->max_vertices = atoi(line);

    // Sekcja 2: col_index (indeksy kolumn)
    if (!fgets(line, sizeof(line), fp)) goto error;
    temp_array = malloc(MAX_LINE_LENGTH * sizeof(int));
    count = 0;
    char *token = strtok(line, ";");
    while (token && count < MAX_LINE_LENGTH) {
        temp_array[count++] = atoi(token);
        token = strtok(NULL, ";");
    }

    // Wczytujemy mapę grafu
    int num_vertices = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fscanf(fp, "%d", &map[i][j]) != 1) {
                fprintf(stderr, "Błąd: Nieprawidłowa mapa grafu.\n");
                return -1;
            }
            if (map[i][j] == 1) num_vertices++; // Liczymy wierzchołki
    // Sekcja 3: row_ptr (zakresy wierszy)
    if (!fgets(line, sizeof(line), fp)) goto error;
    temp_array = malloc(MAX_LINE_LENGTH * sizeof(int));
    count = 0;
    token = strtok(line, ";");
    while (token && count < MAX_LINE_LENGTH) {
        temp_array[count++] = atoi(token);
        token = strtok(NULL, ";");
    }
    graph->row_ptr = malloc(count * sizeof(int));
    memcpy(graph->row_ptr, temp_array, count * sizeof(int));
    graph->num_vertices = count - 1;
    free(temp_array);
    temp_array = NULL;

    // Sekcja 4: group_list (lista grup)
    if (!fgets(line, sizeof(line), fp)) goto error;
    temp_array = malloc(MAX_LINE_LENGTH * sizeof(int));
    count = 0;
    token = strtok(line, ";");
    while (token && count < MAX_LINE_LENGTH) {
        temp_array[count++] = atoi(token);
        token = strtok(NULL, ";");
    }
    graph->group_list = malloc(count * sizeof(int));
    memcpy(graph->group_list, temp_array, count * sizeof(int));
    free(temp_array);
    temp_array = NULL;

    // Sekcja 5: group_ptr (wskaźniki grup)
    if (!fgets(line, sizeof(line), fp)) goto error;
    temp_array = malloc(MAX_LINE_LENGTH * sizeof(int));
    count = 0;
    token = strtok(line, ";");
    while (token && count < MAX_LINE_LENGTH) {
        temp_array[count++] = atoi(token);
        token = strtok(NULL, ";");
    }
    graph->group_ptr = malloc(count * sizeof(int));
    memcpy(graph->group_ptr, temp_array, count * sizeof(int));
    free(temp_array);


    
    convert_csr_to_neighbors(graph);

   // Konwersja CSR do listy sąsiedztwa — poprawiona wersja
    /*for (int i = 0; i < graph->num_vertices; i++) {
        int start = graph->row_ptr[i];
        int end = graph->row_ptr[i + 1];
        for (int j = start; j < end; j++) {
            int neighbor = graph->col_index[j];
            if (i != neighbor) {
                add_edge(graph, i, neighbor);  // dodaje i→neighbor i neighbor→i (bo add_edge jest dwukierunkowy)
            }
        }
    }*/

    // Przypisanie wierzchołków do grup
    /*for (int group_id = 0; group_id < count - 1; group_id++) {
        int start = graph->group_ptr[group_id];
        int end = graph->group_ptr[group_id+1];
        for (int i = start; i < end; i++) {
            int vertex = graph->group_list[i];
            if (vertex < graph->num_vertices) {
                graph->group_assignment[vertex] = group_id + 1; // Grupy od 1
            }
        }
    }

    fclose(fp);*/
    return 0;

error:
    if (fp) fclose(fp);
    if (temp_array) free(temp_array);
    free_graph(graph);
    return -1;
}
void dfs(const Graph *graph, int v, bool visited[]) {
    visited[v] = true;
    for (int i = 0; i < graph->neighbor_count[v]; i++) {
        int neighbor = graph->neighbors[v][i];
        if (!visited[neighbor]) {
            dfs(graph, neighbor, visited);
        }
    }

    // Tworzymy struktury CSR
    graph->num_vertices = num_vertices;
    graph->col_index = malloc(num_vertices * sizeof(int));
    graph->row_ptr = malloc((num_vertices + 1) * sizeof(int));

    if (!graph->col_index || !graph->row_ptr) return -1;

    graph->row_ptr[0] = 0;
    int edge_count = 0;
    
    // Wczytujemy listę krawędzi
    int u, v;
    while (fscanf(fp, "%d %d", &u, &v) == 2) {
        graph->col_index[edge_count++] = v; 
        graph->row_ptr[u + 1] = edge_count; 
    }

    // Zwolnienie tymczasowej macierzy
    for (int i = 0; i < rows; i++) free(map[i]);
    free(map);

    return 0;
}

void print_graph(const Graph *graph) {
    printf("Liczba wierzchołków: %d\n", graph->num_vertices);
    
    if (graph->col_index) {
        printf("Col_index: ");
        for (int i = 0; i < graph->num_vertices; i++) { // Drukujemy tylko załadowane elementy
            printf("%d ", graph->col_index[i]);
        }
        printf("\n");
    }
    
    if (graph->row_ptr) {
        printf("Row_ptr: ");
        for (int i = 0; i < graph->num_vertices + 1; i++) { // Drukujemy tylko załadowane elementy
            printf("%d ", graph->row_ptr[i]);
        }
        printf("\n");
    }
    
    if (graph->group_list) {
        printf("Group_list: ");
        for (int i = 0; i < graph->num_vertices; i++) { // Drukujemy tylko załadowane elementy
            printf("%d ", graph->group_list[i]);
        }
        printf("\n");
    }
    
    if (graph->group_ptr) {
        printf("Group_ptr: ");
        for (int i = 0; i < graph->num_vertices + 1; i++) { // Drukujemy tylko załadowane elementy
            printf("%d ", graph->group_ptr[i]);
        }
        printf("\n");
    }
}

void free_graph(Graph *graph) {
    if (graph->col_index) free(graph->col_index);
    if (graph->row_ptr) free(graph->row_ptr);
    if (graph->group_list) free(graph->group_list);
    if (graph->group_ptr) free(graph->group_ptr);
}
