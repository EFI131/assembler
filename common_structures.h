#define MAX_LABEL_SIZE 31

/* used in both extern queue & entry linked list*/
struct EntryNode
{
    struct EntryNode *next;
    int address;
    char label[MAX_LABEL_SIZE];
};
