#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "globals.h"

#define HASH_SIZE 20
#define MAX_LABEL_LENGTH 31

/* representd different label types */
enum DataType
{
    ENTRY_LABEL = 1,
    EXTERN_LABEL = 2,
    CODE_LABEL = 4,
    NUMERIC_LABEL = 8,
    CHAR_LABEL = 16
};

/* Data Items for the labels table */
struct DataItem
{
    int address;
    enum DataType type;
    struct DataItem *next;
    char key[MAX_LABEL_LENGTH + 1];
};

/* TODO: hide if possible */
/** Array of labels DataItem linked lists -> enreis assigned a linked list via a hash function **/
extern struct DataItem *hashArray[];

/** returns the corrrsponding index in the hash array */
int hashCode(char *label);

/**
 *  looks for an entry by label 
 * returns DataItem* upon success and NULL if no match found */
struct DataItem *hashTableSearch(char *label);

struct DataItem *hashTableInsert(char *label);

struct DataItem *linkedListSearch(struct DataItem *root, char *label);

/* inserts label as the first element in the linked list
 * @param: rooP -  a pointer to the linked list root
 * @param: label - the label to insert 
 */
struct DataItem *linkedListInsert(struct DataItem **rootP, char *label);

/**
 * traverses the label hash table and updates the addresses according to their types
**/
void updateTableAddresses();

/* calls f on each table entry */
void hashTableTraverse(void (*f)(struct DataItem *));

/* inerts nodeP into the linkedlist pointed by root intob it's acsending order location in the list */
struct DataItem *linkedListInsertSorted(struct DataItem *root, struct DataItem *nodeP);

/* removes all entries from the laabels hash table */
void clearHashTable();
