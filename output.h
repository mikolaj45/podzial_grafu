#ifndef OUTPUT_H
#define OUTPUT_H

#include "graph.h"

void save_to_text(Graph *graph, int *partition, const char *filename);
void save_to_binary(Graph *graph, int *partition, const char *filename);

#endif // OUTPUT_H