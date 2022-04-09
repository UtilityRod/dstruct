#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <dstruct_funcs.h>
#include <stddef.h>

typedef struct priority_queue priority_queue_t;

priority_queue_t * priority_queue_create(size_t size, compare_f compare);
void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy);

#endif