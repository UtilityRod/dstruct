#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <heap.h>
#include <stdbool.h>
#include <pathfinder.h>

struct graph 
{
    uint32_t verticies;
    int ** adj_matrix;
    void ** data_array;
    uint32_t filled;
    compare_f compare;
};

typedef struct 
{
    void * data;
} node_t;

enum {DISTANCE, PREVIOUS, VISITED, UNVISITED, SIZE};

static void destroy_adj_matrix(int ** adj_matrix, uint32_t verticies);
static int ** create_adj_matrix(uint32_t verticies);
static int get_node_idx(graph_t * graph, void * data);
static int setup_dijkstra_arrays(graph_t * graph, int * arrays[SIZE]);
static void dijkstra(graph_t * graph, int start, int * arrays[SIZE]);
static int compare_weights(const void * arg1, const void * arg2);
static bool array_empty(int * array, uint32_t verticies);
static int get_array_index(int * array, uint32_t verticies, int data);

graph_t * graph_create(uint32_t verticies, compare_f compare)
{

    if ((0 == verticies) || (NULL == compare))
    {
        return NULL;
    }

    graph_t * graph = calloc(1, sizeof(*graph));

    if (NULL == graph)
    {
        return NULL;
    }

    graph->compare = compare;
    graph->verticies = verticies;
    graph->data_array = calloc(verticies, sizeof(*(graph->data_array)));

    if (NULL == graph->data_array)
    {
        free(graph);
        return NULL;
    }

    graph->adj_matrix = create_adj_matrix(verticies);

    if (NULL == graph->adj_matrix)
    {
        // Could not allocate memory for adjacency matrix
        free(graph->data_array);
        free(graph);
        graph = NULL;
    }

    return graph;
}

void graph_destroy(graph_t * graph, destroy_f destroy_data)
{
    if (NULL == graph)
    {
        return;
    }

    destroy_adj_matrix(graph->adj_matrix, graph->verticies);
    graph->adj_matrix = NULL;
    
    if (destroy_data != NULL)
    {
       for (uint32_t i = 0; i < graph->filled; i++)
        {
            if (graph->data_array[i] != NULL)
            {
                destroy_data(graph->data_array[i]);
            }
        } 
    }
    
    free(graph->data_array);
    graph->data_array = NULL;
    free(graph);
    return;
}

void * graph_search(graph_t * graph, void * data)
{
    void * return_data = NULL;
    if ((NULL == graph) || (NULL == graph->data_array) || (NULL == data))
    {
        return NULL;
    }
    else
    {
        for (uint32_t i = 0; i < graph->filled; i++)
        {
            // Search each node in the data array for the one you are looking for
            if (NULL == graph->data_array[i])
            {
                continue;
            }

            int comparison = graph->compare(graph->data_array[i], data);

            if (comparison == 0)
            {
                return_data = graph->data_array[i];
                break;
            }
        }
    }

    return return_data;
}

int graph_add_node(graph_t * graph, void * data)
{
    void * search_data = NULL;
    if ((NULL == graph) || (NULL == graph->data_array))
    {
        return STRUCTURE_NULL;
    }
    else if (graph->filled == graph->verticies)
    {
        return STRUCTURE_FULL;
    }
    else if (NULL == data)
    {
        return DATA_NULL;
    }
    else
    {
        // Search for the data inside of the graph
        search_data = graph_search(graph, data);
    }

    if (search_data != NULL)
    {
        // Data has already been inserted into the graph
        return KEY_EXISTS;
    }

    int vertex_idx = -1;
    for (uint32_t i = 0; i < graph->verticies; i++)
    {
        if (graph->data_array[i] == NULL)
        {
            graph->data_array[i] = data;
            graph->filled++;
            vertex_idx = i;
            break;
        }
    }

    graph->adj_matrix[vertex_idx][vertex_idx] = 0;
    
    for (uint32_t i = 0; i < graph->verticies; i++)
    {
        if (graph->adj_matrix[i][i] != -1)
        {
            graph->adj_matrix[i][vertex_idx] = 0;
            graph->adj_matrix[vertex_idx][i] = 0;
        }
        
    }

    return OK;
}

void * graph_remove_node(graph_t * graph, void * vertex)
{
    void * return_data = NULL;
    if ((graph != NULL) && (graph->data_array != NULL) && (vertex != NULL))
    {
        int vertex_idx = get_node_idx(graph, vertex);

        if (vertex_idx != KEY_ERROR)
        {
            for (uint32_t i = 0; i < graph->verticies; i++)
            {
                graph->adj_matrix[i][vertex_idx] = -1;
                graph->adj_matrix[vertex_idx][i] = -1;
            }

            return_data = graph->data_array[vertex_idx];
            graph->data_array[vertex_idx] = NULL;
            graph->filled--;
        }
    }

    return return_data;
}

int graph_update_edge(graph_t * graph, void * vertex1, void * vertex2, uint32_t weight)
{
    if ((NULL == graph) || (NULL == graph->data_array))
    {
        return STRUCTURE_NULL;
    }
    else if ((NULL == vertex1) || (NULL == vertex2))
    {
        return DATA_NULL;
    }
    else
    {
        int vertex1_idx = get_node_idx(graph, vertex1);
        int vertex2_idx = get_node_idx(graph, vertex2);

        if ((KEY_ERROR == vertex1_idx) || (KEY_ERROR == vertex2_idx))
        {
            return KEY_ERROR;
        }
        else if (vertex1_idx == vertex2_idx)
        {
            return KEY_ERROR;
        }
        else
        {
            graph->adj_matrix[vertex1_idx][vertex2_idx] = weight;
            graph->adj_matrix[vertex2_idx][vertex1_idx] = weight;
        }

    }

    return OK;
}

int graph_remove_edge(graph_t * graph, void * vertex1, void * vertex2)
{
    if ((NULL == graph) || (NULL == graph->data_array))
    {
        return STRUCTURE_NULL;
    }
    else if ((NULL == vertex1) || (NULL == vertex2))
    {
        return DATA_NULL;
    }
    else
    {
        int vertex1_idx = get_node_idx(graph, vertex1);
        int vertex2_idx = get_node_idx(graph, vertex2);

        if ((KEY_ERROR == vertex1_idx) || (KEY_ERROR == vertex2_idx))
        {
            return KEY_ERROR;
        }
        else if (vertex1_idx == vertex2_idx)
        {
            return KEY_ERROR;
        }
        else
        {
            graph->adj_matrix[vertex1_idx][vertex2_idx] = 0;
        }

    }

    return OK;
}

uint32_t graph_get_edge(graph_t * graph, void * vertex1, void * vertex2)
{
    uint32_t weight = KEY_ERROR;
    if ((NULL == graph) || (NULL == graph->data_array))
    {
        return STRUCTURE_NULL;
    }
    else if ((NULL == vertex1) || (NULL == vertex2))
    {
        return DATA_NULL;
    }
    else
    {
        int vertex1_idx = get_node_idx(graph, vertex1);
        int vertex2_idx = get_node_idx(graph, vertex2);

        if ((KEY_ERROR != vertex1_idx) || (KEY_ERROR != vertex2_idx))
        {
            weight = graph->adj_matrix[vertex1_idx][vertex2_idx];
        }
        
    }

    return weight;
}

void graph_print_adj_matrix(graph_t * graph)
{
    for (uint32_t i = 0; i < graph->verticies; i++)
    {
        for (uint32_t j = 0; j < graph->verticies; j++)
        {
            printf("%d ", graph->adj_matrix[i][j]);
        }
        printf("\n");
    }
}

int graph_get_path_weight(graph_t * graph, void * start, void * end)
{
    pathfinder_t * pathfinder = pathfinder_create(graph->filled);
    pathfinder_load_matrix(pathfinder, graph->adj_matrix);
    int start_index = get_node_idx(graph, start);
    pathfinder_find_paths(pathfinder, start_index);
    int end_index = get_node_idx(graph, end);
    int path_weight = pathfinder_get_path_weight(pathfinder, end_index);
    pathfinder_destroy(pathfinder);
    return path_weight;
}

static int ** create_adj_matrix(uint32_t verticies)
{
    int ** matrix = calloc(verticies, sizeof(int *));

    if (matrix != NULL)
    {
        // Memory allocated for the matrix
        for (uint32_t i = 0; i < verticies; i++)
        {
            // Allocate memory for each row in the adjacency matrix
            int * tmp_array = calloc(verticies, sizeof(int));
            if (NULL == tmp_array)
            {
                // Failure to allocate all memory, destroy what has been allocated and return
                destroy_adj_matrix(matrix, verticies);
                matrix = NULL;
                break;
            }

            memset(tmp_array, -1, sizeof(int) * verticies);
            matrix[i] = tmp_array;
        }
        printf("\n\n");
    }
    
    return matrix;
}

static void destroy_adj_matrix(int ** adj_matrix, uint32_t verticies)
{
    for (uint32_t i = 0; i < verticies; i++)
    {
        // For each row in the matrix
        if (adj_matrix[i] != NULL)
        {
            // Free the row
            free(adj_matrix[i]);
        }
    }
    // Free the matrix
    free(adj_matrix);
}

static int get_node_idx(graph_t * graph, void * data)
{
    int idx = KEY_ERROR;
    if ((graph != NULL) && (graph->data_array != NULL) && (data != NULL))
    {
        // Find the first node that matches the search data
        for (uint32_t i = 0; i < graph->verticies; i++)
        {
            // Search each node in the data array for the one you are looking for
            if (NULL == graph->data_array[i])
            {
                continue;
            }

            int comparison = graph->compare(graph->data_array[i], data);

            if (comparison == 0)
            {
                // Found node in graph
                // Set idx and break out of for loop
                idx = i;
                break;
            }
        }
    }

    return idx;
}
// END OF SOURCE