#ifndef _STACk_H_
#define _STACk_H_

#include <stddef.h>

typedef void (*destroy_f)(void * p_arg);
typedef int (*compare_f)(const void * p_arg1, const void * p_arg2);
typedef struct stack_ stack_t;

stack_t * stack_create(compare_f compare, destroy_f destroy);
void stack_destroy(stack_t * p_stack);
size_t stack_push(stack_t * p_stack, void * p_data);
void * stack_pop(stack_t * p_stack);

#endif
