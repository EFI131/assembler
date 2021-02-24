#ifndef FILE_PARSING__H
#define FILE_PARSING__H

/** reads & assembles fileName.as , produces .ob, .ext, .ent upon successful assembling
 * prints an error message to stdout incase of such.
 * @param: fileName: char * - an extentionless filename, fileName.as assumed to exist.
 **/
void assemble(char *fileName);

/* TODO: consider declaring as static and moving into the .c file*/
int readLine(char *buffer, FILE *fp);

/** writes all output files: filename.ob, filename.ent, filename.ext
 *  @param: fileName: char * - an extentionless filename
 * TODO: update return type to void.
 **/
void writeOutFiles(char *fileName);

/* defines max legal no. of chars in line in our assembler program */
#define MAX_LINE_LENGTH 81

/* defines max significatnt no. of chars for a file name in our assembler program */
#define MAX_FILENAME_LENGTH 255

#endif