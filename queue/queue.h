#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>
#include <dstruct_funcs.h>

typedef struct queue_ queue_t;

queue_t * queue_create(compare_f compare, destroy_f destroy);
void queue_destroy(queue_t * p_queue);
size_t queue_enqueue(queue_t * p_queue, void * p_data);
void * queue_dequeue(queue_t * p_queue);
size_t queue_get_size(queue_t * queue);
void * queue_search(queue_t * queue, void * data);
void * queue_find_nth(queue_t * queue, size_t idx);

#endif
