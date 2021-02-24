/* Code word structs organized by the different layout corresponding to different code words */

/* operation word*/
struct Operator
{
    unsigned int are : 3;
    unsigned int dst : 4;
    unsigned int src : 4;
    unsigned int opcode : 4;
};

/* immediate value */
struct Immediate
{
    unsigned are : 3;
    signed val : 12;
};

/* label */
struct Direct
{
    unsigned are : 3;
    unsigned address : 12;
};

/* indirect register */
struct IndirectRegister
{
    unsigned are : 3;
    unsigned dstReg : 3;
    unsigned srcReg : 3;
    unsigned padding : 6;
};

/* direct register */
struct DirectRegister
{
    unsigned are : 3;
    unsigned dstReg : 3;
    unsigned srcReg : 3;
    unsigned padding : 6;
};

/* used for raw word retrieving */
struct RawVal
{
    unsigned val : 15;
};

/* pack the all as a union */
union OpWord {
    struct Operator op;
    struct Immediate imm;
    struct Direct dir;
    struct IndirectRegister indirReg;
    struct DirectRegister dirReg;
    struct RawVal raw;
};

/* a struct holding the actual word + a type field so we know how to read it */
struct CodeWord
{
    int type;
    union OpWord word;
};

/* Code Linked List Node*/
struct QueueNode
{
    struct CodeWord codeWord;
    struct QueueNode *next;
    int address;    /* address in .as file */
    char label[32]; /* in case of direct params we need the label used till we replace it with the actual address*/
};

/**************************
 *    Code Queue
 *    Holds QueueNodes arranged according to the .as file parsing order,
 *    representing the code memory segment
 ************************/
struct CodeQueue
{
    unsigned int length;
    struct QueueNode *tailP;
    struct QueueNode *headP;
};

/* queues node into the code queue */
struct QueueNode *codeEnqueue(struct QueueNode *nodeP);

/* Dequeues a node from the code queue */
struct QueueNode *codeDequeue();

/* retuns the number of code words enqueued */
int nextCodeAddress();

/* goes over the codeQueue and calls f on each node */
void traverseCodeQueue(void (*f)(struct QueueNode *));
