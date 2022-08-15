CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -c

all: go

abb.o: abb.h abb.c pila.c pila.h
	$(CC) $(CFLAGS) abb.c

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) pila.c

testing.o: testing.c testing.h
	$(CC) $(CFLAGS) testing.c

pruebas_abb.o: abb.c abb.h testing.c testing.h pruebas_abb.c 
	$(CC) $(CFLAGS) pruebas_abb.c

go: testing.o abb.o pruebas_abb.o pila.o
	$(CC) -g -std=c99 -Wall -Wtype-limits -pedantic -Werror testing.o abb.o pruebas_abb.o pila.o -o go

valgrind: 
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./go

clean:
	rm *.o