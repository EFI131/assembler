#ifndef OPERATIONS__H
#define OPERATIONS__H
/* aadressing types */
enum AddressingType
{
    invalid = 0,
    immediate = 1,
    direct = 2,
    indirectRegister = 4,
    directRegister = 8
};

/* opCodes */
enum OpCode
{
    invalidOpCode = -1,
    mov = 0,
    cmp = 1,
    add,
    sub,
    lea,
    clr,
    not,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
};

/* holds valid addressing types */
struct OperationMetadata
{
    enum AddressingType src;
    enum AddressingType dst;
};

/* return the opcode corresponding to the string passed or -1 in case of an invalid one */
enum OpCode getOpCode(char *op);

/* gets an opcode and returns a corresponding OperationMetadata pointer */
struct OperationMetadata *getOperationMetadata(enum OpCode opcode);

#endif