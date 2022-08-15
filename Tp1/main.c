#include "fixcol.h"

#define PARAMETROS_MINIMOS 2
#define PARAMETROS_MAXIMOS 3

int isNumber(char s[]){ //Chequeo que sea un número.
    for (int i = 0; s[i]!= '\0'; i++){
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

int main(int argc, char* argv[]){
    if (argc < PARAMETROS_MINIMOS || argc > PARAMETROS_MAXIMOS || !isNumber(argv[1])){
        fprintf(stderr, "%s\n", "Error: Cantidad erronea de parametros");
        return EXIT_FAILURE; 
    }

    int buffer = (atoi(argv[1])); //Paso a entero el número dado.
    if (buffer < 1) return EXIT_FAILURE;
    
    if (argc < PARAMETROS_MAXIMOS) leer_escribir(stdin, (size_t) buffer);
    else usar_archivo(argv[2], (size_t) buffer);
    return EXIT_SUCCESS;
}
