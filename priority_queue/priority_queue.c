#include <priority_queue.h>
#include <heap.h>
#include <stdlib.h>
#include <string.h>

struct priority_queue
{
    heap_t * heap;
    priority_t priority[PRIORITY_SZ];
};

typedef struct key 
{
    int priority;
} queue_key_t;

static int compare(const void * arg1, const void * arg2);

priority_queue_t * priority_queue_create(size_t size)
{
    priority_queue_t * queue = calloc(1, (sizeof(*queue)));

    if (queue != NULL)
    {
        priority_t tmp[] = {LOW, MEDIUM, HIGH, CRITICAL};
        memcpy(queue->priority, &tmp, sizeof(int) * PRIORITY_SZ);
        queue->heap = heap_create(size, compare);
        
        if (NULL == queue->heap)
        {
            free(queue);
            queue = NULL;
        }
    }

    return queue;
}

void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy_value)
{

    if (queue != NULL)
    {
        if (queue->heap != NULL)
        {
            heap_destroy(queue->heap, NULL, destroy_value);   
        }
        
        free(queue);
        return;
    }
}

int priority_queue_insert(priority_queue_t * queue, void * data, priority_t priority)
{

    priority_t * key = NULL;
    switch (priority)
    {
        case CRITICAL:
            key = &(queue->priority[CRITICAL]);
            break;
        case HIGH:
            key = &(queue->priority[HIGH]);
            break;
        case MEDIUM:
            key = &(queue->priority[MEDIUM]);
            break;
        case LOW:
            key = &(queue->priority[LOW]);
            break;
        default:
            return KEY_ERROR;
    }

    return heap_insert(queue->heap, key, data);
}

void * priority_queue_extract(priority_queue_t * queue)
{
    return heap_extract(queue->heap, NULL);
}

static int compare(const void * arg1, const void * arg2)
{
    priority_t * priority1 = (priority_t *)arg1;
    priority_t * priority2 = (priority_t *)arg2;

    return *priority2 - *priority1;
}
// END OF SOURCE