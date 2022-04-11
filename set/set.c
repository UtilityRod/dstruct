#include <set.h>
#include <stdlib.h>

struct set
{
    void ** array;
    size_t size;
    size_t filled;
    compare_f compare;
};

set_t * set_create(size_t size, compare_f compare)
{
    if ((0 == size) || (NULL == compare))
    {
        return NULL;
    }

    set_t * set = calloc(1, sizeof(*set));

    if (set != NULL)
    {
        set->size = size;
        set->compare = compare;
        set->array = calloc(size, sizeof(void **));

        if (NULL == set->array)
        {
            free(set);
            set = NULL;
        }
    }

    return set;
}

void set_destroy(set_t * set, destroy_f destroy)
{
    if (NULL == set)
    {
        return;
    }

    if (NULL == set->array)
    {
        free(set);
        return;
    }

    if (destroy != NULL)
    {
        for (size_t i = 0; i < set->size; i++)
        {
            if (set->array[i] != NULL)
            {
                destroy(set->array[i]);
            }
        }
    }

    free(set->array);
    free(set);
}

int set_add(set_t * set, void * data)
{
    if ((NULL == set) || (NULL == data))
    {
        return DATA_ERROR;
    }

    bool contains = set_contains(set, data);

    if (true == contains)
    {
        return KEY_EXISTS;
    }

    int return_value = STRUCTURE_FULL;
    for (size_t i = 0; i < set->size; i++)
    {
        if (NULL == set->array[i])
        {
            set->array[i] = data;
            return_value = OK;
            set->filled++;
            break;
        }
    }

    return return_value;
}

void * set_remove(set_t * set, void * data)
{
    if ((NULL == set) || (NULL == data))
    {
        return NULL;
    }

    void * return_value = NULL;
    for (size_t i = 0; i < set->size; i++)
    {
        int comparison = set->compare(data, set->array[i]);
        if (comparison == 0)
        {
            return_value =  set->array[i];
            set->array[i] = NULL;
            set->filled--;
            break;
        }
    }
}

bool set_contains(set_t * set, void * data)
{
    if ((NULL == set) || (NULL == data))
    {
        return NULL;
    }

    bool return_value = false;
    for (size_t i = 0; i < set->size; i++)
    {
        if (set->array[i] == NULL)
        {
            continue;
        }

        int comparison = set->compare(data, set->array[i]);
        if (comparison == 0)
        {
            return_value =  true;
        }
    }

    return return_value;
}

size_t set_get_size(set_t * set)
{
    if (set != NULL)
    {
        return set->filled;
    }
}
// END OF SOURCE