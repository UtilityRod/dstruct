#include <heap.h>
#include <stdlib.h>

typedef struct element
{
    size_t idx;
    void * key;
    void * data;
} element_t;

struct heap
{
    element_t ** data_array;
    compare_f compare;
    size_t size;
    size_t filled;
};

typedef enum {HEAP_NULL, KEY_NULL, DATA_NULL, ALLOCATION_ERROR, OK} error_t;

static void heapify(heap_t * heap, element_t * element);
static element_t * get_parent(element_t ** data_array, size_t child_index);
static void swap_nodes(element_t ** data_array, size_t parent_idx, size_t child_idx);

heap_t * heap_create(size_t size, compare_f compare)
{

    if ((0 == size) || (NULL == compare))
    {
        return NULL;
    }

    heap_t * heap = calloc(1, sizeof(*heap));

    if (heap != NULL)
    {
        heap->size = size;
        heap->compare = compare;
        heap->data_array = calloc(size, sizeof(element_t *));

        if (NULL == heap->data_array)
        {
            free(heap);
            heap = NULL;
        }

    }

    return heap;
}

void heap_destroy(heap_t * heap, destroy_f destroy_key, destroy_f destroy_data)
{
    if (heap != NULL)
    {
        for (size_t i = 0; i < heap->filled; i++)
        {
            element_t * tmp_element = heap->data_array[i];

            if (destroy_key != NULL)
            {
                destroy_key(tmp_element->key);
            }

            if (destroy_data != NULL)
            {
                destroy_data(tmp_element->data);
            }

            free(tmp_element);
        }

        free(heap->data_array);
        free(heap);
    }
}

int heap_insert(heap_t * heap, void * key, void * data)
{

    if (NULL == heap)
    {
        return HEAP_NULL;
    }
    else if (NULL == key)
    {
        return KEY_NULL;
    }
    else if (NULL == data)
    {
        return DATA_NULL;
    }
    else
    {
        element_t * element = calloc(1, sizeof(*element));

        if (NULL == element)
        {
            return ALLOCATION_ERROR;
        }

        element->key = key;
        element->data = data;
        element->idx = heap->filled;
        heap->data_array[element->idx] = element;
        heapify(heap, element);
        heap->filled++;
    }
    
    return OK;
}

void heap_print(heap_t * heap, print_f print)
{
    for (size_t i = 0; i < heap->filled; i++)
    {
        print(heap->data_array[i]->data);
    }
}

static void heapify(heap_t * heap, element_t * element)
{
    for (;;)
    {
        if (element->idx != 0)
        {
            element_t * parent = get_parent(heap->data_array, element->idx);
            int comparison = heap->compare(parent->key, element->key);

            if (comparison > 0)
            {
                // The new element is larger so swap data and key with the parent
                swap_nodes(heap->data_array, parent->idx, element->idx);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
}

static element_t * get_parent(element_t ** data_array, size_t child_index)
{
    int parent_index = (child_index - 1) / 2;
    return data_array[parent_index];
}

static int get_lchild_index(int parent_index)
{
    return (2 * parent_index) + 1;
}

static int get_rchild_index(int parent_index)
{
    return (2 * parent_index) + 2;
}

static void swap_nodes(element_t ** data_array, size_t parent_idx, size_t child_idx)
{
    // Swap the points in the data array
    element_t * tmp_element = data_array[parent_idx];
    data_array[parent_idx] = data_array[child_idx];
    data_array[child_idx] = tmp_element;
    // Update indexs for swapped nodes
    data_array[parent_idx]->idx = parent_idx;
    data_array[child_idx]->idx = child_idx;
}
// END OF SOURCE