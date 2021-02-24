#include <stdio.h>
#include "line_parsing.h"
#include "file_parsing.h"
#include "memory.h"
#include "operations.h"
#include "globals.h"
#include "label_hashtable.h"
#include "entry_linked_list.h"
#include "extern_queue.h"
#include "code_queue.h"

int writeObjectFile(char *fileName);

/* writes out fileName.ext */
int writeExternFile(char *fileName);

/* writes out fileName.ent */
int writeEntryFile(char *fileName);

/* resets & clears all data & data structures related to the file */
static void clear();

/** reads & assembles fileName.as , produces .ob, .ext, .ent upon successful assembling
 * prints an error message to stdout incase of such.
 * @param: fileName: char * - an extentionless filename, fileName.as assumed to exist.
 **/
void assemble(char *fileName)
{

    extern int lineNo;
    char buffer[MAX_FILENAME_LENGTH];
    char line[MAX_LINE_LENGTH];

    FILE *fp;
    strcpy(buffer, fileName);
    strcat(buffer, ".as");

    if (!(fp = fopen(buffer, "r")))
    {
        printf("can't open file %s\n", buffer);
        return;
    }
    /* reset line no. */
    lineNo = 0;
    while (readLine(line, fp)) /* read & parse lines */
    {
        lineNo++;
        parseLine(line);
    }

    if (errorFlag) /* error during line parsing */
    {
        clear();
        return;
    }

    updateTableAddresses(); /* updates addresses as we now know the code section's size */

    rewind(fp); /* seek back to beginning of file */
    lineNo = 0;
    /* handle entry line */
    while (readLine(line, fp))
    {
        lineNo++;
        parseEntryLine(line);
    }

    updateDirectParams(); /* replace labels in code with their addresses */

    if (errorFlag) /* error parsing entry lines */
    {
        clear();
        return;
    }

    writeOutFiles(fileName); /* writes .obj, .ent, .ext files */
    clear();                 /* clear file code & data */
}

/** readLine: reads a line from fp into buffer upto a '\n' an EOF or MAX_LINE_LENGTH was reached.
 * @param: buffer: char * - rading into, @param fp: FILE* - reading from.
 * @return: number of chars red from file, 0 in case of EOF.
 **/
int readLine(char *buffer, FILE *fp)
{
    static const char *TAG = "READ_LINE";

    int i = 0;

    do
    {
        *buffer = fgetc(fp);
    } while (!feof(fp) && i++ < MAX_LINE_LENGTH && *buffer != '\n' && buffer++);

    *buffer = '\0';

    if (i == MAX_LINE_LENGTH)
    {
        errorFlag = ON;
        printf("%s::ERROR::LINE_TOO_LONG: line length > %d\n", TAG, MAX_LINE_LENGTH);
    }

    return i;
}

/** writes all output files: filename.ob, filename.ent, filename.ext
 *  @param: fileName: char * - an extentionless filename
 * TODO: update return type to void.
 **/
void writeOutFiles(char *fileName)
{
    /* create to corresponding files */
    /* write into them */
    writeObjectFile(fileName);
    writeExternFile(fileName);
    writeEntryFile(fileName);
}

/* writes the .obj file
*  @param: fileName: char * - an extentionless filename
 * TODO: update return type to void.
 **/
int writeObjectFile(char *fileName)
{
    /* open file for writing */
    char buffer[MAX_FILENAME_LENGTH];
    FILE *fp;
    struct QueueNode *codeNodeP;
    struct MemoryNode *memNodeP;
    int i;

    strcpy(buffer, fileName);
    strcat(buffer, ".ob");

    fp = fopen(buffer, "w");
    if (!fp)
    {
        puts("Cannot open");
    }
    /* write title */
    fprintf(fp, "   %d %d\n", nextCodeAddress(), nextMemoryAddress());
    /* traverse code queue and write to file */

    i = 0;
    while ((codeNodeP = codeDequeue()) && (i * WORD_LENGTH < MAX_PROGRAM_MEMORY))
    {
        /* write code word to file */
        fprintf(fp, "%04d %05o\n", i + PROGRAM_OFFSET, codeNodeP->codeWord.word.raw.val);
        i++;
    }

    /* traverse over memory queue and write to file */
    while ((memNodeP = memoryDequeue()) && i * WORD_LENGTH < MAX_PROGRAM_MEMORY)
    {
        /* write code word to file */
        fprintf(fp, "%04d %05o\n", i + PROGRAM_OFFSET, memNodeP->data.c.value);
        i++;
    }
    return 1;
}

/* writes the .ext file 
*  @param: fileName: char * - an extentionless filename
 * TODO: update return type to void.
 **/
int writeExternFile(char *fileName)
{
    char buffer[MAX_FILENAME_LENGTH];
    struct EntryNode *nodeP;
    FILE *fp;
    strcpy(buffer, fileName);
    strcat(buffer, ".ext");

    if (!externQueue.head) /* no entries */
        return 0;
    /* open file for writing */
    fp = fopen(buffer, "w");

    /* dequeue, write & free extern entries */
    while ((nodeP = (struct EntryNode *)externQueue.dequeue((void *)&externQueue)))
    {
        fprintf(fp, "%s %04d\n", nodeP->label, nodeP->address);

        free(nodeP);
    }

    return 1;
}

/* writes the .ent file
*  @param: fileName: char * - an extentionless filename
 * TODO: update return type to void.
 **/
int writeEntryFile(char *fileName)
{
    char buffer[MAX_FILENAME_LENGTH];
    struct EntryNode *removed;
    FILE *fp;
    strcpy(buffer, fileName);
    strcat(buffer, ".ent");

    if (!entryList.root) /* no entries */
        return 0;

    /* open file for writing */
    fp = fopen(buffer, "w");

    while ((removed = (struct EntryNode *)entryList.remove(&entryList)))
    {
        fprintf(fp, "%s %04d\n", removed->label, removed->address);
        /*  */
        free(removed);
    }

    return 1;
}

/* resets & clears data structures used for file assembling */
static void clear()
{ /* clear code */
    void *removed;
    while ((removed = (void *)codeDequeue()))
        free(removed);

    /* clear memory */
    while ((removed = memoryDequeue()))
        free(removed);

    /* clear extern files */
    while ((removed = externQueue.dequeue((void *)&externQueue)))
        free(removed);
    /* clear antry list */
    while ((removed = entryList.remove((void *)&entryList)))
        free(removed);

    /* clear labels hash table */
    clearHashTable();

    /* reset error flag */
    errorFlag = OFF;
    /* resets & clears all data & data structures related to the file */
}
