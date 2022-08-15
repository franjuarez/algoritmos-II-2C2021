cleanCC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -c

all: go

lista.o: lista.h lista.c
	$(CC) $(CFLAGS) lista.c

hash.o: hash.c hash.h lista.c lista.h
	$(CC) $(CFLAGS) hash.c

testing.o: testing.c testing.h
	$(CC) $(CFLAGS) testing.c

hash_pruebas.o: hash.c testing.c hash_pruebas.c testing.h hash.h lista.c lista.h
	$(CC) $(CFLAGS) hash_pruebas.c

main.o: main.c testing.h testing.c
	$(CC) $(CFLAGS) main.c

go: testing.o hash.o hash_pruebas.o lista.o main.o
	$(CC) -g -std=c99 -Wall -Wtype-limits -pedantic -Werror main.o testing.o hash.o hash_pruebas.o lista.o -o go

valgrind: 
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./go

clean:
	rm *.o