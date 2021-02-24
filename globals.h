

/* utility flags to help with types identification */
#define NUMERIC 1
#define IMMEDIATE 2
#define REGISTER 4
#define INDIRECT 8
#define LABEL 16
#define DEFINITION 32
#define DIRECTIVE 64
#define OPERATION 128
#define STRING 256
#define ENTRY 512
#define EXTERN 1024
#define COMMENT 2048

/* a/r/e bit flags */
#define A 4
#define R 2
#define E 1

/* */
#define PROGRAM_OFFSET 100
#define WORD_LENGTH 15
#define MAX_PROGRAM_MEMORY 4096

/* on off values for the error flag */
#define OFF 0
#define ON 1

/* will be set on upon an error */
extern int errorFlag;
extern int lineNo;
