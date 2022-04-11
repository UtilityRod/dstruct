#ifndef _SET_H_
#define _SET_H_

#include <dstruct_funcs.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct set set_t;

set_t * set_create(size_t size, compare_f compare);
void set_destroy(set_t * set, destroy_f destroy);
int set_add(set_t * set, void * data);
void * set_remove(set_t * set, void * data);
bool set_contains(set_t * set, void * data);
size_t set_get_size(set_t * set);

#endif