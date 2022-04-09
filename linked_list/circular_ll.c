#include <circular_ll.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    void * data;
    struct node_ * next;
    struct node_ * previous;
} node_t;

struct circular_list_ {
    node_t * head;
    ssize_t size;
    compare_f compare;
    destroy_f destroy;
    bool return_node;
};

typedef enum {LIST_EMPTY = -1, NOT_FOUND = -2} error_t;

circular_list_t * circular_create(compare_f compare, destroy_f destroy)
{
    circular_list_t * list = NULL;

    list = calloc(sizeof(*list), 1);
    if (list == NULL)
    {
        // Memory allocation failed.
        perror("Linked list memory allocation.\n");
    }
    else
    {
        list->compare = compare; // Function to compare node data
        list->destroy = destroy; // Function to destroy node data if needed
    }
    
    return list;
}

void circular_destroy(circular_list_t * list)
{
    if (list->head != NULL)
    {
        // If there are nodes inside of the list, delete all the nodes
        node_t * current = list->head;
        do
        {
            node_t * delete = current; // Save current node off for delete later
            current = current->next; // Move to next node in list

            if (list->destroy != NULL)
            {
                // Destroy function registered, call on each node's data to free all memory
                list->destroy(delete->data);
            }

            free(delete);
        } while (current != list->head);
    }

    list->head = NULL;
    free(list);
}

size_t circular_insert(circular_list_t * list, void * data, location_t location)
{
    if ((NULL == list) || (NULL == data))
    {
        // The list or the data can not be NULL, return to calling function
        return list->size;
    }
    
    node_t * new = calloc(sizeof(*new), 1);
    if (new != NULL)
    {
        new->data = data;

        if (NULL == list->head)
        {
            // List is empty
            list->head = new; // I am the head of the list
            new->next = new; // My next node is me
            new->previous = new; // My previous node is me
        }
        else
        {
            // List is not empty
            // The head node's previous node pointer is the last node in the list
            node_t * p_last = list->head->previous;
            // Update the next pointer of the last node in the list to the new node pointer
            p_last->next = new;
            // Update the new node's previous pointer to the last element in the list
            new->previous = p_last;
            // Update the head node's previous pointer to the new node
            list->head->previous = new;
            // Update the new node's next point to the list's head node
            new->next = list->head;
        }

        if (location == FRONT)
        {
            // Adding node to the front
            // Update list head pointer to new node
            list->head = new;
        }

        list->size++;
    }

    return list->size;
}

void * circular_search(circular_list_t * list, void * data)
{
    
    if (NULL == list->head)
    {
        // Linked list is empty
        return NULL;
    }

    // Iterate through list to find node containing the data passed to function
    node_t * p_search = NULL;
    node_t * current = list->head;

    do
    {
        if (list->compare(data, current->data) == 0)
        {
            // Found the node containing the data
            p_search = current;
            break;
        }
        current = current->next; // Move to next node in the list
    } while (current != list->head);

    void * return_data = NULL;
    if (p_search != NULL)
    {
        // The search node was found. Return either the data or the node pointer
        if (true == list->return_node)
        {
            // Return node flag invoked, return the pointer to the node
            return_data = p_search;
        }
        else
        {
            // Return node flag not invoked, return the node's data
            return_data = p_search->data;
        }
    }

    return return_data;
}

void * circular_get_data(circular_list_t * list, uint64_t idx)
{
    if (NULL == list || (ssize_t)idx > list->size || idx < 1)
    {
        return NULL;
    }
    
    node_t * current = list->head;
    for(uint64_t i = 1; i < idx; i++)
    {
        current = current->next;
    }
    
    return current->data;
}

void * circular_remove_at(circular_list_t * list, location_t location)
{
    node_t * p_remove = NULL;
    if ((list != NULL) && (list->head != NULL) && !((location > (int)list->size) || (location < BACK)))
    {
        /*
         * Condition 1: List must not be NULL
         * Condition 2: List must have at least one node
         * Condition 3: Location (x) must be BACK (-2) <= x < list size 
         */ 
        switch(location)
        {
            case FRONT: ;
                // Remove node is the first node in the list
                p_remove = list->head;
                break;
            case BACK: ;
                // Remove node is the last node in the list
                // Head node's previous node
                p_remove = list->head->previous;
                break;
            default:
                // Any other number will get the correct index in the list
                p_remove = list->head;
                for (int i = 1; i < location; i++)
                {
                    // Iterate through list until correct index
                    p_remove = p_remove->next;
                }
                break;
        }
    }
    else
    {
        // Could not get a node to remove
        return NULL;
    }

    if (1 == list->size)
    {
        // One on node in the list, list is now empty
        list->head = NULL;
    }
    else
    {
        // Update pointers for node's previous and next nodes;
        p_remove->next->previous = p_remove->previous;
        p_remove->previous->next = p_remove->next;

        if (p_remove == list->head)
            {
                // Removing head node, new head for list is remove node's next node
                list->head = p_remove->next;
            }
    }

    void * return_data = p_remove->data;
    list->size--;
    free(p_remove);

    return return_data;
}

void circular_ll_sort(circular_list_t * list)
{
    if (list->head == NULL)
    {
        fputs("Circular Linked List is Empty\n", stderr);
    }
    else
    {
        void ** data_array = calloc(sizeof(*data_array), list->size); // Create temporary array of void * to hold every node's data
        size_t size = list->size; // Temporary variable containing the original size of the linked list
        for (size_t i = 0; i < size; i++)
        {
            // Insert each node's data into the array to be sorted
            data_array[i] = circular_remove_at(list, FRONT);
        }
        // Sort the array with qsort and the compare function
        qsort(data_array, size, sizeof(void *), list->compare);
        for (size_t i = 0; i < size; i++)
        {
            // Insert each node's data into the array to be sorted
            circular_insert(list, data_array[i], BACK);
        }
        free(data_array);
    }
}

ssize_t circular_get_size(circular_list_t * list)
{
    ssize_t size = 0;
    if (list != NULL)
    {
        size = list->size;
    }

    return size;
}
// END OF SOURCE