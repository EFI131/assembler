#ifndef MEMORY__H
#define MEMORY__H
/* basic augmented types representing data memory words */
/* number */
struct NumWord
{
    unsigned int value : 14;
    unsigned int sign : 1;
};

/* character */
struct CharWord
{
    unsigned int value : 15;
};

/* union of both num & data */
union DataWord {
    struct NumWord n;
    struct CharWord c;
};

/* node for the memory queue*/
struct MemoryNode
{
    int type;
    union DataWord data;
    struct MemoryNode *next;
};

/* memory queue will hold data in it's order of insertion */
struct MemoryQueue
{
    unsigned int length;
    struct MemoryNode *tailP;
    struct MemoryNode *headP;
};

/** queues nodeP onto the memory queue
 * @param: nodeP - pointer to the to be inserted MemoryNode 
 * @return: a pointer to inserted Memory Node 
 * **/
struct MemoryNode *memoryEnqueue(struct MemoryNode *nodeP);

/** dequeues node from the memory queue 
 * @return: a pointer to dequeued MemoryNode 
 **/
struct MemoryNode *memoryDequeue();

/** enqueues string onto the memory queue
 * @param: string - enqueue
 * @return: 1 upon success, 0 upon faliure
 */
int enqueueString(char *string);

/**
 * enqueues a number onto the meory queue
 * @param: num - the number to enqueue
 * @return: 1 upon success, 0 upon faliure
 **/
int enqueueNumber(int num);

/**
 * @return: no of nodes in memory queue 
*/
int nextMemoryAddress();

#endif