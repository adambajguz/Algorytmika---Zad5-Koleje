#ifndef PS5_ZAD3_GRAPH_H
#define PS5_ZAD3_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "input.h"

struct t_AdjecencyListNode {
    uint16_t end_vertex;
    uint16_t time;
    struct t_AdjecencyListNode *next;

    bool available;

    struct t_record *conn_build_record, *conn_last_record;
};

struct t_AdjecencyList {
    struct t_AdjecencyListNode *head;
};

struct t_Graph {
    uint16_t V;
    struct t_AdjecencyList *adjecency_list;
};

struct t_Graph * GraphInit(const uint16_t V);

struct t_AdjecencyListNode ** GraphAddEdge(struct t_Graph *const graph, const uint16_t src, const uint16_t dest, const uint16_t weight);

#endif //PS5_ZAD3_GRAPH_H
