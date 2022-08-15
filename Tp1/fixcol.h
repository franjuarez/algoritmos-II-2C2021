#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Lee el archivo recibido, que también puede ser la entrada standard, y
// printea por salida standard la cantidad de columnas pedidas.
// Pre: Recibe un archivo abierto (o stdin) y un número entero positivo (buffer).
// Post: Printea por salida standard lo de dentro del archivo (o lo escrito por 
// entrada standard) con las columnas que se piden por parámetro.
void leer_escribir(FILE* mi_archivo, size_t buffer);

// Se encarga de abrir, leer y cerrar pertinentemente un archivo recibido por parámetro. 
// Pre: Recibe el nombre del archivo y un número entero positivo (buffer)
// Post: Por salida standard printea lo de dentro del archivo, con las columnas
// que se piden por parámetro.
void usar_archivo(char* archivo, size_t buffer);
