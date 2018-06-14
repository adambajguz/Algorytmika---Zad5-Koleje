//
// Created by Adam Bajguz on 2017-12-21.
//

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "input.h"
#include "dijkstra.h"

#define PRINT_FILE_INFO 0 // 0 or 1
#define PRINT_GRAPH 0 // 0 or 1

#define NAIVE_ALGORITHM 0

#define FILE_ID "_sredni3"

#define MAKE_FILENAME(name) name FILE_ID ".txt"
#define INPUT_FILENAME MAKE_FILENAME("in")
#define OUTPUT_FILENAME "out.txt"

static void graph_activate(const struct t_data *const data, uint32_t start, uint32_t end) {
    for (uint32_t i = start; i <= end; ++i) {
        data->record[i].edge_to_start->available = data->record[i].edge_to_end->available = true;
        data->record[i].edge_to_start->time = data->record[i].edge_to_end->time = data->record[i].time;
    }
}

static void graph_deactivate(const struct t_data *const data, uint32_t start, uint32_t end) {
    for (int32_t i = end; i > start; --i) {
        if (data->record[i].type == 'b')
            data->record[i].edge_to_start->available = data->record[i].edge_to_end->available = false;
        else
            data->record[i].edge_to_start->time = data->record[i].edge_to_end->time = data->record[i].prev->time;
    }
}

static void graph_prepare_half(const struct t_data *const data) {
    graph_deactivate(data, (data->records - 1) / 2, data->records - 1);
    for (uint32_t i = 0; i < data->records / 2; ++i) {
        data->record[i].edge_to_start->available = data->record[i].edge_to_end->available = true;
        data->record[i].edge_to_start->time = data->record[i].edge_to_end->time = data->record[i].time;
    }
}

int main() {
    struct t_data data;
    struct t_Graph *const graph = read_file(&data, INPUT_FILENAME);

#if PRINT_FILE_INFO == 1
#if PRINT_GRAPH == 1
    printf("GRAPH STRUCTURE:\n");
    for (uint16_t i = 1; i < graph->V; ++i) {
        printf(" %5d: ", i);

        struct t_AdjecencyListNode *node = graph->adjecency_list[i].head;
        while (node) {
            printf("(%d, ", node->end_vertex);
            printf("%d, ", node->time);
            printf("%s)\t", node->available ? "T" : "F")        ;

            node = node->next;
        }
        printf("\n");
    }

    printf("\n\nCONNECTIONS:\n");
    printf("    Date    |  Type  | Start |  End  | Distance [km] | Time [min] \n");
    printf("------------+--------+-------+-------+---------------+------------\n");
    for (uint32_t i = 0; i < data.records; ++i)
        printf(" %s | %s | %5d | %5d |           %3d |       %4d\n", data.record[i].date,
               data.record[i].type == 'b' ? " build" : "   mod", data.record[i].start, data.record[i].end,
               data.record[i].distance, data.record[i].time);
#endif
    printf("\n\nQUERIES:\n");
    printf(" Start |  End  | Time max [min] \n");
    printf("-------+-------+----------------\n");
    for (uint8_t i = 0; i < data.queries; ++i)
        printf(" %5d | %5d |          %5d \n", data.query[i].start, data.query[i].end, data.query[i].max_minutes);
    printf("\n");
#endif

	{
        FILE *out = fopen(OUTPUT_FILENAME, "wt");

        clock_t exec_begin_algo = clock();
		register uint16_t answer, max_minutes;
		uint32_t start, pivot, end;

		for (uint8_t i = 0; i < data.queries; ++i) {
			max_minutes = data.query[i].max_minutes;

			graph_prepare_half(&data);

			start = 0, pivot = (data.records - 1) / 2, end = data.records - 1;
			while(end - start > 1) {
				answer = dijkstra(graph, data.query[i].start, data.query[i].end);

                if(answer == max_minutes)
                    break;
				if (answer < max_minutes) {
					end = pivot;
                    pivot = (start + end) / 2;

					graph_deactivate(&data, pivot, end);
				}

				if (answer > max_minutes) {
					start = pivot;
                    pivot = (start + end) / 2;

					graph_activate(&data, start, pivot);
				}
			}

            graph_deactivate(&data, 0, data.records - 1);
            graph_activate(&data, 0, end);

            answer = dijkstra(graph, data.query[i].start, data.query[i].end);

            fprintf(out, "%s\n", (answer <= data.query[i].max_minutes) ? data.record[end].date : "NIE");
            printf("%s\n", (answer <= data.query[i].max_minutes) ? data.record[end].date : "NIE");

//            printf("%d: %s %d END=%d\n", i, (answer <= data.query[i].max_minutes) ? data.record[end].date : "NIE", answer, end);
		}
		clock_t exec_end_algo = clock();
        fclose(out);

		double exec_time_algo = ((double) (exec_end_algo - exec_begin_algo)) / CLOCKS_PER_SEC;
		printf("   Execution time without operations on file: %f seconds.\n\n", exec_time_algo);
	}


#if NAIVE_ALGORITHM == 1
    {
        FILE *out = fopen(OUTPUT_FILENAME, "wt");
        clock_t exec_begin_algo = clock();

        register uint32_t end;
        register uint16_t answer, max_minutes;
        for (uint8_t i = 0; i < data.queries; ++i) {
            graph_deactivate(&data, 0, data.records - 1);
            max_minutes = data.query[i].max_minutes;

            for (uint32_t j = 0; j < data.records; ++j) {
                graph_activate(&data, j, j);

                answer = dijkstra(graph, data.query[i].start, data.query[i].end);
                end = j;

                if (answer <= max_minutes)
                    break;
            }
            fprintf(out, "%s %d END=%d\n", answer <= data.query[i].max_minutes ? data.record[end].date : "NIE",
                    answer,
                    end);
            printf("%d: %s %d END=%d\n", i, answer <= data.query[i].max_minutes ? data.record[end].date : "NIE",
                   answer,
                   end);
        }
        clock_t exec_end_algo = clock();
        fclose(out);

        double exec_time_algo = ((double) (exec_end_algo - exec_begin_algo)) / CLOCKS_PER_SEC;
        printf("   Execution time without operations on file: %f seconds.\n", exec_time_algo);
    }
#endif

    return 0;
}
