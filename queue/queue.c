#include <queue.h>
#include <circular_ll.h>
#include <stdio.h>
#include <stdlib.h>

struct queue_ {
    circular_list_t * list;
    size_t size;
};

queue_t * queue_create(compare_f compare, destroy_f destroy)
{
    queue_t * queue = calloc(sizeof(*queue), 1);

    if (queue == NULL)
    {
        perror("Queue memeory allocation.");
    }
    else
    {
        queue->list = circular_create(compare, destroy);
    }

    return queue;
}

void queue_destroy(queue_t * queue)
{
    if (queue != NULL && queue->list)
    {

        if (queue->list)
        {
           circular_destroy(queue->list); 
        }
        
        free(queue);
    }
}

size_t queue_enqueue(queue_t * queue, void * data)
{

    if (queue && queue->list)
    {
        queue->size = circular_insert(queue->list, data, BACK);
    }

    return queue->size;
}

void * queue_dequeue(queue_t * queue)
{
    void * data = NULL;

    if (queue->list)
    {
        data = circular_remove_at(queue->list, FRONT);
    }

    if (data != NULL)
    {
        queue->size--;
    }

    return data;
}

size_t queue_get_size(queue_t * queue)
{
    return queue->size;
}

void * queue_search(queue_t * queue, void * data)
{
    void * return_data = NULL;
    if ((queue != NULL) && (queue->list != NULL) && (data != NULL))
    {
        return_data = circular_search(queue->list, data);
    }
    
    return return_data;
}

void * queue_find_nth(queue_t * queue, size_t idx)
{
    void * return_data = NULL;
    if ((queue != NULL) && (queue->list != NULL) && (idx < queue->size))
    {
        return_data = circular_get_data(queue->list, idx);
    }

    return return_data;
}
// END OF SOURCE