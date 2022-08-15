#include "pila.h"
#include "testing.h"
#include "stdio.h"
#include "stdbool.h"

#define ANSI_COLOR_ORANGE "\033[0;34m" //Para escribir de este color 
#define ANSI_COLOR_RESET   "\x1b[0m" //Para reiniciar a blanco

#define VALOR_RANDOM_1 12
#define VALOR_RANDOM_2 339
#define VALOR_RANDOM_3 3
#define VALOR_RANDOM_4 111        // Defino valores Random para usar más adelante en las pruebas.
#define VALOR_RANDOM_5 7213
#define VALOR_RANDOM_6 1566
#define VALOR_RANDOM_7 23
#define VALOR_RANDOM_8 324

#define CANTIDAD_VOLUMEN 10000


static void prueba_pila_vacia(void) {
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nEMPIEZO PRUEBAS CON UNA PILA VACÍA\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    pila_t *pila = pila_crear();

    //Chequeos con Pila vacía
    print_test("Chequeo que se cree exitosamente", pila!=NULL);
    print_test("Veo si la pila se crea vacía",pila_esta_vacia(pila));
    print_test("Chequeo que NO se pueda ver el tope ya que recién la cree", pila_ver_tope(pila) == NULL);
    print_test("Chequeo que NO se pueda desapilar ya que recién la cree", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
}

static void prueba_NULL(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf ("\nPRUEBA CON ELEMENTO NULL\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    pila_t *pila = pila_crear();

    //Prueba apilando y desapilando NULL.
    print_test("Chequeo que se puede apilar NULL", pila_apilar(pila,NULL) == true);
    print_test("Chequeo que se pueda ver el NULL", pila_ver_tope(pila) == NULL);
    print_test("Chequeo que la pila NO está vacía ", !pila_esta_vacia(pila));
    print_test("Chequeo que se pueda desapilar el NULL", pila_desapilar(pila) == NULL);
    print_test("Chequeo que la pila SI está vacía ", pila_esta_vacia(pila));

    pila_destruir(pila);
}

static void prueba_pila_apilar(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nPRUEBA DE INVARIANTE DE LA PILA\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    //Creo la pila y un bool para ir chequeando si todo está bien
    pila_t* pila = pila_crear();
    bool todo_ok = true;

    //Empiezo apilando 1 solo elemento
    int valor_primer_prueba = VALOR_RANDOM_1;

    //Chequeo que en apile bien
    todo_ok &= pila_apilar(pila, &valor_primer_prueba);
    print_test("Apilo 1 elemento",todo_ok);
    print_test("Chequeo que sea el mismo", pila_desapilar(pila) == &valor_primer_prueba);
    print_test("Chequeo que esté vacía", pila_esta_vacia(pila));
    
    // Pruebo apilando 2 elementos en la misma pila de antes
    int valor_1_segunda = VALOR_RANDOM_2;
    int valor_2_segunda = VALOR_RANDOM_3;

    //Cheequeo que se apilen bien
    todo_ok &= pila_apilar(pila, &valor_1_segunda);
    todo_ok &= pila_apilar(pila, &valor_2_segunda);
    print_test("Apilo 2 elementos",todo_ok);
    print_test("Chequeo que sean los mismos", pila_desapilar(pila) == &valor_2_segunda && pila_desapilar(pila) == &valor_1_segunda);
    print_test("Chequeo que esté vacía", pila_esta_vacia(pila));

    // Pruebo apilando 3 elementos en la misma pila que antes
    int valor_1_tercera = VALOR_RANDOM_4;
    int valor_2_tercera = VALOR_RANDOM_5;
    int valor_3_tercera = VALOR_RANDOM_6;

    // Chequeo que se apilen bien
    todo_ok &= pila_apilar(pila, &valor_1_tercera);
    todo_ok &= pila_apilar(pila, &valor_2_tercera);
    todo_ok &= pila_apilar(pila, &valor_3_tercera);
    print_test("Apilo 3 elementos",todo_ok);
    print_test("Chequeo que sean los mismos", pila_desapilar(pila) == &valor_3_tercera && pila_desapilar(pila) == &valor_2_tercera && pila_desapilar(pila) == &valor_1_tercera);
    print_test("Chequeo que esté vacía", pila_esta_vacia(pila));

    pila_destruir(pila); 
}

static void prueba_volumen(void){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nEMPIEZO PRUEBA DE VOLUMEN CON %d ELEMENTOS\n", CANTIDAD_VOLUMEN);
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    //Creo la pila, también un bool para saber si todo está bien
    // Y por último una lista para guardar los elementos y usar diferentes
    // Punteros en cada caso al apilar.
    pila_t* pila = pila_crear();
    bool todo_ok = true;
    int lista[CANTIDAD_VOLUMEN+1];

    // Apilo muchos elementos, usando una lista así los punteros son diferentes.
    for(int i = 0; i<=CANTIDAD_VOLUMEN;i++){
        lista[i] = i ;
        todo_ok &= pila_apilar(pila,&lista[i]);
    }

    print_test("Veo si se apilaron todos bien", todo_ok);

    //Desapilo y chequeo uno a uno que sean iguales.
    for(int j = CANTIDAD_VOLUMEN; j>=0;j--){
        int valor_esperado = j;
        int valor_obtenido = *(int*) pila_desapilar(pila);
        todo_ok &= (valor_obtenido == valor_esperado);
        if (pila_esta_vacia(pila)) break;
        todo_ok &= *(int*) pila_ver_tope(pila) == j - 1; //Chequeo que el tope de la pila sea el correcto en cada paso.
        if (!todo_ok) break; //Si es false no tiene sentido seguir chequeando.
    }

    // Chequeo que todos los valores hayan sido los mismos
    // y que se cumpla las condiciones de estar vacía
    print_test("Veo si todos los valores desapilados son iguales a los apilados", todo_ok);
    print_test("Veo que esté vacía", pila_esta_vacia(pila));
    print_test("Chequeo que NO se pueda ver el tope ya que está vacía", pila_ver_tope(pila) == NULL);
    print_test("Chequeo que NO se pueda desapilar ya que está vacía", pila_desapilar(pila) == NULL);

    //Pruebo apilando dos valores más, para ver si se sigue cumpliendo la invariante de la pila.
    int valor = VALOR_RANDOM_7;
    int valor1 = VALOR_RANDOM_8; //Elijo valores random para la prueba
    pila_apilar(pila, &valor);
    pila_apilar(pila, &valor1);
    print_test("Chequeo que después de apilar tantos elementos se comporte como una pila recién creada", pila_desapilar(pila) == &valor1 && pila_desapilar(pila) == &valor);

    pila_destruir(pila);
    putchar('\n');
}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_NULL();
    prueba_pila_apilar();
    prueba_volumen();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
