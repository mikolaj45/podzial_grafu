#ifndef PARTITION_H
#define PARTITION_H

#include "graph.h"

/**
 * @brief Znajduje wierzchołek centralny grafu.
 * 
 * Wykorzystuje uproszczoną wersję algorytmu Dijkstry (z wagą = 1 dla każdej krawędzi)
 * do obliczenia sumy najkrótszych ścieżek z danego wierzchołka do pozostałych.
 * Wybiera wierzchołek o minimalnej sumie jako punkt startowy.
 * 
 * @param graph Wskaźnik do struktury Graph.
 * @return Numer wierzchołka centralnego lub -1 w przypadku błędu.
 */
int find_center(Graph *graph);

/**
 * @brief Rekurencyjne przeszukiwanie DFS.
 * 
 * Funkcja przypisuje wierzchołek v do części (part_id) oraz odwiedza rekurencyjnie wszystkich
 * sąsiadów nieodwiedzonych.
 * 
 * @param graph Wskaźnik do struktury Graph.
 * @param v Numer bieżącego wierzchołka.
 * @param visited Tablica zaznaczająca odwiedzone wierzchołki.
 * @param part_id Numer przypisywanej części.
 * @param partition Tablica z przypisaniami wierzchołków do części.
 */
void dfs(Graph *graph, int v, int *visited, int part_id, int *partition);

/**
 * @brief Główna funkcja realizująca podział grafu.
 * 
 * Najpierw wybiera punkt startowy przy pomocy find_center(), a następnie wykonuje DFS,
 * przypisując wierzchołki do kolejnych części. Po wstępnym podziale wywoływane są dodatkowe
 * korekty – weryfikacja spójności części oraz kontrola równowagi (max_diff).
 * 
 * @param graph Wskaźnik do struktury Graph.
 * @param num_parts Liczba części, na które dzielimy graf.
 * @param max_diff Maksymalna dopuszczalna różnica rozmiarów między częściami.
 * @param partition Tablica wynikowa z przypisaniami wierzchołków do części.
 */
void dfs_partition(Graph *graph, int num_parts, int max_diff, int *partition);

#endif // PARTITION_H
