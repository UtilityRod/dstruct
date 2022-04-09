#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <stddef.h>

typedef struct search_tree_ search_tree_t;

search_tree_t * search_tree_create(compare_f compare, destroy_f destroy);
void search_tree_destroy(search_tree_t * tree);
size_t search_tree_insert(search_tree_t * p_tree, void * p_data);
void * search_tree_search(search_tree_t * p_tree, void * p_data);
void * search_tree_delete(search_tree_t * p_tree, void * p_data);
void search_tree_print(search_tree_t * tree, print_f printer);

#endif
