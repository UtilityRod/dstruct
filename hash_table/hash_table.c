#include <hash_table.h>
#include <circular_ll.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    void * data;
    int hash;
} element_t;

struct hash_table 
{
    circular_list_t ** data_array;
    hash_function_f hash;
    size_t size;
    size_t filled;
};

typedef enum {KEY_IN_TABLE = -4, TABLE_NULL, KEY_NULL, ALLOCATION_ERROR} table_error_t;

static int hash_compare(const void * arg1, const void * arg2);

hash_table_t * table_create(size_t size, hash_function_f hash)
{

    if (NULL == hash)
    {
        // Hash function must be provided
        // Type must either be integer or variable
        return NULL;
    }


    hash_table_t * table = calloc(1, sizeof(*table));

    if(table != NULL)
    {
        // Memory allocated for table properly
        table->data_array = calloc(size, sizeof(circular_list_t **));

        if (NULL == table->data_array)
        {
            // Error allocating memory for data array
            free(table);
            table = NULL;
        }
        else
        {
            table->hash = hash;
            table->size = size;
        }
    }

    return table;
}

void table_destroy(hash_table_t * table, destroy_f value_destroy)
{
    if (NULL == table)
    {
        return;
    }

    if (NULL == table->data_array)
    {
        free(table);
        return;
    }

    for (size_t i = 0; i < table->size; i++)
    {
        // Free all the element data find in each data array in the table.
        while (circular_get_size(table->data_array[i]) != 0)
        {
            // There are elements in the list, remove from the front
            element_t * element = circular_remove_at(table->data_array[i], FRONT);
            // If the value destroy function was passed through, call with the element's data
            if (value_destroy != NULL)
            {
                value_destroy(element->data);
            }
            // Free memory allocated for the element
            free(element);
        }

        circular_destroy(table->data_array[i]);
    }

    free(table->data_array);
    free(table);
    return;
}

size_t table_insert(hash_table_t * table, void * key, void * value)
{
    int hash = table->hash(key);
    int idx = hash % table->size;

    if (NULL == table->data_array[idx])
    {
        table->data_array[idx] = circular_create(hash_compare, NULL);

        if (NULL == table->data_array[idx])
        {
            // Could not allocate memory for data_array[idx]
            return ALLOCATION_ERROR;
        }
    }

    element_t * element = calloc(1, sizeof(*element));

    if (NULL == element)
    {
        // Element could not be allocated return current filled slots in table
        return ALLOCATION_ERROR;
    }

    element->data = value;
    element->hash =hash;
    circular_insert(table->data_array[idx], element, FRONT);
    table->filled++;
    
    return table->filled;
}

void * table_search(hash_table_t * table, void * key)
{
    if (NULL == table)
    {
        return NULL;
    }
    
    if (NULL == key)
    {
        return NULL;
    }

    int hash = table->hash(key);
    int idx = hash % table->size;
    element_t search_element = { .hash = hash };
    return circular_search(table->data_array[idx], &search_element);
}

static int hash_compare(const void * arg1, const void * arg2)
{
    element_t * element1 = (element_t *)arg1;
    element_t * element2 = (element_t *)arg2;

    return element2->hash - element1->hash;
}
// END OF SOURCE