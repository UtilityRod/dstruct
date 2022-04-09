#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stddef.h>

typedef struct hash_table hash_table_t;
typedef int (*hash_function_f)(void * arg);

hash_table_t * table_create(size_t size, hash_function_f hash);
size_t table_insert(hash_table_t * table, void * key, void * value);
void table_destroy(hash_table_t * table, destroy_f value_destroy);
void * table_search(hash_table_t * table, void * key);

#endif