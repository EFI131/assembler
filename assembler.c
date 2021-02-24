#include <stdio.h>
#include "file_parsing.h" /* parseFile function */

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        puts("no file supplied");
    }

    for (i = 1; i < argc; i++)
    {
        /* parse file */
        assemble(argv[i]);
    }

    return 0;
}
