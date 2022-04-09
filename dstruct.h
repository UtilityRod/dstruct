#ifndef _DSTRUCT_H_
#define _DSTRUCT_H_

#include <circular_ll.h>
#include <queue.h>
#include <stack.h>
#include <binary_search_tree.h>
#include <hash_table.h>

typedef void (*destroy_f)(void * arg);
typedef int (*compare_f)(const void * arg1, const void * arg2);
typedef void (*print_f)(void * arg);

#endif