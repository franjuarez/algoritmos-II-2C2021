#include "fixcol.h"

void leer_escribir(FILE* mi_archivo, size_t buffer){
    char cadena[buffer+1]; //Le sumo uno por el \0.
    while (fgets(cadena, (int) (buffer+1), mi_archivo) != NULL){
        size_t longitud = strlen(cadena);
        if (longitud <= 0 || cadena[0] == '\n') continue;
        if (longitud < buffer || cadena[buffer-1] == '\n'){ 
            printf("%s", cadena);
            continue;
        }
        printf("%s\n", cadena); 
    }
}

void usar_archivo(char* archivo, size_t buffer){
    FILE* mi_archivo = fopen(archivo, "r");
    if (!mi_archivo){
        fprintf(stderr, "%s\n", "Error: archivo fuente inaccesible");
        return;
    }
    leer_escribir(mi_archivo, buffer);
    fclose(mi_archivo); 
}
