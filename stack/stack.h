#ifndef _STACk_H_
#define _STACk_H_

#include <stddef.h>
#include <dstruct_funcs.h>

typedef struct stack_ stack_t;

stack_t * stack_create(compare_f compare, destroy_f destroy);
void stack_destroy(stack_t * p_stack);
size_t stack_push(stack_t * p_stack, void * p_data);
void * stack_pop(stack_t * p_stack);

#endif
