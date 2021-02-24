#include "entry_linked_list.h"

#define NULL ((void *)0)

/** insertEntry: inserts an entry into list  
 *  @param list: a pointer to a linkedList
 *  @param entry:  a pointer to and EntryNode
 *  @return: a pointer to the list's root EntryNode
 **/
void *insertEntry(void *t, void *l);

/** 
 * removeEntry removes an entry from the entry linked list 
 * @param: a pointer to a linked list
 * @return: a pointer to the removed EntryNode or NULL if list is empty.
 **/
void *removeEntry(void *t);

/*void *searchEntry(void *t, void *l);
void *traverseList(void*, void *l;*/

/** a linked list consisting of entriy names and addresses
 * exposing a list of  linked list handling utility functions
 **/
struct LinkedList entryList = {
    NULL,
    insertEntry,
    removeEntry};

/** insertEntry: inserts an entry into list  
 *  @param list: a pointer to a linkedList
 *  @param entry:  a pointer to and EntryNode
 *  @return: a pointer to the list's root EntryNode
 **/
void *insertEntry(void *list, void *entry)
{
    /* since we know what our arguments are */
    struct LinkedList *this = (struct LinkedList *)list;
    struct EntryNode *nodeP = (struct EntryNode *)entry;
    struct EntryNode *prev = NULL;
    struct EntryNode *curr;

    /* early return */
    if (!list || !entry)
        return NULL;

    curr = (struct EntryNode *)this->root;

    while (curr && (nodeP->address > curr->address))
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == (struct EntryNode *)this->root)
    {
        nodeP->next = (struct EntryNode *)this->root;
        this->root = (void *)nodeP;
    }
    else
    {
        nodeP->next = curr;
        prev->next = nodeP;
    }

    return this->root;
}

/** 
 * removeEntry removes an entry from the entry linked list 
 * @param: a pointer to a linked list
 * @return: a pointer to the removed EntryNode or NULL if list is empty.
 **/
void *removeEntry(void *list)
{
    struct LinkedList *this = (struct LinkedList *)list;
    struct EntryNode *removed;

    /* early return */
    if (!this)
        return NULL;

    removed = this->root;

    if (removed)
        this->root = removed->next;

    return (void *)removed;
}
