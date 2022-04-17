#include <tree.h>
#include <circular_ll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_
{
    circular_list_t * children;
    char * path;
    void * data;
    char * key;
    struct node_ * parent;
} node_t;

struct tree 
{
    node_t * root;
    node_t * current_node;
    bool node_return;
};

static int key_compare(const void * arg1, const void * arg2);
static void destroy_node(node_t * node);

tree_t * tree_create(destroy_f destroy)
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

    char root_path[] = "root";
    root->path = strdup(root_path);

    if (NULL == root->path)
    {
        goto free_root;
    }

    root->key = root->path;
    root->children = circular_create(key_compare, destroy);

    if (NULL == root->children)
    {
        // Children LL could not be allocated, free root node, tree, and return back to calling function
        goto free_root_path;
    }

    // Everything allcoted properly, return back to calling function
    tree->root = root;
    tree->current_node = root;
    goto return_back;

free_root_path:
    free(root->path);
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

int tree_insert(tree_t * tree, char * path, char * key, void * data)
{
    if ((NULL == tree) || (NULL == path) || (NULL == key) || (NULL == data))
    {
        // Data passed to function is null, cannot be null
        return DATA_ERROR;
    }

    // Search for node to insert new data into
    tree->node_return = true;
    node_t * parent = tree_search(tree, path);
    tree->node_return = false;

    if (NULL == parent)
    {
        return KEY_ERROR;
    }

    // Allocat memory for new node
    node_t * new_node = calloc(1, sizeof(*new_node));

    if (NULL == new_node)
    {
        return ALLOCATION_ERROR;
    }

    new_node->children = circular_create(key_compare, NULL);

    if (NULL == new_node->children)
    {
        free(new_node);
        return ALLOCATION_ERROR;
    }

    // Create new path for new node
    size_t parent_sz = strlen(parent->path) + strlen(key) + 2;
    new_node->path = calloc(parent_sz, sizeof(char));

    if (NULL == new_node->path)
    {
        // Path did not allocate properly
        circular_destroy(new_node->children);
        free(new_node);
        return ALLOCATION_ERROR;
    }
    // Add information to node struct
    snprintf(new_node->path, parent_sz, "%s.%s", parent->path, key);
    new_node->data = data;
    new_node->key = key;
    new_node->parent = parent;
    circular_insert(parent->children, new_node, FRONT);

    return 0;
}

void * tree_search(tree_t * tree, char * path)
{
    if ((NULL == tree) || (NULL == path))
    {
        return NULL;
    }

    char * search_str = strdup(path);

    if (NULL == search_str)
    {
        return NULL;
    }

    char * token = strtok(search_str, ".");
    node_t * current = tree->root;

    if (strcmp(token, "root") != 0)
    {
        free(search_str);
        return NULL;
    }

    for (;;)
    {
        token = strtok(NULL, ".");

        if (NULL == token)
        {
            break;
        }

        node_t search = {.key = token};
        current = circular_search(current->children, &search);

        if (NULL == current)
        {
            free(search_str);
            return NULL;
        }
    }

    free(search_str);
    return tree->node_return ? current : current->data;
}

char * tree_pwd(tree_t * tree)
{
    char * return_str = NULL;
    if ((tree != NULL) && (tree->current_node != NULL))
    {
        return_str = tree->current_node->path;
    }
    printf("%s\n", return_str);
    return return_str;
}

int tree_relative_cd(tree_t * tree, char * path)
{
    if ((NULL == tree))
    {
        return DATA_ERROR;
    }

    tree->node_return = true;
    node_t * node = tree_search(tree, path);
    tree->node_return = false;

    int rv = KEY_ERROR;
    if (node != NULL)
    {
        tree->current_node = node;
        rv = OK;
    }

    return rv;
}

static int key_compare(const void * arg1, const void * arg2)
{
    node_t * node1 = (node_t *)arg1;
    node_t * node2 = (node_t *)arg2;

    return strcmp(node1->key, node2->key);
}

static void destroy_node(node_t * node)
{
    while (circular_get_size(node->children) != 0)
    {
        node_t * current = circular_remove_at(node->children, FRONT);
        destroy_node(current);
    }

    circular_destroy(node->children);
    node->children = NULL;
    free(node->path);
    node->path = NULL;
    free(node);
}
// END OF SOURCE