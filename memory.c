#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

/* empty queue */
struct MemoryQueue memoryQueue = {0, NULL, NULL};
static struct MemoryQueue *queueP = &memoryQueue;

/** queues MemoryNode* nodeP onto the memory queue
 * @param: nodeP - pointer to the to be inserted MemoryNode 
 * @return: a pointer to inserted Memory Node 
 * **/
struct MemoryNode *memoryEnqueue(struct MemoryNode *nodeP)
{
	if (queueP->length)
	{
		(queueP->tailP)->next = nodeP;
	}
	else
	{
		queueP->headP = nodeP;
	}

	queueP->tailP = nodeP;

	queueP->length++;

	return nodeP;
}

/** dequeues node from the memory queue 
 * @return: a pointer to dequeued MemoryNode 
 **/
struct MemoryNode *memoryDequeue()
{
	struct MemoryNode *nodeP = queueP->headP;

	if (queueP->headP == queueP->tailP)
	{
		queueP->length = 0;
		queueP->headP = queueP->tailP = 0;
	}
	else
	{
		queueP->length--;
		queueP->headP = nodeP->next;
	}

	return nodeP;
}

/** enqueues string onto the memory queue
 * @param: string - enqueue
 * @return: 1 upon success, 0 upon faliure
 */
int enqueueString(char *string)
{
	struct MemoryNode *nodeP, *enqueued;

	/* allocate */
	/* enqueue */
	/* as long as allocation suceeded , !", enuqueing succeeded */
	while (
		(nodeP = (struct MemoryNode *)malloc(sizeof(struct MemoryNode))) &&
		(enqueued = memoryEnqueue(nodeP)) &&
		((nodeP->type = 'c')) &&
		(nodeP->data.c.value = *++string) != '\"')
		;

	if (!nodeP)
	{ /* can't allocate memory*/
		return 0;
	}
	else if (!enqueued)
	{
		/* can't enqueue */
		return 0;
	}

	nodeP->data.c.value = '\0';
	return 1;
}

/**
 * enqueues a number onto the meory queue
 * @param: num - the number to enqueue
 * @return: 1 upon success, 0 upon faliure
 **/
int enqueueNumber(int num)
{
	struct MemoryNode *nodeP, *enqueued;
	/* allocate */
	/* enqueue */
	if ((nodeP = (struct MemoryNode *)malloc(sizeof(struct MemoryNode))) &&
		(enqueued = memoryEnqueue(nodeP)))
	{
		nodeP->type = 'n';
		nodeP->data.n.sign = num < 0 ? 1 : 0;
		nodeP->data.n.value = num;
	}
	else if (!nodeP)
	{ /* can't allocate memory */
		return 0;
	}
	else if (!enqueued)
	{ /* can't enqueue */
		return 0;
	}

	return 1;
}

/**
 * @return: no of nodes in memory queue 
*/
int nextMemoryAddress()
{
	return memoryQueue.length;
}
