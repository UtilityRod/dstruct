#ifndef _HEAP_H_
#define _HEAP_H_

typedef struct heap heap_t;

#include <stddef.h>
#include <dstruct_funcs.h>

heap_t * heap_create(size_t size, compare_f compare);
void heap_destroy(heap_t * heap, destroy_f destroy_key, destroy_f destroy_data);
int heap_insert(heap_t * heap, void * key, void * data);
void heap_print(heap_t * heap, print_f print);

#endif