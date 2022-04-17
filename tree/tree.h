#ifndef _TREE_H_
#define _TREE_H_

#include <dstruct_funcs.h>

typedef struct tree tree_t;

tree_t * tree_create(destroy_f destroy);
void tree_destroy(tree_t * tree);
int tree_insert(tree_t * tree, char * path, char * key, void * data);
void * tree_search(tree_t * tree, char * path);
char * tree_pwd(tree_t * tree);
int tree_relative_cd(tree_t * tree, char * path);

#endif