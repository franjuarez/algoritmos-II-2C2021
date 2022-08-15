CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -c

all: go

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) pila.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) lista.c

testing.o: testing.c testing.h
	$(CC) $(CFLAGS) testing.c

pruebas_lista.o: lista.c testing.c pruebas_lista.c testing.h lista.h pila.h pila.c
	$(CC) $(CFLAGS) pruebas_lista.c

go: testing.o lista.o pruebas_lista.o pila.o
	$(CC) -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror testing.o lista.o pruebas_lista.o pila.o -o go

valgrind: 
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./go

clean:
	rm *.o