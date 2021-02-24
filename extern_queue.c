/**
 * The extern_queue file defines 
 * a queue holding all occurences of extern labels and their addresses 
 * and queue handelling functions    
 **/

#include "extern_queue.h"
#include <stdio.h>

/* enqueues onto q*/
void *enqueue(void *q, void *node);

/* dequeues from q */
void *dequeue(void *q);

/* A queue holding the exetern labels occurences */
struct Queue externQueue = {
    NULL,
    NULL,
    enqueue,
    dequeue};

/* Enqueues Node pointed to by node 
    onto Queue pointed to by t and
    returns a pointer to the newly inserted Node e */
void *enqueue(void *q, void *node)
{
    /* since we know what our arguments are */
    struct Queue *this = (struct Queue *)q;
    struct EntryNode *inserted = (struct EntryNode *)node;

    /* early return */
    if (!q || !node)
        return NULL;

    if (!this->tail) /* empty queue */
    {
        this->head = (void *)inserted;
        this->tail = (void *)inserted;
    }
    else
    {
        ((struct EntryNode *)this->tail)->next = inserted;
        this->tail = (void *)inserted;
    }

    return inserted;
}

/* returns a pointer to a node dequeued from the Queue pointed to by q
   or NULL if the queue is empty */
void *dequeue(void *q)
{
    struct Queue *this = (struct Queue *)q;
    struct EntryNode *removed;

    /* early return */
    if (!this)
        return NULL;

    removed = this->head;

    if (!removed)
        this->head = NULL;
    else if (this->head == this->tail)
    {
        this->head = this->tail = NULL;
    }
    else
    {
        this->head = ((struct EntryNode *)this->head)->next;
    }

    return (void *)removed;
}
