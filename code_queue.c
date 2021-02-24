/**************************
 *   Code Queue
 *
 *    Holding QueueNodes arranged according to the .as file parsing order,
 *    representing the code memory segment
 *
 ***********************/
#include "code_queue.h"
#include <stdio.h>

/* our code queue */
struct CodeQueue codeQueue = {0, NULL, NULL};
struct CodeQueue *queueP = &codeQueue;

/* queues node into the code queue */
struct QueueNode *codeEnqueue(struct QueueNode *nodeP)
{
    if (queueP->length)
    {
        ((queueP->tailP)->next) = nodeP;
    }
    else
    {
        queueP->headP = nodeP;
    }

    queueP->tailP = nodeP;

    queueP->length++;

    return nodeP;
}

/* Dequeues a node from the code queue */
struct QueueNode *codeDequeue()
{
    struct QueueNode *nodeP = queueP->headP;

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

/* retuns the number of code words enqueued */
int nextCodeAddress()
{
    return codeQueue.length;
}

/* goes over the codeQueue and calls f on each node */
void traverseCodeQueue(void (*f)(struct QueueNode *))
{
    struct QueueNode *nodeP;
    nodeP = codeQueue.headP;

    while (nodeP)
    {
        (*f)(nodeP);
        nodeP = nodeP->next;
    }
}