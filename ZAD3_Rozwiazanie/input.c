//
// Created by Adam Bajguz on 2018-01-05.
//

#include "input.h"

struct t_Graph *read_file(struct t_data *const data, const char filename[]) {
    FILE *in = fopen(filename, "rt");
    if (!in) {
        printf("Error reading from %s!", filename);
        exit(1);
    }

    fscanf(in, " %hu %u %hhu ", &data->cities, &data->records, &data->queries);

    register struct t_Graph *graph = GraphInit(data->cities + 1);

    data->record = malloc(data->records * sizeof(*data->record));
    data->query = malloc(data->queries * sizeof(*data->query));
    if (!data->record || !data->query) {
        printf("Error allocating memory!");
        exit(2);
    }

    register struct t_AdjecencyListNode ** end_start;
    register struct t_record *curr = data->record, *conn_build_record;

    uint16_t temp_speed;
    for (register uint32_t i = 0; i < data->records; ++i, ++curr) {
        while (!fgets(curr->date, DATE_SIZE, in));

        fscanf(in, " %c %hd %hd %hu", &curr->type, &curr->start, &curr->end, &temp_speed);
        if (curr->type == 'b') {
            fscanf(in, " %hu ", &curr->distance);

            curr->prev = curr->next = NULL;

            curr->time = (curr->distance * 60) / temp_speed;
            end_start = GraphAddEdge(graph, curr->start, curr->end, curr->time);

            curr->edge_to_end = end_start[0];
            curr->edge_to_start = end_start[1];

            end_start[0]->conn_build_record = end_start[1]->conn_build_record = curr;
            end_start[0]->conn_last_record = end_start[1]->conn_last_record = curr;

            if (i < data->records / 2)
                end_start[0]->available = end_start[1]->available = true;
        } else {
            struct t_AdjecencyListNode *edge_to_end = graph->adjecency_list[curr->start].head;
            while (edge_to_end && edge_to_end->end_vertex != curr->end) {
                edge_to_end = edge_to_end->next;
            }

            conn_build_record = edge_to_end->conn_build_record;

            curr->edge_to_end = edge_to_end; // OR curr->edge_to_end = conn_build_record->edge_to_end;
            curr->edge_to_start = conn_build_record->edge_to_start;

            curr->distance = conn_build_record->distance;

			curr->time = (curr->distance * 60) / temp_speed;

			if (i <= round((data->records - 1) / 2))
				curr->edge_to_start->time = edge_to_end->time = curr->time;

            curr->prev = edge_to_end->conn_last_record;
            curr->prev->next = curr;
            curr->next = NULL;

            edge_to_end->conn_last_record = curr;
            curr->edge_to_start->conn_last_record = curr;

            fgetc(in);
        }
    }

    for (register uint8_t i = 0; i < data->queries; ++i)
        fscanf(in, " %hu %hu %hu", &(data->query[i].start), &(data->query[i].end), &(data->query[i].max_minutes));

    fclose(in);

    return graph;
}

