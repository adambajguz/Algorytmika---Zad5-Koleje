#ifndef PS5_ZAD3_DIJKSTRA_H
#define PS5_ZAD3_DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.h"

uint16_t dijkstra(const struct t_Graph *const graph, const uint16_t start, const uint16_t end);

#endif //PS5_ZAD3_DIJKSTRA_H
