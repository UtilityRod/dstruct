#include <priority_queue.h>
#include <heap.h>
#include <stdlib.h>
#include <stdio.h>

struct priority_queue
{
    heap_t * heap;
};

priority_queue_t * priority_queue_create(size_t size, compare_f compare)
{
    priority_queue_t * queue = calloc(1, (sizeof(*queue)));

    if (queue != NULL)
    {
        queue->heap = heap_create(size, compare);
        
        if (NULL == queue->heap)
        {
            free(queue);
            queue = NULL;
        }
    }

    return queue;
}

void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy)
{

    if (queue != NULL)
    {
        if (queue->heap != NULL)
        {
            heap_destroy(queue->heap, NULL, destroy);   
        }
        
        free(queue);
        return;
    }
}