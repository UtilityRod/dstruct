#include <binary_search_tree.h>
#include <stack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_{
    void * data;
    uint32_t height;
    struct node_ * left;
    struct node_ * right;
    struct node_ * parent;
} node_t;

struct search_tree_ {
    node_t * root;
    size_t size;
    destroy_f destroy;
    compare_f compare;
    bool node_return_flag;
};

typedef enum child_count_ {NONE, ONE, TWO} child_count_t;

static child_count_t amount_children(node_t * node);
static void * bst_remove_leaf(search_tree_t * tree, node_t * delete);
static void * bst_one_child_remove(search_tree_t * tree, node_t * delete);
static void * bst_two_child_remove(search_tree_t * tree, node_t * delete);
static int max_height(node_t * node);
static int get_balance_factor(node_t * node);
static void rotate_right(search_tree_t * tree, node_t * parent, node_t * child);
static void rotate_left(search_tree_t * tree, node_t * parent, node_t * child);
static void balance_tree(search_tree_t * tree, node_t * start_node);
static void print_util(node_t * node, int space, print_f printer);

search_tree_t * search_tree_create(compare_f compare, destroy_f destroy)
{

    if (compare == NULL)
    {
        // Binary search tree requires a compare function to properly insert
        fputs("Compare function passed must not be NULL.\n", stderr);
    }

    search_tree_t * tree = calloc(sizeof(*tree), 1);

    if (tree != NULL)
    {
        tree->compare = compare; // Function to compare node data
        tree->destroy = destroy; // Function to destroy node data (Only if needed)
    }

    return tree;
}

void search_tree_destroy(search_tree_t * tree)
{
   if (tree == NULL)
    {
        return;
    }
    // Free all the memory for each node in the linked list
    if (tree->root)
    {
        node_t * current = tree->root;
        stack_t * p_stack = stack_create(tree->compare, tree->destroy);

        while(current)
        {
            if (current->left)
            {
                // If I have a left child push myself to the p_stack
                stack_push(p_stack, current);
                // Temporary holder for my left child
                node_t * p_tmp = current->left;
                // Update current node left pointer to NULL due to freeing of memory for left child
                current->left = NULL;
                // Change current node to the left child (temporary holder) to traverse down tree
                current = p_tmp;
            }
            else if (current->right)
            {
                // If I have a right child push myself to the p_stack
                stack_push(p_stack, current);
                // Temporary holder for my right child
                node_t * p_tmp = current->right;
                // Update current node right pointer to NULL due to freeing of memory for right child
                current->right = NULL;
                // Change current node to the right child (temporary holder) to traverse down tree
                current = p_tmp;
            }
            else
            {
                // Node no longer has any children so free memory for the current node
                if (tree->destroy)
                {
                    // Only if destroy function was registered
                    tree->destroy(current->data);
                }
                free(current); // Free Node's memory
                current = stack_pop(p_stack); // Get next node off the p_stack
            }
        }
        stack_destroy(p_stack); // Destroy the p_stack's memory
    }   

    free(tree); 
}

size_t search_tree_insert(search_tree_t * tree, void * data)
{
    // Performs a BST insert then a AVL Reblance in needed
    if (tree == NULL)
    {
        fputs("ERROR: Binary tree pointer is NULL\n", stdout);
    }

    // Perform standard BST insert
    node_t * new = calloc(sizeof(*new), 1);
    new->height = 0; // Starting height for a node in the binary search tree
    
    if (!new)
    {
        // New node memory allocation failure
        perror("Binary tree node memory allocation.");
        return tree->size;
    }
    else
    {
        new->data = data;
    }

    if (tree->root == NULL)
    {
        // tree is empty insert node at the top of the tree
        tree->root = new;
    }
    else
    {
        // tree is not empty
        node_t * current = tree->root;
        node_t * previous_node = NULL;
        int data_check = 0;
        while (current != NULL)
        {
            // Call compare function to see if data is greater than, less than or equal to the current data
            data_check = tree->compare(data, current->data);
            previous_node = current;
            if ( data_check < 0 )
            {
                // The new data is greater than the current node's data
                current = current->right;
            }
            else if ( data_check > 0 )
            {
                // The new data is less than the current node's data
                current = current->left;
            }
            else
            {
                // The data is the same in both the current node and the new data
                fputs("Key already exists inside of binary tree.\n", stderr);
                return tree->size;
            }
        }

        if (data_check < 0)
        {
            // Add new node to the right of the node who has a NULL right child
            // New node's key is greater than its key
            new->parent = previous_node;
            previous_node->right = new;
        }
        else
        {
            // Add new node to the left of the node who has a NULL left child
            // New node's key is less than its key
            new->parent = previous_node;
            previous_node->left = new;
        }
    }

    //update_heights(new);
    balance_tree(tree, new);
    return ++tree->size;
}

void * search_tree_search(search_tree_t * tree, void * data)
{
    if (tree == NULL || tree->root == NULL || tree->compare == NULL)
    {
        // tree must have allocated memory, at least one node, and a compare function to properly remove
        return NULL;
    }

    node_t * current = tree->root;
    node_t * p_search = NULL;
    while (current)
    {
        int data_check = tree->compare(data, current->data);
        if (data_check < 0)
        {
            // The search key is larger than the current node's key
            // Move to the right child of the current node
            current = current->right;
        }
        else if (data_check > 0)
        {
            // The search key is smaller than the current node's key
            // Move to the left child of the current node
            current = current->left;
        }
        else
        {
            // Search key found in tree
            p_search = current;
            break;
        }
    }
    // If the node was found return the node's data, else return NULL
    void * return_data = NULL;
    if (tree->node_return_flag)
    {
        return_data = p_search;
    }
    else
    {
       return_data = p_search ? p_search->data : NULL; 
    }

    return return_data;
}

void * search_tree_delete(search_tree_t * tree, void * data)
{
    if (tree == NULL || tree->root == NULL || tree->compare == NULL)
    {
        // tree must have allocated memory, at least one node, and a compare function to properly remove
        return NULL;
    }
    tree->node_return_flag = true;
    node_t * delete_node = search_tree_search(tree, data);
    tree->node_return_flag = false;
    void * return_data = NULL;

    if (delete_node)
    {
        child_count_t children = amount_children(delete_node);
        node_t * parent_node = delete_node->parent;
        
        switch (children)
        {
            case NONE:
                return_data = bst_remove_leaf(tree, delete_node);
                break;
            case ONE:
                return_data = bst_one_child_remove(tree, delete_node);
                break;
            case TWO:
                return_data = bst_two_child_remove(tree, delete_node);
                break;
            default:
                break;
        }

        tree->size--;
        if (parent_node != NULL)
        {
            balance_tree(tree, parent_node);
        }
    }
    
    return return_data;
}

void search_tree_print(search_tree_t * tree, print_f printer)
{
    if ((NULL == tree) || (NULL == tree->root))
    {
        return;
    }
    else
    {
        print_util(tree->root, 0, printer);
    }
}

static void print_util(node_t * node, int space, print_f printer)
{
    if (NULL == node)
    {
        return;
    }
    
    space += 10;

    print_util(node->right, space, printer);

    printf("\n");
    for ( int i = 10; i < space; i++)
    {
        printf(" ");
    }
    printer(node->data);
    printf(":%d\n", node->height);

    print_util(node->left, space, printer);
}

static child_count_t amount_children(node_t * node)
{
    if (node->left == NULL && node->right == NULL)
    {
        // Leaf node
        return NONE;
    }
    else if ((node->left == NULL) ^ (node->right == NULL))
    {
        // Only one child
        return ONE;
    }
    else
    {
        // Two children
        return TWO;
    }
}

static void * bst_remove_leaf(search_tree_t * tree, node_t * delete)
{
    if (tree->root == delete)
    {
        // The p_node to be deleted is the only p_node in the tree
        // NULL out the tree root pointer
        tree->root = NULL;
    }
    else if (delete == delete->parent->left)
    {
        delete->parent->left = NULL;
    }
    else
    {
        delete->parent->right = NULL;
    }

    void * p_return_data = delete->data;
    delete->parent->height = max_height(delete->parent);
    free(delete);
    return p_return_data;
}

static void  * bst_one_child_remove(search_tree_t * tree, node_t * delete)
{
    if (delete->parent == NULL && delete->left)
    {
        // p_node to be removed is the root p_node in the tree, and p_node has a left child
        // Remove root p_node and replace with left child
        tree->root = delete->left;
        tree->root->parent = NULL;
    }
    else if (delete->parent == NULL && delete->right)
    {
        // p_node to be removed is the root p_node in the tree, and p_node has a right child
        // Remove root p_node and replace with right child
        tree->root = delete->right;
        tree->root->parent = NULL;
    }
    else if (delete->left && delete == delete->parent->left)
    {
        // p_node to be deleted is the left child of the grandparent, p_node replacing me is my left child
        delete->parent->left = delete->left;
        delete->left->parent = delete->parent;
    }
    else if (delete->right && delete == delete->parent->left)
    {
        // p_node to be deleted is the left child of the grandparent, p_node replacing me is my right child
        delete->parent->left = delete->right;
        delete->right->parent = delete->parent;
    }
    else if (delete->left && delete == delete->parent->right)
    {
        // p_node to be deleted is the right child of the grandparent, p_node replacing me is my left child
        delete->parent->right = delete->left;
        delete->left->parent = delete->parent;
    }
    else
    {
        // p_node to be deleted is the right child of the grandparent, p_node replacing me is my right child
        delete->parent->right = delete->right;
        delete->right->parent = delete->parent;
    }

    void * p_return_data = delete->data;
    delete->parent->height = max_height(delete->parent) + 1;
    free(delete);
    return p_return_data;
}

static void * bst_two_child_remove(search_tree_t * tree, node_t * delete)
{
    // Find the lowest value in the right of the p_node to be deleted
    node_t * minimum = delete->right;
    node_t * current = minimum->left;
    while(current)
    {
        if (tree->compare(minimum->data, current->data) < 0)
        {
            // Found new minimum
            minimum = current;
        }

        current = current->left;
    }

    // Save off data to be removed for return later
    void * p_return_data = delete->data;
    // Update the p_node's data to new value. Minimum of the right side of delete p_node
    delete->data = minimum->data;
    // See if minimu has any children and remove minimum
    child_count_t count = amount_children(minimum);
    switch(count)
    {
        case NONE:
            bst_remove_leaf(tree, minimum);
            break;
        case ONE:
            bst_one_child_remove(tree, minimum);
            break;
        case TWO:
            bst_two_child_remove(tree, minimum);
        default:
            break;
    }

    delete->height = max_height(delete) + 1;
    return p_return_data;
}

static int max_height(node_t * node)
{
    // Get the height of a given node.
    // Height is the max height between the left and right children
    int lheight = node->left ? node->left->height : 0;
    int rheight = node->right ? node->right->height : 0;
    if(lheight > rheight)
    {
        return lheight;
    }
    else
    {
        return rheight;
    }
}

static int get_balance_factor(node_t * node)
{
    // Get the balance factor for a given node needed for AVL trees
    // abs_switch turns on or off absolute value of the balance factor
    int lheight = node->left ? node->left->height : 0;
    int rheight = node->right ? node->right->height : 0;
    return rheight - lheight;
}

static void rotate_right(search_tree_t * tree, node_t * parent, node_t * child)
{
    // child's right child becomes parents new left child
    parent->left = child->right;
    if (parent->left)
    {
        child->right->parent = parent;
    }
    // parent node becomes child node's new right child
    child->right = parent;
    // child's parent is now the parent of parent
    child->parent = parent->parent;
    // parent's new parent is child
    parent->parent = child;

    if (child->parent)
    {
        // Update the child's new parent pointer to point to child
        if(tree->compare(child->parent->data, child->data) > 0)
        {
            // child is larger, therefore on the right side of it's parent
            child->parent->right = child;
        }
        else
        {
            // child is smaller, therefore on the left side of it's parent
            child->parent->left = child;
        }
    }

    if (tree->root == parent)
    {
        // If rotating the root node, then update the pointer to the tree root to be child
        tree->root = child; 
    }
    
    // Update heights node rotated nodes
    int tmp_height = parent->height;
    if ((NULL == parent->left) && (NULL == parent->right))
    {
        parent->height = 0;
    }
    else
    {
        parent->height = child->height - 1;
    }
    
    child->height = tmp_height - 1;
}

static void rotate_left(search_tree_t * tree, node_t * parent, node_t * child)
{
    // child's left child becomes parent's new right child
    parent->right = child->left;
    if (parent->right)
    {
        // Update node parent pointer to be parent
        child->left->parent = parent;
    }
    // parent node becomes child node's new left child
    child->left = parent;
    // child's parent is now the parent of parent
    child->parent = parent->parent;
    // parent's new parent is child
    parent->parent = child;
    if (child->parent)
    {
        // Update the child's new parent pointer to point to child
        if(tree->compare(child->parent->data, child->data) > 0)
        {
            // child is larger, therefore on the right side of it's parent
            child->parent->right = child;
        }
        else
        {
            // child is smaller, therefore on the left side of it's parent
            child->parent->left = child;
        }
    }
    if (tree->root == parent)
    {
        // If rotating the root node, then update the pointer to the tree root to be child
        tree->root = child; 
    }
    
    // Update heights node rotated nodes
    int tmp_height = parent->height;
    if ((NULL == parent->left) && (NULL == parent->right))
    {
        parent->height = 0;
    }
    else
    {
        parent->height = child->height - 1;
    }
    
    child->height = tmp_height - 1;
}

static void balance_tree(search_tree_t * tree, node_t * start_node)
{
    node_t * current = start_node;
    int bf = 0;

    // Travel upwards and Check balance factors
    while(current->parent && (bf <= 1 && bf >= -1))
    {
        current = current->parent;
        bf = get_balance_factor(current); // Get the balance factor of the node
        current->height = max_height(current) + 1; // Update the node's height to the left/right subtree max height plus one
    }

    if (bf >= -1 && bf <= 1)
    {
        return;
    }

    
    // Find the correct child node with is in the path to the new node that was inserted
    if (bf <= -1)
    {
        // left subtree is unbalanced
        if (tree->compare(start_node->data, current->left->data) > 0)
        {
            // Grandchild is the left child of node_t * child
            // Perform right rotate
            rotate_right(tree, current, current->left);
        }
        else
        {
            // Grandchild is the right child of node_t * child
            // Perform left right Rotate
            rotate_left(tree, current->left, current->left->right);
            rotate_right(tree, current, current->left);
        }
    }
    else
    {
        // right subtree is unbalanced
        if (tree->compare(start_node->data, current->right->data) < 0)
        {
            // Grandchild is the right child of node_t * child
            // Perform left Rotate
            rotate_left(tree, current, current->right);
        }
        else
        {
            // Grandchild is the left child of node_t * child
            // Perform right left Rotate
            rotate_right(tree, current->right, current->right->left);
            rotate_left(tree, current, current->right);
        }
    }

    return;
}
// END OF SOURCE