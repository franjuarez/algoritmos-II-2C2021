CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -c

all: tp2

lista.o: ./TDAS/lista.h
	$(CC) $(CFLAGS) ./TDAS/lista.c

usuario.o: ./TDAS/usuario.c ./TDAS/usuario.h ./TDAS/heap.h ./TDAS/abb.h
	$(CC) $(CFLAGS) ./TDAS/usuario.c

heap.o: ./TDAS/heap.h
	$(CC) $(CFLAGS) ./TDAS/heap.c

pila.o: ./TDAS/pila.h
	$(CC) $(CFLAGS) ./TDAS/pila.c

abb.o: ./TDAS/abb.h ./TDAS/pila.h
	$(CC) $(CFLAGS) ./TDAS/abb.c

hash.o: ./TDAS/hash.h ./TDAS/lista.h
	$(CC) $(CFLAGS) ./TDAS/hash.c

red_social.o: ./TDAS/red_social.h ./TDAS/usuario.h ./TDAS/hash.h
	$(CC) $(CFLAGS) ./TDAS/red_social.c

parseo.o: parseo.h
	$(CC) $(CFLAGS) parseo.c

main.o: main.c parseo.h 
	$(CC) $(CFLAGS) main.c

tp2: main.o parseo.o abb.o usuario.o heap.o lista.o hash.o pila.o red_social.o
	$(CC) -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror main.o parseo.o abb.o usuario.o heap.o lista.o hash.o pila.o red_social.o -o tp2

valgrind:
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./tp2 usuarios.txt

clean:
	rm *.o
