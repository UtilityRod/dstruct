#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <dstruct_funcs.h>
#include <stddef.h>

typedef struct priority_queue priority_queue_t;

typedef enum {LOW, MEDIUM, HIGH, CRITICAL, PRIORITY_SZ} priority_t;

priority_queue_t * priority_queue_create(size_t size);
void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy);
int priority_queue_insert(priority_queue_t * queue, void * data, priority_t priority);
void * priority_queue_extract(priority_queue_t * queue);

#endif