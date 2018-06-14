//
// Created by Adam Bajguz on 2017-12-21.
//

#include "Graph.h"

static struct t_AdjecencyListNode * newAdjListNode(const uint16_t dest, const uint16_t weight) __attribute__((always_inline));

struct t_Graph * GraphInit(const uint16_t V) {
    register struct t_Graph *const graph = malloc(sizeof(*graph));

    graph->V = V;
    graph->adjecency_list = calloc(sizeof(*graph->adjecency_list), V);

    return graph;
}

struct t_AdjecencyListNode ** GraphAddEdge(struct t_Graph *const graph, const uint16_t src, const uint16_t dest, const uint16_t weight) {
    static struct t_AdjecencyListNode *src_dest[2];

    src_dest[0] = newAdjListNode(dest, weight);
    src_dest[0]->next = graph->adjecency_list[src].head;
    graph->adjecency_list[src].head = src_dest[0];

    src_dest[1] = newAdjListNode(src, weight);
    src_dest[1]->next = graph->adjecency_list[dest].head;
    graph->adjecency_list[dest].head = src_dest[1];

    return src_dest;
}

struct t_AdjecencyListNode * newAdjListNode(const uint16_t dest, const uint16_t weight) {
    register struct t_AdjecencyListNode *newNode = malloc(sizeof(*newNode));

    newNode->end_vertex = dest;
    newNode->time = weight;
    newNode->next = NULL;
    newNode->available = false;

    return newNode;
}