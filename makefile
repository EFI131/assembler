CC = gcc
CFLAGS = -Wall -ansi -pedantic
assembler:  assembler globals.o assembler.o file_parsing.o\
			line_parsing.o param_parsing.o memory.o label_hashtable.o code_queue.o\
			entry_linked_list.o extern_queue.o operations.o globals.o

			$(CC) $(CFLAGS) -o assembler globals.o assembler.o file_parsing.o\
			line_parsing.o param_parsing.o memory.o label_hashtable.o code_queue.o\
			entry_linked_list.o extern_queue.o operations.o globals.o

assembler.o:
	$(CC) $(CFLAGS) -c assembler.c

file_parsing.o: file_parsing.c label_hashtable.o
	$(CC) $(CFLAGS) -c file_parsing.c


line_parsing.o: line_parsing.c param_parsing.o param_parsing.o operations.o globals.o  
	$(CC) $(CFLAGS) -c line_parsing.c

param_parsing.o: param_parsing.c param_parsing.h globals.c globals.h
	$(CC) $(CFLAGS) -c param_parsing.c	

memory.o: memory.c memory.h 
	$(CC) $(CFLAGS) -c memory.c

label_hashtable.o: label_hashtable.c label_hashtable.h code_queue.o code_queue.h
	$(CC) $(CFLAGS) -c label_hashtable.c

code_queue.o: code_queue.c code_queue.h
	$(CC) $(CFLAGS) -c code_queue.c

entry_linked_list.o: entry_linked_list.c entry_linked_list.h linked_list.h common_structures.h
	$(CC) $(CFLAGS) -c entry_linked_list.c

extern_queue.o: extern_queue.c extern_queue.h queue.h common_structures.h
	$(CC) $(CFLAGS) -c extern_queue.c	

operations.o: operations.c operations.h param_parsing.c param_parsing.h
	$(CC) $(CFLAGS) -c operations.c	

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

.PHONY: clean, clean.o
clean:
	rm assembler *.o

clean.o:
	rm *.o



