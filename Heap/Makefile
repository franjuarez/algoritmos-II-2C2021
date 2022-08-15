CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -c

all: go

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) heap.c

testing.o: testing.c testing.h
	$(CC) $(CFLAGS) testing.c

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) pila.c

pruebas_heap.o: testing.c pruebas_heap.c testing.h heap.h heap.c pila.c pila.h
	$(CC) $(CFLAGS) pruebas_heap.c

go: testing.o  pruebas_heap.o  heap.o pila.o
	$(CC) -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror testing.o heap.o pruebas_heap.o pila.o -o go

valgrind: 
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./go

clean:
	rm *.o