#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

/**
 * @brief Oblicza liczbę przeciętych krawędzi w podziale grafu.
 * 
 * Dla nieskierowanego grafu każda krawędź jest liczona dwa razy, dlatego wynik dzielony jest przez 2.
 * 
 * @param graph Wskaźnik do struktury Graph.
 * @param partition Tablica z przypisaniami wierzchołków do części.
 * @return Liczba przeciętych krawędzi.
 */
int count_cut_edges(Graph *graph, int *partition);

/**
 * @brief Sprawdza spójność podanej części grafu.
 * 
 * Używa DFS do weryfikacji, czy wszystkie wierzchołki należące do danej części są ze sobą połączone.
 * 
 * @param graph Wskaźnik do struktury Graph.
 * @param partition Tablica z przypisaniami wierzchołków do części.
 * @param part_id Identyfikator części do sprawdzenia.
 * @return 1, jeśli część jest spójna, 0 w przeciwnym przypadku.
 */
int check_partition_connectivity(Graph *graph, int *partition, int part_id);

#endif // UTILS_H
