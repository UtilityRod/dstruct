#ifndef _DSTRUCT_FUNCS_H_
#define _DSTRUCT_FUNCS_H_

typedef void (*destroy_f)(void * arg);
typedef int (*compare_f)(const void * arg1, const void * arg2);
typedef void (*print_f)(const void * arg);

#endif