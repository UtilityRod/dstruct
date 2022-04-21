#include <tree.h>
#include <circular_ll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_
{
    circular_list_t * children;
    void * data;
} node_t;

struct tree 
{
    node_t * root;
    bool node_return;
    compare_f compare;
    destroy_f destroy;
};

static void destroy_node(node_t * node);
static node_t * search_node(node_t * node, void * data, compare_f compare);
static node_t * generate_node(tree_t * tree);

tree_t * tree_create(compare_f compare, destroy_f destroy)
{
    tree_t * tree = calloc(1, sizeof(*tree));

    if (NULL == tree)
    {
        // Tree memory allocation failed, return back to calling function
        goto return_back;
    }

    node_t * root = calloc(1, sizeof(*root));

    if (NULL == root)
    {
        // Root node could not be allocated, free the tree and return
        goto free_tree;
    }

    root->children = circular_create(compare, destroy);

    if (NULL == root->children)
    {
        // Children LL could not be allocated, free root node, tree, and return back to calling function
        goto free_root;
    }

    // Everything allcoted properly, return back to calling function
    tree->root = root;
    tree->compare = compare;
    tree->destroy = destroy;
    goto return_back;

free_root:
    free(root);
free_tree:
    free(tree);
    tree = NULL;
return_back:
    return tree;
}

void tree_destroy(tree_t * tree)
{
    if (NULL == tree)
    {
        return;
    }

    if (tree->root != NULL)
    {
        destroy_node(tree->root);
        tree->root = NULL; 
    }

    free(tree);
    tree = NULL;
}

int tree_insert(tree_t * tree, void * parent, void * data)
{
    if ((NULL == tree) || (NULL == data))
    {
        // Data passed to function is null, cannot be null
        return DATA_ERROR;
    }

    node_t * new_node = generate_node(tree);

    if (NULL == new_node)
    {
        return ALLOCATION_ERROR;
    }

    new_node->data = data;
    int insert_check = -1;

    if (NULL == parent)
    {
        // Insert at the root node
        insert_check = circular_insert(tree->root->children, new_node, FRONT);
    }
    else
    {
        // Search for node to insert into
        node_t * node = search_node(tree->root, parent, tree->compare);

        if (node != NULL)
        {
           insert_check = circular_insert(node->children, new_node, FRONT); 
        }
        else
        {
            destroy_node(new_node);
        }
    }

    return insert_check;
}

void * tree_search(tree_t * tree, void * data)
{
    if ((NULL == tree) || (NULL == data))
    {
        return NULL;
    }

    return search_node(tree->root, data, tree->compare);
}

static void destroy_node(node_t * node)
{
    while (circular_get_size(node->children) > 0)
    {
        node_t * current = circular_remove_at(node->children, FRONT);
        destroy_node(current);
    }

    circular_destroy(node->children);
    node->children = NULL;
    free(node);
}

static node_t * search_node(node_t * node, void * data, compare_f compare)
{
    node_t * return_node = NULL;
    if ((node->data != NULL) && (compare(node->data, data) == 0))
    {
        return_node = node;
    }
    else
    {
        size_t list_sz = circular_get_size(node->children);
        for(size_t i = 0; i < list_sz; i++)
        {
            node_t * current_node = circular_get_data(node->children, i + 1);
            node_t * result = search_node(current_node, data, compare);
            if (result != NULL)
            {
               return_node = result;
               break;
            }
        }
    }

    return return_node;
}

static node_t * generate_node(tree_t * tree)
{
    node_t * node = calloc(1, sizeof(*node));

    if (NULL == node)
    {
        return NULL;
    }

    node->children = circular_create(tree->compare, tree->destroy);

    if (NULL == node->children)
    {
        free(node);
        return NULL;
    }

    return node;
}
// END OF SOURCE