#ifndef _DSTRUCT_FUNCS_H_
#define _DSTRUCT_FUNCS_H_

typedef void (*destroy_f)(void * arg);
typedef int (*compare_f)(const void * arg1, const void * arg2);
typedef void (*print_f)(const void * arg);

typedef enum {
    STRUCTURE_NULL = -7, STRUCTURE_FULL, 
    KEY_EXISTS, KEY_ERROR, DATA_NULL, DATA_ERROR,
    ALLOCATION_ERROR, HEAP_FULL, OK = 0
} error_t;

#endif