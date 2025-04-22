#ifndef GRAPH_H
#define GRAPH_H
// Struktura przechowująca graf w formacie CSR dla pliku .csrrg
typedef struct {
    int num_vertices;   // Liczba wierzchołków (Sekcja 1)
    int *col_index;     // Indeksy kolumnowe, czyli układ wierzchołków (Sekcja 2)
    int *row_ptr;       // Wskaźniki na zakresy wierszy (Sekcja 3)
    int *group_list;    // Lista grup połączonych wierzchołków (Sekcja 4)
    int *group_ptr;     // Wskaźniki na pierwsze węzły w grupach (Sekcja 5)
    int **tab_sas;
    int num_points;
    int num_partitions;  // do formatu binarnego
    int num_edges;       // do formatu binarnego
} Graph;

// Wczytuje graf z pliku wejściowego (format .csrrg lub .txt)
// Zwraca 0 przy sukcesie, -1 przy błędzie
int load_graph_from_file(Graph *graph, const char *filename);

// Wypisuje zawartość grafu (do celów debugowania)
void print_graph(const Graph *graph);

// Zwalnia pamięć zaalokowaną dla grafu
void free_graph(Graph *graph);



#endif 
