#include <stdlib.h> /* atoi, ...*/
#include <stdio.h>
#include "line_parsing.h"
#include "param_parsing.h"
#include "memory.h"
#include "label_hashtable.h"
#include "operations.h"
#include "globals.h"
#include "entry_linked_list.h"
#include "extern_queue.h"
#include "code_queue.h"

/** cheks if word is reserved
 * @param char *word
 * @return non zero if reserved, zero otherwise
 **/
int isWordReserved(char *word)
{
	return isOperation(word) || isDirective(word);
}

/** checks if word exists in labels table
 * @return non zero if exists, zero otherwise
 **/
int labelExists(char *word)
{
	return hashTableSearch(word) != NULL;
}

/** parseLine takes in a '\0' terminated string 
 * and puts into the peoper queues/tables the corresponding tokens 
 * an error is printed to stdout in case of one */
void parseLine(char *line)
{
	char buffer[MAX_WORD_LENGTH];
	char label[MAX_LABEL_LENGTH];
	char *pos = line;
	int n;
	int t;
	int labelFlag = OFF;
	struct DataItem *labelEntry = NULL;

	/* read first word */
	n = nextWord(buffer, pos);
	pos += n;

	/* early return for an empty line */
	if (!*buffer || !n)
	{
		return;
	}

	/* is it a label definition? */
	t = wordType(buffer);

	/* label definition */
	if (t == (LABEL | DEFINITION))
	{

		sscanf(buffer, "%[^:]", label); /* copy buffer to label buffer */

		if (isWordReserved(label)) /* check if label is reserved word*/
		{
			printf("Error in line: %d, invalid label definition, %s is a reserved word\n", lineNo, label);
		}
		else if (labelExists(label)) /* check if label is already defined */
		{
			printf("Error in line: %d, label %s already exsists\n", lineNo, label);
		}
		else
		{
			/* we keep apointer to the label such that we could modify it later */
			labelEntry = hashTableInsert(label);
		}

		labelFlag = ON;
		pos += nextWord(buffer, pos);
		t = wordType(buffer);
	}
	else if (t == COMMENT)
		return;

	/* .data */
	if (t == (DIRECTIVE | NUMERIC))
	{
		if (labelFlag && labelEntry)
		{
			/*Todo: unify data types */
			labelEntry->type = NUMERIC_LABEL;
			labelEntry->address = nextMemoryAddress();
		}

		/* loop over args */
		while ((n = nextWord(buffer, pos)))
		{
			/* check that it is a numeric type */
			if ((t = wordType(buffer)) == NUMERIC)
			{
				/*Todo: add a number validity check?insertion check?*/
				enqueueNumber(atoi(buffer));
			}
			else
			{
				errorFlag = ON;
				printf("Error in line %d: expected a number instead got %s \n", lineNo, buffer);
			}
			pos += n;
			pos += nextDelimiter(pos);

			if (*pos && *pos != ',')
			{
				errorFlag = ON;
				printf("Error in line %d: expected a ',' instead got: %c\n", lineNo, *pos);
			}
			else if (*pos)
				pos++;
		}
	}
	else if (t == (DIRECTIVE | STRING)) /* .string */
	{

		if (labelFlag && labelEntry) /* label defined */
		{
			/*Todo: unify data types */
			labelEntry->type = CHAR_LABEL;
			labelEntry->address = nextMemoryAddress();
		}
		/* .string */
		/* get string */
		n = nextWord(buffer, pos);
		/* push if correct */
		if ((t = wordType(buffer)) == STRING)
		{
			enqueueString(buffer);
			/* handle too many arguments */

			/* invalid string */
		}
		else if (t == (STRING | (ERROR << ERROR_OFFSET)))
		{
			printf("Error in line %d: invalid string: %s\n", lineNo, buffer);
		}
		/* expected a string error */
		else
		{

			printf("Error in line %d: expected a string instead got a: %s\n", lineNo, buffer);
			/* push to error queue */
		}
	}
	else if (t == (DIRECTIVE | ENTRY))
	{
		/* .entry treated on th 2nd passage */
		return;
	}
	else if (t == (DIRECTIVE | EXTERN))
	{

		/* loop over args */
		while ((n = nextWord(buffer, pos)))
		{

			/* check that it is a numeric type */
			if ((t = wordType(buffer)) == LABEL)
			{

				/* check for reserved word*/
				if (isWordReserved(buffer))
				{
					errorFlag = ON;
					printf("Error in line %d: %s ia a reserved word\n", lineNo, buffer);
				}
				else if (labelExists(buffer))
				{
					errorFlag = ON;
					printf("Error in line %d: label %s exists\n", lineNo, buffer);
				}
				else
				{
					/* we keep apointer to the label such that we could modify it later */
					labelEntry = hashTableInsert(buffer);
					labelEntry->type = EXTERN_LABEL;
				}
			}
			else
			{
				errorFlag = ON;
				printf("Error in line %d: expected a label instead got %s\n", lineNo, buffer);
			}
			pos += n;
			pos += nextDelimiter(pos);

			if (*pos && *pos != ',')
			{
				errorFlag = ON;
				printf("Error in line %d: expected a ',' instead got: %c\n", lineNo, *pos);
			}
			else if (*pos)
				pos++;
		}
	}
	else if (t == (DIRECTIVE | (ERROR << ERROR_OFFSET)))
	{
		/* bad directive... */
		errorFlag = ON;
		printf("Error in line %d: %s is not a valid directive\n", lineNo, buffer);
	}
	else if (t == OPERATION) /* operation */
	{
		enum OpCode opcode = getOpCode(buffer);
		struct OperationMetadata *omdp;
		struct QueueNode *opNodeP = NULL, *srcNodeP = NULL, *dstNodeP = NULL;

		/* label defined */
		if (labelFlag && labelEntry)
		{
			/*Todo: unify data types */
			labelEntry->type = CODE_LABEL;
			labelEntry->address = nextCodeAddress();
		}

		/* code queue node for the code queue */
		opNodeP = (struct QueueNode *)malloc(sizeof(struct QueueNode));

		/* get operation params metadata  */
		omdp = getOperationMetadata(opcode);
		/*Todo: write a function that encpsulstes node creation */
		/* set to operation */
		opNodeP->codeWord.type = t;
		opNodeP->codeWord.word.op.opcode = opcode;
		opNodeP->codeWord.word.op.are = A;
		opNodeP->codeWord.word.op.src = 0;
		opNodeP->codeWord.word.op.dst = 0;

		/* make sure n == 0 */
		n = 0;

		/** SRC PARAM **/
		/* look for src args */
		if ((omdp->src) && (n = nextWord(buffer, pos)))
		{
			srcNodeP = (struct QueueNode *)malloc(sizeof(struct QueueNode));
			pos += n;

			t = wordType(buffer);
			srcNodeP->codeWord.type = t; /*set word Type*/
			if ((omdp->src & immediate) && t == (NUMERIC | IMMEDIATE))
			{
				srcNodeP->codeWord.word.imm.are = A;
				srcNodeP->codeWord.word.imm.val = atoi(&buffer[1]);
				opNodeP->codeWord.word.op.src = immediate;
			}
			else if ((omdp->src & direct) && t == LABEL)
			{
				/* label will be replaced on the 2nd passage */
				strcpy(srcNodeP->label, buffer);
				srcNodeP->codeWord.word.dir.are = 0;	/* R/E depends on the label */
				opNodeP->codeWord.word.op.src = direct; /*TODO: REPLACE WITH A MACRO */
				/* NOTE: are field will get defined on the 2nd passage */
				srcNodeP->address = nextCodeAddress() + PROGRAM_OFFSET + 1;
			}
			else if ((omdp->src & directRegister) && t == REGISTER)
			{
				int n;
				sscanf(buffer, "r%d", &n);
				opNodeP->codeWord.word.op.src = directRegister;
				srcNodeP->codeWord.word.dirReg.are = A;
				srcNodeP->codeWord.word.dirReg.srcReg = n; /* atoi(&buffer[1]);*/
				srcNodeP->codeWord.word.dirReg.dstReg = 0;
				srcNodeP->codeWord.word.dirReg.padding = 0;
			}
			else if ((omdp->src & indirectRegister) && t == (INDIRECT | REGISTER))
			{
				int n;
				sscanf(buffer, "*r%d", &n);
				opNodeP->codeWord.word.op.src = indirectRegister;
				srcNodeP->codeWord.word.indirReg.are = A;
				srcNodeP->codeWord.word.dirReg.srcReg = n; /* atoi(&buffer[1]);*/
				srcNodeP->codeWord.word.dirReg.dstReg = 0;
				srcNodeP->codeWord.word.dirReg.padding = 0;
			}
			else
			{
				errorFlag = 1;
				/* expected X but got Y */
				printf("Error in line %d: unexpected param type %s\n", lineNo, buffer);
			}

			/* get delimiter */
			n = nextDelimiter(pos);
			pos += n;

			if (!*pos)
			{
				/* missing an argunent */
				errorFlag = 1;
				printf("Error in line %d: not enough arguments for operation\n", lineNo);
			}
			else if (*pos != ',')
			{
				/*error: expected a delimiter */
				errorFlag = 1;
				printf("Error in line %d: expected a ',' but got: \'%c\'\n", lineNo, *pos);
			}
			else
			{
				pos++;
			}
		}
		else if (omdp->src && !n)
		{
			/* missing an argunent */
			errorFlag = 1;
			printf("Error in line %d: not enough arguments for operation\n", lineNo);
		}

		/** DEST PARAM **/
		/* look for dest */

		if ((omdp->dst) && (n = nextWord(buffer, pos)))
		{
			t = wordType(buffer);
			pos += n;
			/* we first need to check wether the first argument was a register */
			/* we first need to check wether the first argument was a register */
			if (srcNodeP && (t == REGISTER || t == (INDIRECT | REGISTER)) && (srcNodeP->codeWord.type == REGISTER || srcNodeP->codeWord.type == (INDIRECT | REGISTER)))
			{
				dstNodeP = srcNodeP;
			}
			else
			{
				dstNodeP = (struct QueueNode *)malloc(sizeof(struct QueueNode));
			}

			dstNodeP->codeWord.type = t; /*set word Type*/
			if ((omdp->dst & immediate) && t == (NUMERIC | IMMEDIATE))
			{
				int n = atoi(&buffer[1]);
				dstNodeP->codeWord.word.imm.are = A; /*TODO: Replace with a A/R/E MACRO */
				dstNodeP->codeWord.word.imm.val = n;
				opNodeP->codeWord.word.op.dst = immediate; /* Replace with a macro */
			}
			else if ((omdp->dst & direct) && t == LABEL)
			{
				/* label will be replaced in 2nd passage */
				strcpy(dstNodeP->label, buffer);
				dstNodeP->codeWord.word.dir.are = 0;
				opNodeP->codeWord.word.op.dst = direct;
				dstNodeP->address = nextCodeAddress() + PROGRAM_OFFSET + (srcNodeP ? 2 : 1);
			}
			else if ((omdp->dst & directRegister) && t == REGISTER)
			{
				int n;
				sscanf(buffer, "r%d", &n);
				opNodeP->codeWord.word.op.dst = directRegister;

				dstNodeP->codeWord.word.dirReg.are = A;
				dstNodeP->codeWord.word.dirReg.dstReg = n; /*atoi(&buffer[1]);*/
			}
			else if ((omdp->dst & indirectRegister) && t == (INDIRECT | REGISTER))
			{
				int n;
				sscanf(buffer, "*r%d", &n);
				opNodeP->codeWord.word.op.dst = indirectRegister;

				dstNodeP->codeWord.word.dirReg.are = A;
				dstNodeP->codeWord.word.dirReg.dstReg = n; /*atoi(&buffer[1]);*/
			}
			else
			{
				/* expected X but got Y */
				errorFlag = 1;
				printf("Error in line %d: unexpected param type: %s\n", lineNo, buffer);
			}

			/* get delimiter */
			n = nextDelimiter(pos);
			pos += n;

			/* expectes end of line */
			if (*pos)
			{
				errorFlag = 1;
				printf("Error in line %d: excessive input, expected end of line instead got: %s\n", lineNo, pos);
			}
		}
		else if (omdp->dst && !n)
		{
			errorFlag = 1;
			/* missing an argunent */
			printf("Error in line %d: not enough arguments for operation\n", lineNo);
		}

		/* test for too many args */

		/* enqueue only if no error */
		if (!errorFlag)
		{
			codeEnqueue(opNodeP);

			if (srcNodeP)
			{
				if (!codeEnqueue(srcNodeP))
				{
					printf("message: in line %d:: enqueueing problem can't enqueue code word\n", lineNo);
				}
			}

			if (dstNodeP && dstNodeP != srcNodeP)
			{
				if (!codeEnqueue(dstNodeP))
				{
					printf("message: in line %d:: enqueueing problem can't enqueue code word\n", lineNo);
				}
			}
		}
	}
	else if (t == (OPERATION | (ERROR << ERROR_OFFSET)))
	{
		/*invalid  operation */
		errorFlag = ON;
		printf("Error in line %d: %s is an invalid operation\n", lineNo, buffer);
	}
	else
	{

		/* handle error */
		if (labelFlag)
		{
			errorFlag = ON;
			printf("Error in line %d: invalid token, expected a directive or an operation, \"%s\" is neither nor\n", lineNo, buffer);
		}
		else
		{
			errorFlag = ON;
			printf("Error in line %d: invalid token: expected a label definition, a directive, or an operation, \"%s\" is neither nor\n", lineNo, buffer);
		}
	}
}

/* parseEntries takes a line and updates the Label table as to which labels are entries and their corresponding addresses */
void parseEntryLine(char *line)
{
	char buffer[MAX_WORD_LENGTH];
	char *pos = line;
	int n, t;

	/* take in the first word */
	n = nextWord(buffer, pos);
	if (!n) /* empty line */
		return;

	pos += n;
	/* check if it is a .entry */
	if ((t = wordType(buffer)) == (DIRECTIVE | ENTRY))
	{
		/* look for label */
		n = nextWord(buffer, pos);
		pos += n;
		if ((t = wordType(buffer)) == LABEL)
		{
			struct DataItem *entryP;
			/* search for label */
			if ((entryP = hashTableSearch(buffer)))
			{
				if (entryP->type == EXTERN) /* conflicting decalrations */
				{
					errorFlag = ON; /* turn error flag on */
					printf("Error in line %d: conflicting declarations, %s can't be .entry and .extern at the same time\n", lineNo, buffer);
				}
				else
				{
					struct EntryNode *nodeP = malloc(sizeof(struct EntryNode));
					nodeP->address = entryP->address;
					strcpy(nodeP->label, entryP->key);
					entryP->type |= ENTRY_LABEL;
					/* insert into entry list */
					/* EXAMINATION */
					entryList.insert((void *)&entryList, nodeP);
				}
			}
			else /* label  not found */
			{
				errorFlag = ON;
				printf("Error in line %d: label \"%s\" is not defined\n", lineNo, buffer);
			}
		}
		else
		{ /* not a valid label */
			printf("Error in line %d: unexpected token, expected a label instead got: %s\n", lineNo, buffer);
			errorFlag = ON;
		}

		/* check for excessive params */
		n = nextDelimiter(pos);
		pos += n;
		if (*pos)
		{
			printf("Error in line %d: excessive input, expected end of line instead got: %s\n", lineNo, pos);
			errorFlag = ON;
		}
	}
}

/* replae label replaces a label with its corresponding address and adds an extern labels to the extern queue */
void replaceLabel(struct QueueNode *nodeP)
{

	struct DataItem *entryP;
	if (nodeP->codeWord.type == LABEL)
	{
		if ((entryP = hashTableSearch(nodeP->label)))
		{
			/* set are*/
			if (entryP->type == EXTERN_LABEL)
			{
				/* EXPIRMENTING */
				/* TODO: REPLACE WITH A NEW FUNCTION */
				/* enqueue onto extern queue */
				struct EntryNode *e = malloc(sizeof(struct EntryNode *));
				e->address = nodeP->address;
				strcpy(e->label, nodeP->label);
				externQueue.enqueue((void *)&externQueue, (void *)e);

				/* set A/R/E */
				nodeP->codeWord.word.dir.are = E;
			}
			else
			{
				nodeP->codeWord.word.dir.are = R;
				nodeP->codeWord.word.dir.address = entryP->address;
			}
		}
		else
		{
			errorFlag = ON;
			printf("Error in line %d: label \"%s\" is not defined\n", lineNo, nodeP->label);
		}
	}
}

/* update direct params traverses the code words and replaces the labels with theri corresponding addresses */
void updateDirectParams()
{
	traverseCodeQueue(replaceLabel);
}
