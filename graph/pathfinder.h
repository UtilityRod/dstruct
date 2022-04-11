#ifndef _PATHFINDER_H_
#define _PATHFINDER_H_

#include <stddef.h>

typedef struct pathfinder pathfinder_t;

pathfinder_t * pathfinder_create(size_t size);
void pathfinder_destroy(pathfinder_t * pathfinder);
int pathfinder_load_matrix(pathfinder_t * pathfinder, int ** adj_matrix);
int pathfinder_find_paths(pathfinder_t * pathfinder, int vertex);
int pathfinder_get_path_weight(pathfinder_t * pathfinder, int end);

#endif