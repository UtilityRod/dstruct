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

typedef enum {HEAP_NULL, KEY_NULL, DATA_NULL, ALLOCATION_ERROR, OK, HEAP_EMPTY} error_t;

static void heapify_insert(heap_t * heap, element_t * element);
static void heapify_extract(heap_t * heap, element_t * element);
static element_t * get_parent(element_t ** data_array, size_t child_index);
static element_t * get_lchild(heap_t * heap, size_t idx);
static element_t * get_rchild(heap_t * heap, size_t idx);
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
        heapify_insert(heap, element);
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

void * heap_extract(heap_t * heap, destroy_f destroy_key)
{
    if ((NULL == heap) || (0 == heap->filled))
    {
        return NULL;
    }

    element_t * return_element = heap->data_array[0];
    element_t * new_root = heap->data_array[heap->filled - 1];
    heap->data_array[heap->filled - 1] = NULL;
    new_root->idx = 0;
    heap->data_array[0] = new_root;
    heapify_extract(heap, new_root);

    if (destroy_key != NULL)
    {
        destroy_key(return_element->key);
    }

    void * return_data = return_element->data;
    free(return_element);
    heap->filled--;
    return return_data;
}

static void heapify_insert(heap_t * heap, element_t * element)
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

static void heapify_extract(heap_t * heap, element_t * element)
{
    for (;;)
    {
        element_t * max_child = NULL;
        element_t * lchild = get_lchild(heap, element->idx);
        if (lchild != NULL)
        {
            // If there is a left child mark it as the max originally
            max_child = lchild;
        }

        element_t * rchild = get_rchild(heap, element->idx);
        if (rchild != NULL)
        {
            // If there is a right child, there must be a left child
            // per the rules of a array heap, assume there is a left child
            // Check to see which child is larger between the left and the right
            int comparison = heap->compare(rchild->data, max_child->data); 
            if (comparison < 0)
            {
                // Right child is larger than left child.
                max_child = rchild;
            }
        }

        if (max_child != NULL)
        {
            int comparison = heap->compare(max_child->data, element->data);
            if (comparison < 0)
            {
                swap_nodes(heap->data_array, element->idx, max_child->idx);
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

static element_t * get_lchild(heap_t * heap, size_t idx)
{
    int child_idx = (2 * idx) + 1;
    return child_idx >= heap->filled ? NULL : heap->data_array[child_idx];
}

static element_t * get_rchild(heap_t * heap, size_t idx)
{
    int child_idx = (2 * idx) + 2;
    return child_idx >= heap->filled ? NULL : heap->data_array[child_idx];
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