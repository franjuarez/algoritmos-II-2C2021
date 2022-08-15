#ifndef _POSIX_C_SOURCE 
#define _POSIX_C_SOURCE 200809L //Para que ande getline
#include "parseo.h"
#endif


static size_t LONGITUD_ENTRADA = 0; //Para getline, puede redimensionar si fuese necesario
static char* COMANDO_DESCONOCIDO = "Error: Comando desconocido";

static void rstrip(char* cadena){
    char* caracter = "\n";
    size_t indice_caracter = strcspn(cadena, caracter);
    cadena[indice_caracter] = 0;
}

void leer_entrada(char** cadena){
    *cadena = NULL;
    if (getline(cadena, &LONGITUD_ENTRADA, stdin) == EOF){
        free(*cadena);
        *cadena = NULL; 
        return;
    }
    rstrip(*cadena);
}

void comandos(char* entrada, red_social_t* red_social){
    if(entrada == NULL) return;

    if (strcmp(entrada, "login") == 0){
        leer_entrada(&entrada);
        red_social_login(red_social, entrada);
        free(entrada);
    } else if (strcmp(entrada, "logout") == 0){
        red_social_logout(red_social);
    } else if(strcmp(entrada, "publicar") == 0){
        leer_entrada(&entrada);
        red_social_publicar(red_social, entrada);
        free(entrada);
    } else if (strcmp(entrada, "ver_siguiente_feed") == 0){
        red_social_ver_siguiente_feed(red_social);
    } else if (strcmp(entrada, "likear_post") == 0){
        leer_entrada(&entrada);
        red_social_likear_post(red_social, entrada);
        free(entrada);
    } else if (strcmp(entrada, "mostrar_likes") == 0){
        leer_entrada(&entrada);
        red_social_mostrar_likes(red_social, entrada);
        free(entrada);
    } else{
        fprintf(stdout, "%s\n", COMANDO_DESCONOCIDO);
    }
}
