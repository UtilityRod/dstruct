#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include <dstruct_funcs.h>

typedef struct graph graph_t;

graph_t * graph_create(uint32_t verticies, compare_f compare);
void graph_destroy(graph_t * graph, destroy_f destroy_data);
void * graph_search(graph_t * graph, void * data);
int graph_add_node(graph_t * graph, void * data);
void * graph_remove_node(graph_t * graph, void * vertex);
int graph_update_edge(graph_t * graph, void * vertex1, void * vertex2, uint32_t weight);
int graph_remove_edge(graph_t * graph, void * vertex1, void * vertex2);
uint32_t graph_get_edge(graph_t * graph, void * vertex1, void * vertex2);
void graph_print_adj_matrix(graph_t * graph);

#endif