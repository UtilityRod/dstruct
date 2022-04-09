#include "stack.h"
#include <circular_ll.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct stack_ {
    circular_list_t * p_list;
    size_t size;
};

stack_t * stack_create(compare_f compare, destroy_f destroy)
{
    stack_t * p_stack = calloc(sizeof(*p_stack), 1);

    if (p_stack == NULL)
    {
        // Memory allocation for stack failed
        perror("Stack creation.");
    }
    else
    {
        // Call circular linked list function to create list
        p_stack->p_list = circular_create(compare, destroy);
    }

    return p_stack;
}

void stack_destroy(stack_t * p_stack)
{
    if (p_stack && p_stack->p_list)
    {

        if (p_stack->p_list)
        {
            circular_destroy(p_stack->p_list);
        }

        free(p_stack);
    }
}

size_t stack_push(stack_t * p_stack, void * p_data)
{
    if (p_stack && p_stack->p_list)
    {
        p_stack->size = circular_insert(p_stack->p_list, p_data, FRONT);
    }

    return p_stack->size;
}

void * stack_pop(stack_t * p_stack)
{
    void * p_data = NULL;

    if (p_stack && p_stack->p_list)
    {
        p_data = circular_remove_at(p_stack->p_list, FRONT);
    }

    return p_data;
}
// END OF SOURCE
