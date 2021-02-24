#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "label_hashtable.h"
#include "code_queue.h"

/* 
    an array of DataItem* acting as our labels hash table
    which holds the varioud labels, their key, type(code, data, etc.)
    & address.
 */
struct DataItem *hashArray[HASH_SIZE];

/* returns the corrrsponding index in the hash array for a given label */
int hashCode(char *label)
{
    long code = 0;
    int curr;

    /*  todo: add a check that number doesn't exceed long limits */
    for (curr = *label; curr; curr = *++label)
    {
        code = code * 10 + curr;
    }

    return code % HASH_SIZE;
}

/* calls f on each table entry */
void hashTableTraverse(void (*f)(struct DataItem *))
{
    int i;
    struct DataItem *curr;
    for (i = 0; i < HASH_SIZE; i++)
    {
        curr = hashArray[i];

        while (curr)
        {
            (*f)(curr);
            curr = curr->next;
        }
    }
}

/**
 * traverses the label hash table and updates the addresses according to their types
**/
void updateTableAddresses()
{
    int i;
    struct DataItem *curr;
    for (i = 0; i < HASH_SIZE; i++)
    {
        curr = hashArray[i];
        while (curr)
        {
            if (curr->type == CODE_LABEL)
                curr->address += PROGRAM_OFFSET;
            else if (curr->type == NUMERIC_LABEL || curr->type == CHAR_LABEL)
            {
                curr->address += PROGRAM_OFFSET + nextCodeAddress();
            }
            curr = curr->next;
        }
    }
}

/**
 * look for DataItem with kry == label
 * @param: label - the char* we are looking for
 * @return: the found item or NULL if not found
*/
struct DataItem *hashTableSearch(char *label)
{
    /* get the hash */
    int hashIndex = hashCode(label);

    return linkedListSearch(hashArray[hashIndex], label);
}

/** insert label into label hash table
 * @param: label - char* that will be inserted into table
 * @return pointer to the inserted DataItem 
 * */
struct DataItem *hashTableInsert(char *label)
{
    /* get the hash */
    int hashIndex = hashCode(label);

    return linkedListInsert(&hashArray[hashIndex], label);
}

/** searching for entry where key == label
 * @param: root - pointer to linked list root item
 * @param: label - the char* we are looking for
 * @return: pointer ot the found DataItem or NULL upon failure 
*/
struct DataItem *linkedListSearch(struct DataItem *root, char *label)
{

    struct DataItem *curr = root;

    while (curr && strcmp(curr->key, label))
        curr = curr->next;

    return curr;
}

/** inserts into the label linked list, 
 *   @param: rootP - points to the list's root
 *   @param: nodeP - points to the node that gets inserted
 *   @return: pointer to the inserted item pointe which is the root item as well 
 **/
struct DataItem *linkedListInsert(struct DataItem **rootP, char *label)
{
    struct DataItem *dataItem;

    if (linkedListSearch(*rootP, label))
        return NULL;

    dataItem = malloc(sizeof(struct DataItem));
    if (!dataItem)
        return NULL;

    dataItem->next = *rootP;
    *rootP = dataItem;
    strcpy(dataItem->key, label);

    return dataItem;
}

/** inserts into the label linked list,
 *  inserted into position in an ascending order sorted list 
 *   @param: rootP - points to the list's root
 *   @param: nodeP - points to the node that gets inserted
 *   @return: list's root pointer */
struct DataItem *linkedListInsertSorted(struct DataItem *root, struct DataItem *nodeP)
{
    struct DataItem *curr = NULL, *prev = NULL;

    if (!root) /* adding into an empty list */
        return nodeP;

    curr = root;

    while (curr && !(nodeP->address < curr->address))
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == root)
    {
        nodeP->next = curr;
        root = nodeP;
    }
    else
    {
        nodeP->next = curr;
        prev->next = nodeP;
    }

    return root;
}

/** removes the first item from a DataItem linked list
    @param: rootP - a pointer to the linked list root
    @return: the removed DataItem* upon success or NULL if list is empty
**/
static struct DataItem *linkedListRemove(struct DataItem **rootP)
{
    struct DataItem *removed = rootP ? *rootP : NULL;

    if (removed)
        *rootP = removed->next;

    return removed;
}

/* removes all entries from the laabels hash table */
void clearHashTable()
{
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        void *removed;
        while ((removed = linkedListRemove(&hashArray[i])))
        {
            free(removed);
        }
    }
}
