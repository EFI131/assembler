#ifndef LINKED_LIST__H
#define LINKED_LIST__H

/**
 * Linked list a generic structure 
 * representing a linked list
 * implementation specidied by each "extention"
*/
struct LinkedList
{
    void *root;
    void *(*insert)(void *, void *);
    void *(*remove)(void *);
    void *(*search)(void *, void *);
    void *(*traverse)(void *, void *);
};

#endif