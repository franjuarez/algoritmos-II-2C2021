#include <string.h>
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include "./TDAS/red_social.h"

/* Lee lo pasado por la entrada estandar
 * Pre: Recibe una cadena
 * Post: Se sobrescribe la cadena pasada por 
 * parámetro con lo recibido por la entrada estandar
*/
void leer_entrada(char** cadena);

/* Recibe una cadena y chequea a que comando de la
 * red social pertenece.
*/
void comandos(char* entrada, red_social_t* red_social);
