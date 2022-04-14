#ifndef _STACk_H_
#define _STACk_H_

#include <stddef.h>
#include <dstruct_funcs.h>

typedef struct stack_ stack_t;

stack_t * stack_create(compare_f compare, destroy_f destroy);
stack_t * stack_create_n(compare_f compare, destroy_f destroy, size_t size);
void stack_destroy(stack_t * p_stack);
size_t stack_push(stack_t * p_stack, void * p_data);
void * stack_pop(stack_t * p_stack);
void * stack_find_nth(stack_t * stack, size_t idx);
size_t stack_get_size(stack_t * stack);

#endif
