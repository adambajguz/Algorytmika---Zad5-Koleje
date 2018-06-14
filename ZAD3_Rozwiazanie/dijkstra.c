//
// Created by Adam Bajguz on 2017-12-21.
//

#include "dijkstra.h"

struct t_MinHeapNode {
    uint16_t vertex_number;
    uint16_t distance;
};

struct t_MinHeap {
    uint16_t size;
    uint16_t *position;
    struct t_MinHeapNode **array;
};

#define PTR_SWAP(a, b) do { void *tmp = a; \
                            a = b; \
                            b = tmp; } while(0)


static void min_heap_add(struct t_MinHeap *min_heap, const uint16_t index);

static void min_heap_decrease_key(struct t_MinHeap *min_heap, const uint16_t vertex_number, const uint16_t dist) __attribute__((always_inline));

uint16_t dijkstra(const struct t_Graph *const graph, const uint16_t start, const uint16_t end) {
    register const uint16_t V = graph->V;
    register uint16_t *const distance = malloc(sizeof(*distance) * V);

    register struct t_MinHeap *const min_heap = malloc(sizeof(*min_heap));
    min_heap->position = malloc(V * sizeof(*min_heap->position));
    min_heap->size = 0;
    min_heap->array = malloc(V * sizeof(*min_heap->array));

    for (uint16_t vertex_number = 0; vertex_number < V; ++vertex_number) {
        distance[vertex_number] = USHRT_MAX;

        min_heap->array[vertex_number] = malloc(sizeof(*min_heap->array[vertex_number]));
        min_heap->array[vertex_number]->vertex_number = vertex_number;
        min_heap->array[vertex_number]->distance = distance[vertex_number];

        min_heap->position[vertex_number] = vertex_number;
    }

    min_heap->size = V;
    min_heap->array[start]->vertex_number = start;
    min_heap->array[start]->distance = distance[start];
    min_heap->position[start] = start;

    distance[start] = 0;

    min_heap_decrease_key(min_heap, start, distance[start]);

    register struct t_MinHeapNode *minHeapNode;
    register struct t_AdjecencyListNode *traverse;
    register uint16_t vertex_number, v;
    while (min_heap->size) {
        // get minimum node
        {
            struct t_MinHeapNode *const root = min_heap->array[0], *const last = min_heap->array[min_heap->size - 1];
            min_heap->array[0] = last;
            min_heap->position[root->vertex_number] = min_heap->size - 1;
            min_heap->position[last->vertex_number] = 0;

            --min_heap->size;

            min_heap_add(min_heap, 0);

            minHeapNode = root;
        }

        vertex_number = minHeapNode->vertex_number;

        traverse = graph->adjecency_list[vertex_number].head;
        while (traverse != NULL) {
            v = traverse->end_vertex;

            // if is in min-heap && ...
            if (min_heap->position[v] < min_heap->size && distance[vertex_number] != USHRT_MAX && traverse->available &&
                traverse->time + distance[vertex_number] + 5 < distance[v]) {
                distance[v] = traverse->time + distance[vertex_number] + 5;

                min_heap_decrease_key(min_heap, v, distance[v]);
            }
            traverse = traverse->next;
        }
    }

    const uint16_t distance_ret = distance[end] - 5;

    free(min_heap->position);

//    for (uint16_t vertex_num = 0; vertex_num < V; ++vertex_num)
//        free(min_heap->adjecency_list[vertex_num]);
    free(min_heap->array);

    free(min_heap);
    free(distance);

    return distance_ret;
}

void min_heap_add(struct t_MinHeap *min_heap, const uint16_t index) {
    register uint16_t smallest = index;
    register const uint16_t left = 2 * index + 1, right = left + 1;

    if (left < min_heap->size &&
        min_heap->array[left]->distance < min_heap->array[smallest]->distance)
        smallest = left;

    if (right < min_heap->size &&
        min_heap->array[right]->distance < min_heap->array[smallest]->distance)
        smallest = right;

    if (smallest != index) {
        min_heap->position[min_heap->array[smallest]->vertex_number] = index;
        min_heap->position[min_heap->array[index]->vertex_number] = smallest;

        PTR_SWAP(min_heap->array[smallest], min_heap->array[index]);

        min_heap_add(min_heap, smallest);
    }
}


void min_heap_decrease_key(struct t_MinHeap *min_heap, const uint16_t vertex_number, const uint16_t dist) {
    register uint16_t i = min_heap->position[vertex_number], i_half = (i - 1) / 2;

    min_heap->array[i]->distance = dist;

    while (i && min_heap->array[i]->distance < min_heap->array[i_half]->distance) {
        min_heap->position[min_heap->array[i]->vertex_number] = i_half;
        min_heap->position[min_heap->array[i_half]->vertex_number] = i;

        PTR_SWAP(min_heap->array[i], min_heap->array[i_half]);

        i = i_half;
        i_half = (i - 1) / 2;
    }
}
