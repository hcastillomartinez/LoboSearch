all: query

query:graph.h indexFiles.h hashtable.h query.c
	gcc -Wall -ansi -pedantic -g -o query graph.c indexFiles.c hashtable.c query.c 
clean:
	-rm query
