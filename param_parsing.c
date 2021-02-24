#include "param_parsing.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/** paramType accepts a trimmed pointer to a parameter string &
 * returns an int consisting of type flags indicating the param type, the bit in the ERROR_OFFSET bit will be onin case of an erroneous type */
int wordType(char *param)
{
	char *pos; /* position in line */
	int type = 0;

	pos = param;

	/* looking for a preceding punctuation chars that may help define the param type*/
	if (ispunct(*pos))
	{
		switch (*pos)
		{
		case '#':
			type = IMMEDIATE;
			pos++;

		case '+':
		case '-':
			type |= NUMERIC;
			pos++;
			break;

		case '*':
			type = INDIRECT;
			pos++;
			break;

		case '.':
			type = DIRECTIVE;
			pos++;
			break;

		case '\"':
			type = STRING;
			pos++;
			break;

		case ';':
			type = COMMENT;
			pos++;
			break;
		}
	}

	if (type == COMMENT)
		return type; /* the rest is irrelevant*/

	else if ((type & NUMERIC) || (!type && isdigit(*pos)))
	{ /* either numeric data or immediate param */
		type |= NUMERIC;
		while (isdigit(*pos))
			pos++;
	}
	else if (type & STRING)
	{
		while (*pos && *pos != '\"')
			pos++; /* look for closing quotes */

		if (*pos == '\"')
			pos++;
	}
	else if ((type & INDIRECT) || (type & DIRECTIVE) || isalpha(*pos))
	{
		/* directive */
		if (type == DIRECTIVE)
		{ /* find which directive or set an error*/
			if (!strcmp("data", pos))
			{
				pos += 4;
				type |= NUMERIC;
			}
			else if (!strcmp("entry", pos))
			{
				pos += 5;
				type |= ENTRY;
			}
			else if (!strcmp("extern", pos))
			{
				pos += 6;
				type |= EXTERN;
			}
			else if (!strcmp("string", pos))
			{
				pos += 6;
				type |= STRING;
			}
			else
			{
				printf("invalid directive: %s\n", param);
				type |= ERROR << ERROR_OFFSET;
			}
		}
		/* register */
		else if (*pos == 'r' && (*++pos >= '0' && *pos <= '7') && !*++pos)
		{
			type |= REGISTER;
		}
		/* label, label definition, operation */
		if (*pos && !(type & (INDIRECT | REGISTER)) && !(type & DIRECTIVE))
		{
			while (isalnum(*pos))
				pos++;

			if (*pos == ':')
			{
				type |= DEFINITION;
				pos++;
			}
			/* operation */
			if (isOperation(param))
				type |= OPERATION;
			else /* label */
				type |= LABEL;
		}
	}
	else
	{
		type |= ERROR << ERROR_OFFSET;
	}

	/* set errors */
	if (type == STRING)
	{
		/* look for closing " */
		if (*pos)
		{
			printf("expected a \", instead got %c\n", *pos);
			type |= ERROR << ERROR_OFFSET;
		}
	}

	/* excess input */
	if (*pos)
	{
		type |= ERROR << ERROR_OFFSET;
	}

	return type;
}

/* reads next word, cleans preceding white spaces, returns the number of chars red */
int nextWord(char *buffer, char *line)
{
	char *pos = line;
	while (isspace(*pos))
		pos++;

	if (*pos == '\"')
	{
		*buffer = *pos;
		while ((*++buffer = *++pos) && *buffer != '\"')
			;
		if (*buffer == '\"')
		{
			buffer++;
			pos++;
		}
	}
	else
	{
		while (!isspace((*buffer = *pos)) && *buffer && *buffer != ',')
		{
			buffer++;
			pos++;
		}
	}

	*buffer = '\0';

	return pos - line;
}

/* reads untill the next non space char returns number of chars red */
int nextDelimiter(char *line)
{
	char *pos = line;

	while (isspace(*pos))
		pos++;

	return pos - line;
}

/* list of machine operations */
char *operationList[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
						 "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

/* returns 1 if a valid operation was found*/
int isOperation(char *param)
{
	int i;
	for (i = 0; i < N_OPERATION; i++)
		if (!strcmp(operationList[i], param))
			return 1;

	return 0;
}

/* list of directives */
char *directiveList[] = {".data", ".string", ".entry", ".extern"};

/* returns 1 if argument provided is a directive, 0 otherwise */
int isDirective(char *param)
{
	int i;
	for (i = 0; i < N_DIRECTIVE; i++)
		if (!strcmp(directiveList[i], param))
			return 1;

	return 0;
}
