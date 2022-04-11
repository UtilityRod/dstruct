#include <pathfinder.h>
#include <dstruct_funcs.h>
#include <set.h>
#include <stdlib.h>
#include <dstruct_funcs.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

struct pathfinder
{
    int ** adj_matrix;
    int * verticies;
    int * distances;
    int * previous;
    int * path;
    set_t * visited;
    set_t * unvisited;
    size_t size;
    size_t filled;
};

static int compare_weights(const void * arg1, const void * arg2);
static int get_vertex_index(pathfinder_t * pathfinder, int vertex);
static int find_nth_smallest(pathfinder_t * pathfinder, int n);
static int find_next_smallest_distance(pathfinder_t * pathfinder);

pathfinder_t * pathfinder_create(size_t size)
{
    if (0 == size)
    {
        return NULL;
    }

    pathfinder_t * pathfinder = calloc(1, sizeof(*pathfinder));

    if (NULL == pathfinder)
    {
        return NULL;
    }

    pathfinder->size = size;
    pathfinder->verticies = calloc(size, sizeof(*(pathfinder->verticies)));
    pathfinder->distances = calloc(size, sizeof(*(pathfinder->distances)));
    pathfinder->previous = calloc(size, sizeof(*(pathfinder->previous)));
    pathfinder->path = calloc(size, sizeof(*(pathfinder->path)));
    pathfinder->visited = set_create(size, compare_weights);
    pathfinder->unvisited = set_create(size, compare_weights);

    if ((NULL == pathfinder->verticies) || (NULL == pathfinder->distances) || 
        (NULL == pathfinder->previous) || (NULL == pathfinder->visited) || 
        (NULL == pathfinder->unvisited) || (NULL == pathfinder->path))
    {
        // If any of the sets or arrays were not properly allocated then destroy the pathfinder
        pathfinder_destroy(pathfinder);
        pathfinder = NULL;
    }

    memset(pathfinder->path, -1, size);
    return pathfinder;
}

void pathfinder_destroy(pathfinder_t * pathfinder)
{
    if (NULL == pathfinder)
    {
        return;
    }

    if (pathfinder->verticies != NULL)
    {
        free(pathfinder->verticies);
    }
    
    if (pathfinder->distances != NULL)
    {
        free(pathfinder->distances);
    }

    if (pathfinder->previous != NULL)
    {
        free(pathfinder->previous);
    }

    if (pathfinder->unvisited != NULL)
    {
        set_destroy(pathfinder->unvisited, NULL);
    }

    if (pathfinder->visited != NULL)
    {
        set_destroy(pathfinder->visited, NULL);
    }

    if (pathfinder->path != NULL)
    {
        free(pathfinder->path);
    }

    free(pathfinder);
    return;
}

int pathfinder_load_matrix(pathfinder_t * pathfinder, int ** adj_matrix)
{

    if ((NULL == pathfinder) || (NULL == adj_matrix))
    {
        return DATA_ERROR;
    }

    pathfinder->adj_matrix = adj_matrix;

    for (size_t i = 0; i < pathfinder->size; i++)
    {
        if (adj_matrix[i][i] != -1)
        {
            pathfinder->verticies[pathfinder->filled] = i;
            pathfinder->distances[pathfinder->filled] = INT_MAX;
            pathfinder->previous[pathfinder->filled] = -1;
            set_add(pathfinder->unvisited, &(pathfinder->verticies[pathfinder->filled]));
            pathfinder->filled++;
        }
    }

    return OK;
}

int pathfinder_find_paths(pathfinder_t * pathfinder, int start)
{
    if ((NULL == pathfinder) || (start < 0))
    {
        return DATA_ERROR;
    }
    
    if (start < 0)
    {
        return DATA_ERROR;
    }

    int vertex_idx = get_vertex_index(pathfinder, start);
    pathfinder->distances[vertex_idx] = 0;

    int size = set_get_size(pathfinder->unvisited);
    while (size != 0)
    {
        // Remove first node from unvisited node, and add to visited node
        int * tmp_vertex = set_remove(pathfinder->unvisited, &vertex_idx);
        size = set_get_size(pathfinder->unvisited);
        set_add(pathfinder->visited, tmp_vertex);
        int * path_weights = pathfinder->adj_matrix[vertex_idx];
        for (int i = 0; i < pathfinder->size; i++)
        {
            if (pathfinder->adj_matrix[vertex_idx][i] != 0)
            {
                // Calculate possible distance to a non zero node in the adj matrix
                int tmp_distance = pathfinder->distances[vertex_idx] + pathfinder->adj_matrix[vertex_idx][i];
                int tmp_vertex_idx = get_vertex_index(pathfinder, i);

                if (tmp_distance < pathfinder->distances[tmp_vertex_idx])
                {
                    pathfinder->distances[tmp_vertex_idx] = tmp_distance;
                    pathfinder->previous[tmp_vertex_idx] = vertex_idx;
                }
            }
        }
        
        vertex_idx = find_next_smallest_distance(pathfinder);
    }
}

int pathfinder_get_path_weight(pathfinder_t * pathfinder, int end)
{
    if ((NULL == pathfinder) || (end < 0) || (end >= pathfinder->size))
    {
        return DATA_ERROR;
    }

    return pathfinder->distances[end];
}

static int compare_weights(const void * arg1, const void * arg2)
{
    int * weight1 = (int *)arg1;
    int * weight2 = (int *)arg2;
    return *weight2 - *weight1;
}

static int get_vertex_index(pathfinder_t * pathfinder, int vertex)
{
    if ((NULL == pathfinder) || (vertex < 0))
    {
        return DATA_ERROR;
    }

    int vertex_idx = DATA_ERROR;
    for (int i = 0; i < pathfinder->size; i++)
    {
        if (vertex == pathfinder->verticies[i])
        {
            vertex_idx = i;
            break;
        }
    }

    return vertex_idx;
}

static int find_next_smallest_distance(pathfinder_t * pathfinder)
{
    int smallest_idx = -1;
    int smallest_distance = INT_MAX;
    for (int i = 0; i < pathfinder->size; i++)
    {
        bool contains = set_contains(pathfinder->visited, &(pathfinder->verticies[i]));
        if (true == contains)
        {
            continue;
        }
        else
        {
            //printf("Check: %d %d %d\n", i, pathfinder->distances[i], smallest_distance);
            if (pathfinder->distances[i] < smallest_distance)
            {
                smallest_idx = i;
                smallest_distance = pathfinder->distances[i];
            }
        }
    }
    return smallest_idx;
}
// END OF SOURCE