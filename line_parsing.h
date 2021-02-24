#ifndef LINE_PARSING__H
#define LINE_PARSING__H

#define MAX_WORD_LENGTH 81
#define MAX_LABEL_LENGTH 31

/* goes over a line determines it's token tyeps and pushes to crresponding data/code datastructures */
void parseLine(char *line);

/* goes over .entry directive lines and adds those to the corrsponding data structure */
void parseEntryLine(char *line);

/* itearates over cod elines and updates the addresses once both code and data sizes were deined */
void updateDirectParams();

#endif