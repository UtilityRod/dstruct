#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <dstruct_funcs.h>
#include <stddef.h>

typedef struct priority_queue priority_queue_t;

typedef enum {CRITICAL, HIGH, MEDIUM, LOW} priority_t;

priority_queue_t * priority_queue_create(size_t size, compare_f compare);
void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy);
int priority_queue_insert(priority_queue_t * queue, void * data, priority_t priority);
void * priority_queue_extract(priority_queue_t * queue);

#endif