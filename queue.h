struct Queue
{
    void *head;
    void *tail;
    void *(*enqueue)(void *, void *);
    void *(*dequeue)(void *);
};
