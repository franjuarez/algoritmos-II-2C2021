#include "cola.h"
#include "pila.h" //Para las pruebas de destruir
#include "testing.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

#define ANSI_COLOR_ORANGE  "\033[0;34m" //Para escribir de este color 
#define ANSI_COLOR_RESET   "\x1b[0m" //Para reiniciar a blanco

#define VALOR_RANDOM_1 12
#define VALOR_RANDOM_2 339
#define VALOR_RANDOM_3 111
#define VALOR_RANDOM_4 3         // Defino valores Random para usar más adelante en las pruebas.
#define VALOR_RANDOM_5 7213
#define VALOR_RANDOM_6 1566
#define VALOR_RANDOM_7 23
#define VALOR_RANDOM_8 324
#define CHAR_RANDOM 'j'
#define CHAR_RANDOM_2 'P'
#define CHAR_RANDOM_3 'Y'
#define DOUBLE_RANDOM 32.1 
#define DOUBLE_RANDOM_2 10.239 

#define CANTIDAD_VOLUMEN 10000


static void pruebas_cola_vacia(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nEMPIEZO PRUEBAS CON UNA COLA VACÍA\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    cola_t* cola = cola_crear();
    // Chequeos con cola vacía.

    print_test("Chequeo si se creo exitosamente", cola !=NULL);
    print_test("Chequeo que al crear la cola esté vacía", cola_esta_vacia(cola));
    print_test("Chequeo que no me deje ver el primero ya que está vacía", cola_ver_primero(cola) == NULL);
    print_test("Chequeo que no me deje desencolar en la cola recién creada", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
}

static void prueba_encolar_NULL(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nPRUEBA CON ELEMENTO NULL\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    cola_t* cola = cola_crear();

    // Pruebo encolando y desencolando NULL.
    print_test("Chequeo que se puede encolar NULL", cola_encolar(cola, NULL) == true);
    print_test("Chequeo que se pueda ver el NULL", cola_ver_primero(cola) == NULL);
    print_test("Chequeo que la pila NO está vacía ", !cola_esta_vacia(cola));
    print_test("Chequeo que se pueda desencolar el NULL", cola_desencolar(cola) == NULL);
    print_test("Chequeo que la cola SI está vacía ", cola_esta_vacia(cola));

    cola_destruir(cola, NULL);
}

static void pruebas_invariante(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nEMPIEZO A ENCOLAR PARA VER SI SE CUMPLE LA INVARIANTE\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    //Creo la cola y un bool para saber si todo está bien
    cola_t* cola = cola_crear();
    bool todo_ok = true;

    // Empiezo la prueba encolando un solo elemento
    int valor_primer_prueba = VALOR_RANDOM_1;

    //Chequeo que se encole bien
    todo_ok &= cola_encolar(cola, &valor_primer_prueba);
    print_test("Encolo 1 elemento",todo_ok);
    print_test("Chequeo que sea el mismo", cola_desencolar(cola) == &valor_primer_prueba);
    print_test("Chequeo que esté vacía", cola_esta_vacia(cola));

    // Pruebo encolando 2 elementos en la misma cola.
    int valor_1_segunda = VALOR_RANDOM_2;
    int valor_2_segunda = VALOR_RANDOM_3;

    //Chequeo que se encolen bien
    todo_ok &= cola_encolar(cola, &valor_1_segunda);
    todo_ok &= cola_encolar(cola, &valor_2_segunda);
    print_test("Encolo 2 elementos",todo_ok);
    print_test("Chequeo que sean los mismos", cola_desencolar(cola) == &valor_1_segunda && cola_desencolar(cola) == &valor_2_segunda);
    print_test("Chequeo que esté vacía", cola_esta_vacia(cola));

    // Pruebo encolando ahora 3 elementos en la misma cola.
    int valor_1_tercera = VALOR_RANDOM_4;
    int valor_2_tercera = VALOR_RANDOM_5;
    int valor_3_tercera = VALOR_RANDOM_6;

    //Chequeo que se encolen bien
    todo_ok &= cola_encolar(cola, &valor_1_tercera);
    todo_ok &= cola_encolar(cola, &valor_2_tercera);
    todo_ok &= cola_encolar(cola, &valor_3_tercera);
    print_test("Encolo 3 elementos",todo_ok);
    print_test("Chequeo que sean los mismos", cola_desencolar(cola) == &valor_1_tercera && cola_desencolar(cola) == &valor_2_tercera && cola_desencolar(cola) == &valor_3_tercera);
    print_test("Chequeo que esté vacía", cola_esta_vacia(cola));

    cola_destruir(cola, NULL);
}

static void prueba_volumen(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nEMPIEZO PRUEBA DE VOLUMEN CON 100000 ELEMENTOS\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco

    //Creo la cola, un bool para saber si todo está bien
    // Y una lista para guardar los elementos y usar diferentes
    // Punteros en cada caso al encolar.
    cola_t* cola = cola_crear();
    bool todo_ok = true;
    int lista[CANTIDAD_VOLUMEN+1];

    //Encolo muchos elementos, usando una lista así los punteros son diferentes.
    for(int i = 0; i<=CANTIDAD_VOLUMEN;i++){
        lista[i] = i;
        todo_ok &= cola_encolar(cola,&lista[i]);
    }
    
    print_test("Veo si se encolaron todos bien", todo_ok);

    //Desencolo y chequeo uno a uno que sean iguales.
    for(int j = 0; j<=CANTIDAD_VOLUMEN;j++){
        int valor_esperado = j;
        int valor_obtenido = *(int*) cola_desencolar(cola);
        todo_ok &= (valor_obtenido == valor_esperado);
        if (cola_esta_vacia(cola)) break;
        todo_ok &= *(int*) cola_ver_primero(cola) == j + 1; //Chequeo que el primero de la cola sea el correcto en cada paso.
        if (!todo_ok) break; //Si es false no tiene sentido seguir chequeando.
    }

    //Chequeo que todos los valores hayan sido los mismos 
    //y que cumpla las condiciones de estar vacía.  
    print_test("Veo si todos los valores desencolados son iguales a los encolados", todo_ok);
    print_test("Veo que esté vacía", cola_esta_vacia(cola));
    print_test("Chequeo que NO se pueda ver el primero ya que está vacía", cola_ver_primero(cola) == NULL);
    print_test("Chequeo que NO se pueda desencolar ya que está vacía", cola_desencolar(cola) == NULL);

    //Pruebo encolando dos valores más, para ver si se sigue cumpliendo la invariante de la cola.
    int valor_prueba_1 = VALOR_RANDOM_7;
    int valor_prueba_2 = VALOR_RANDOM_8;
    cola_encolar(cola,&valor_prueba_1);
    cola_encolar(cola,&valor_prueba_2);
    print_test("Chequeo que después de encolar tantos elementos se comporte como una cola recién creada", cola_desencolar(cola) == &valor_prueba_1 && cola_desencolar(cola) == &valor_prueba_2);
    
    cola_destruir(cola, NULL);
}

static void prueba_free(){
    cola_t* cola = cola_crear();
    int* punt_entero = malloc(sizeof(int));
    *punt_entero = VALOR_RANDOM_4;
    char* punt_char = malloc(sizeof(char));  //Pido esta memoria para después ver si se libera con free
    *punt_char = CHAR_RANDOM;
    double* punt_double = malloc(sizeof(double));
    *punt_double = DOUBLE_RANDOM;
    cola_encolar(cola,punt_entero);
    cola_encolar(cola,punt_char);
    cola_encolar(cola,punt_double);
    cola_destruir(cola, free);
    
    printf("Prueba con FREE, chequear con Valgrind que no se pierda memoria\n");
    //Valgrind no tira ningún error, el mensaje lo dejo como parte de las pruebas
}

static void prueba_NULL(){    
    cola_t* cola = cola_crear();
    int valor = VALOR_RANDOM_1;
    char caracter = CHAR_RANDOM_2;
    float con_coma = VALOR_RANDOM_2;
    cola_encolar(cola,&valor);
    cola_encolar(cola,&caracter);
    cola_encolar(cola,&con_coma);
    cola_destruir(cola, NULL);

    printf("Prueba con NULL, chequear con Valgrind que no se pierda memoria\n");
    //Valgrind no tira ningún error, el mensaje lo dejo como parte de las pruebas
}

void pila_destruida(void* pila){ //Wrapper así cumple con la firma de la función.
    pila_t* nueva_pila = (pila_t*) pila;
    pila_destruir(nueva_pila);
}

static void prueba_con_pila(){
    cola_t* cola = cola_crear();

    //Creo y apilo en una primera pila
    pila_t* pila0 = pila_crear();
    int valor0 = VALOR_RANDOM_6;
    pila_apilar(pila0, &valor0);

    //Repito para dos pilas más
    pila_t* pila1 = pila_crear();
    char valor1 = CHAR_RANDOM_3;
    pila_apilar(pila1, &valor1);
    pila_t* pila2 = pila_crear();
    double valor2 = DOUBLE_RANDOM_2;
    pila_apilar(pila2, &valor2);

    //Encolo todas las pilas en la cola
    cola_encolar(cola, pila0);
    cola_encolar(cola, pila1);
    cola_encolar(cola, pila2);
    cola_destruir(cola, pila_destruida);

    printf("Prueba con PILA_DESTRUIR, chequear con Valgrind que no se pierda memoria\n"); 
    //Valgrind no tira ningún error, el mensaje lo dejo como parte de las pruebas
    putchar('\n');
}

static void prueba_destruccion_diferentes_maneras(){
    printf(ANSI_COLOR_ORANGE); // Color
    printf("\nPRUEBAS DE DESTRUCCION DE DIFERENTES MANERAS\n");
    printf(ANSI_COLOR_RESET); // Vuelvo a Blanco
    prueba_free();
    prueba_NULL();
    prueba_con_pila();
}

void pruebas_cola_estudiante(){
    pruebas_cola_vacia();
    prueba_encolar_NULL();
    pruebas_invariante();
    prueba_volumen();
    prueba_destruccion_diferentes_maneras();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif