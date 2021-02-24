#include "operations.h"
#include "param_parsing.h" /*TODO: MOVE TYPES TO ANOTHER HEADER */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* holds strutcs indicating the legal addressing types for each operation, opcodes correspond with the Metadata object at that index */
struct OperationMetadata operationMetadataList[] = {
    {/* 0 mov */
     immediate | direct | indirectRegister | directRegister,
     direct | indirectRegister | directRegister},
    {/* 1 cmp */
     immediate | direct | indirectRegister | directRegister,
     immediate | direct | indirectRegister | directRegister},
    {/* 2 add */
     immediate | direct | indirectRegister | directRegister,
     direct | indirectRegister | directRegister},
    {/* 3 sub */
     immediate | direct | indirectRegister | directRegister,
     direct | indirectRegister | directRegister},
    {/* 4 lea */
     direct,
     direct | indirectRegister | directRegister},
    {/* 5 clr */
     0,
     direct | indirectRegister | directRegister},
    {/* 6 not */
     0,
     direct | indirectRegister | directRegister},
    {/* 7 inc */
     0,
     direct | indirectRegister | directRegister},
    {/* 8 dec */
     0,
     direct | indirectRegister | directRegister},
    {/* 9 jmp */
     0,
     direct | indirectRegister},
    {/* 10 bne */
     0,
     direct | indirectRegister},
    {/* 11 red */
     0,
     direct | indirectRegister | directRegister},
    {/* 12 prn */
     0,
     immediate | direct | indirectRegister | directRegister},
    {/* 13 jsr */
     0,
     direct | indirectRegister},
    {/* 14 rts */
     0,
     0},
    {/* 15 stop */
     0,
     0}};

enum OpCode getOpCode(char *op)
{

    if (!strcmp(op, "mov"))
        return mov;
    else if (!strcmp(op, "cmp"))
        return cmp;

    else if (!strcmp(op, "add"))
        return add;

    else if (!strcmp(op, "sub"))
        return sub;

    else if (!strcmp(op, "lea"))
        return lea;

    else if (!strcmp(op, "clr"))
        return clr;

    else if (!strcmp(op, "not"))
        return not;

    else if (!strcmp(op, "inc"))
        return inc;

    else if (!strcmp(op, "dec"))
        return dec;

    else if (!strcmp(op, "jmp"))
        return jmp;

    else if (!strcmp(op, "bne"))
        return bne;

    else if (!strcmp(op, "red"))
        return red;

    else if (!strcmp(op, "prn"))
        return prn;

    else if (!strcmp(op, "jsr"))
        return jsr;

    else if (!strcmp(op, "rts"))
        return rts;

    else if (!strcmp(op, "stop"))
        return stop;

    return invalidOpCode;
}

/**
 *  @param opCode: an enum of the various opcodes
 *  @return OperationMetadata* holding the valid addressing types for the operation's parameters*/
struct OperationMetadata *getOperationMetadata(enum OpCode opCode)
{

    if (opCode == invalidOpCode)
        return NULL;

    return &operationMetadataList[opCode];
}
