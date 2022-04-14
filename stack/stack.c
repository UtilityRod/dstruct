#include <stack.h>
#include <circular_ll.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct stack_ {
    circular_list_t * list;
    size_t size;
    size_t filled;
};

stack_t * stack_create(compare_f compare, destroy_f destroy)
{
    stack_t * stack = calloc(sizeof(*stack), 1);

    if (stack == NULL)
    {
        // Memory allocation for stack failed
        perror("Stack creation.");
    }
    else
    {
        // Call circular linked list function to create list
        stack->list = circular_create(compare, destroy);
    }

    return stack;
}

stack_t * stack_create_n(compare_f compare, destroy_f destroy, size_t size)
{
    stack_t * stack = calloc(1, sizeof(*stack));

    if (NULL == stack)
    {
        // Memory allocation failed for stack
        return NULL;
    }

    stack->list = circular_create(compare, destroy);

    if (NULL == stack->list)
    {
        // Memory allocation failed for stack's linked list
        free(stack);
        return NULL;
    }

    for (size_t i = 0; i < size; i++)
    {
        // Insert NULL data into the linked list as a placeholder for data later
        void * tmp_ptr = NULL;
        stack->size = circular_insert(stack->list, tmp_ptr, FRONT);
    }

    return stack;
}

void stack_destroy(stack_t * stack)
{
    if (stack && stack->list)
    {

        if (stack->list)
        {
            circular_destroy(stack->list);
        }

        free(stack);
    }
}

size_t stack_push(stack_t * stack, void * data)
{
    if ((NULL == stack) || (NULL == stack->list))
    {
        return 0;
    }

    // Check to see if the size of the linked list equals the amount filled in the stack
    if (stack->size == stack->filled)
    {
        // Linked list size is equal to the amount filled in the stack
        // This means there are no longer any place holders left in the linked list
        // Insert new node into linked list
        int tmp_size = circular_insert(stack->list, data, FRONT);

        if (stack->size < tmp_size)
        {
            // Insert succeeded if the tmp_size is greater than the stack size
            stack->size++;
            stack->filled++;
        }

    }
    else
    {
        // Linked list size is not equal to the amount filled in the stack
        // This means there are placeholders available in the linked list
        // Update a placeholder to hold data instead of insert into linked list 
        size_t node_index = stack->size - stack->filled - 1;
        circular_update_nth(stack->list, node_index, data);
        // Update the amount filled in the stack
        stack->filled++;
    }

    return stack->filled;
}

void * stack_pop(stack_t * stack)
{
    void * data = NULL;

    if (stack && stack->list)
    {
        data = circular_remove_at(stack->list, FRONT);
        stack->size--;
    }

    return data;
}

void * stack_find_nth(stack_t * stack, size_t idx)
{
    void * return_data = NULL;
    if ((stack != NULL) && (stack->list != NULL) && (idx < stack->size))
    {
        return_data = circular_get_data(stack->list, idx);
    }

    return return_data;
}

size_t stack_get_size(stack_t * stack)
{
    return (stack != NULL) ? stack->size : 0;
}
// END OF SOURCE