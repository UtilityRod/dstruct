#ifndef _CIRCULAR_LL_H_
#define _CIRCULAR_LL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dstruct_funcs.h>

// Circular Linked List
typedef struct circular_list_ circular_list_t;
typedef enum location_ {FRONT = -1, BACK = -2}location_t;

circular_list_t * circular_create(compare_f compare, destroy_f destroy);
void circular_destroy(circular_list_t * p_list);
size_t circular_insert(circular_list_t * p_list, void * p_data, location_t location);
void * circular_search(circular_list_t * p_list, void * p_data);
void * circular_get_data(circular_list_t * list, uint64_t idx);
void * circular_remove_at(circular_list_t * p_list, location_t location);
void circular_ll_sort(circular_list_t * p_list);
ssize_t circular_get_size(circular_list_t * list);
void * circular_update_nth(circular_list_t * list, size_t idx, void * data);
#endif
