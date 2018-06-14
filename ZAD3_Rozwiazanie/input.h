#ifndef PS5_ZAD3_INPUT_H
#define PS5_ZAD3_INPUT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "Graph.h"



#define DATE_SIZE 11

struct t_record {
    char date[DATE_SIZE];
    char type;
    uint16_t start, end;
    uint16_t distance, time;

    struct t_AdjecencyListNode *edge_to_end, *edge_to_start;

    struct t_record *prev, *next;
};

struct t_query {
    uint16_t start, end;
    uint16_t max_minutes;
};

struct t_data {
    uint16_t cities;
    uint32_t records;
    uint8_t queries;

    struct t_record *record;
    struct t_query *query;
};

struct t_Graph *read_file(struct t_data *const data, const char filename[]);

#endif //PS5_ZAD3_INPUT_H
