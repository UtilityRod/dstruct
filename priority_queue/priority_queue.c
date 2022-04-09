#include <priority_queue.h>
#include <heap.h>
#include <stdlib.h>

struct priority_queue
{
    heap_t * heap;
};

typedef struct key 
{
    int priority;
} queue_key_t;

static void destroy_key(void *arg);

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

void priority_queue_destroy(priority_queue_t * queue, destroy_f destroy_value)
{

    if (queue != NULL)
    {
        if (queue->heap != NULL)
        {
            heap_destroy(queue->heap, destroy_key, destroy_value);   
        }
        
        free(queue);
        return;
    }
}

int priority_queue_insert(priority_queue_t * queue, void * data, priority_t priority)
{
    queue_key_t * key = calloc(1, sizeof(*key));

    if (NULL == key)
    {
        return ALLOCATION_ERROR;
    }

    key->priority = priority;

    int insert_check = heap_insert(queue->heap, key, data);

    if (insert_check < 0)
    {
        free(key);
    }

    return insert_check;
}

void * priority_queue_extract(priority_queue_t * queue)
{
    return heap_extract(queue->heap, destroy_key);
}

static void destroy_key(void *arg)
{
    queue_key_t * key = (queue_key_t *)arg;
    free(key);
}
// END OF SOURCE