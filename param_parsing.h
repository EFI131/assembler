/* utilities and defninitions aa the single word level */
#include "globals.h"

#define ERROR 1
#define ERROR_OFFSET 12

/* scans and cleand preceding white space*/
int nextWord(char *buffer, char *line);

/* reads untill the next non space char returns number of chars red */
int nextDelimiter(char *line);

/** paramType accepts a trimmed pointer to a parameter string &
 * returns an int consisting of type flags indicating the param type,
 *  the bit in the ERROR_OFFSET bit will be onin case of an erroneous type */
int wordType(char *param);

#define N_OPERATION 16
/* returns 1 if argument provided is an operation, 0 otherwise */
int isOperation(char *param);

#define N_DIRECTIVE 4
/* returns 1 if argument provided is a directive, 0 otherwise */
int isDirective(char *param);
