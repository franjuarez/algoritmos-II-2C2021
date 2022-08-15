#include "parseo.h"

static size_t CANTIDAD_PARAMETROS = 2;
// static size_t LONGITUD_ENTRADA = 100; //Igual getline usa el espacio necesario

int main(int argc, char* argv[]){
    if(argc != CANTIDAD_PARAMETROS){
        fprintf(stdout, "%s\n", "Error: Cantidad erronea de parametros"); 
        return EXIT_FAILURE; 
    }

    red_social_t* red_social = red_social_crear(argv[1]); 

    char* cadena;
    while(cadena != NULL){
        leer_entrada(&cadena);
        comandos(cadena, red_social);
        free(cadena);
    }
    red_social_destruir(red_social);
    return EXIT_SUCCESS;
}
