#include "heap.h"
#include "pila.h"
#include "testing.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

#define PURPLE "\033[1;35m" //Para escribir de este color 
#define RESET_COLOR "\x1b[0m" //Para reiniciar a blanco


//Funciones de comparación 

int strcmp_wrapper(const void* valor1,const void* valor2){
    return strcmp((char*)valor1, (char*)valor2);
}

int numcmp_wrapper(const void* valor1, const void* valor2){
    int* num1 = (int*) valor1;
    int* num2 = (int*) valor2;
    if(*num1 > *num2) return 1;
    else if(*num1 < *num2) return -1;
    else return 0;
}

int pilacmp_wrapper(const void* valor1, const void* valor2){
    int* tope1 = pila_ver_tope((pila_t*) valor1);
    int* tope2 = pila_ver_tope((pila_t*) valor2);
    if(*tope1 > *tope2)return 1;
    else if(*tope1 < *tope2) return -1;
    else return 0;
}

static void pruebas_heap_vacio(){
    printf(PURPLE "PRUEBAS SOBRE UNA HEAP VACÍO" RESET_COLOR "\n");
    heap_t* heap = heap_crear(strcmp_wrapper);

    print_test("Prueba heap vacío se creó correctamente", heap != NULL);
    print_test("Prueba heap al crear esta vacío", heap_esta_vacio(heap));
    print_test("Prueba heap no me deja desencolar", heap_desencolar(heap) == NULL);
    print_test("Prueba heap ver el máximo devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap cantidad devuelve 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);

    putchar('\n');
}

static void pruebas_heap_clave_vacia(){
    printf(PURPLE "PRUEBA CON CLAVE VACÍA\n" RESET_COLOR);
    heap_t* heap = heap_crear(strcmp_wrapper);

    char* clave = "";

    // Pruebo encolando y desencolando una clave vacía.
    print_test("Prueba heap encolar clave vacía", heap_encolar(heap, clave));
    print_test("Prueba heap el máximo es la clave vacía", heap_ver_max(heap) == clave);
    print_test("Prueba heap no esta vacío ", !heap_esta_vacio(heap));
    print_test("Prueba heap desencolar", heap_desencolar(heap) == clave);
    print_test("Prueba heap este vacío ", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
    
    putchar('\n');
}

static void pruebas_heap_invariante(){
    printf(PURPLE "PRUEBAS INVARIANTE DEL HEAP" RESET_COLOR "\n");    
    heap_t* heap = heap_crear(strcmp_wrapper);

    char* clave1 = "Hola";

    //Pruebo encolando y desencolando 1 elemento
    print_test("Prueba heap encolar 1 elemento",heap_encolar(heap, clave1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap máximo es clave1", heap_ver_max(heap) == clave1);
    print_test("Prueba heap desencolar es el máximo", heap_desencolar(heap) == clave1);
    print_test("Prueba heap esta vacío", heap_esta_vacio(heap));

    char* clave2 = "abc";
    char* clave3 = "zapallo";
    bool ok = true;
    
    //Pruebo encolando 2 elementos
    ok &= heap_encolar(heap, clave2);
    ok &= heap_encolar(heap, clave3);
    print_test("Prueba heap encolar 2 elementos",ok);
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap máximo es clave3", heap_ver_max(heap) == clave3);
    print_test("Prueba heap desencolar, devuelve la clave3", heap_desencolar(heap) == clave3);
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap nuevo máximo es clave2", heap_ver_max(heap) == clave2);
    print_test("Prueba heap desencolar, devuelve la clave restante, clave2", heap_desencolar(heap) == clave2);
    print_test("Prueba heap esta vacío", heap_esta_vacio(heap));
    
    char* clave4 = "apa";
    char* clave5 = "papa";
    char* clave6 = "la";

    //Pruebo encolando 3 elementos
    ok &= heap_encolar(heap, clave4);
    ok &= heap_encolar(heap, clave5);
    ok &= heap_encolar(heap, clave6);

    print_test("Prueba heap encolar 3 elementos",ok);
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap máximo es clave5", heap_ver_max(heap) == clave5);
    print_test("Prueba heap desencolar, devuelve la clave5", heap_desencolar(heap) == clave5);
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap nuevo máximo es clave6", heap_ver_max(heap) == clave6);
    print_test("Prueba heap desencolar, devuelve clave6", heap_desencolar(heap) == clave6);
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap nuevo máximo es clave4", heap_ver_max(heap) == clave4);
    print_test("Prueba heap desencolar, devuelve clave4", heap_desencolar(heap) == clave4);
    print_test("Prueba heap esta vacío", heap_esta_vacio(heap));
    
    heap_destruir(heap, NULL);
    
    putchar('\n');
}

static int* crear_arreglo_prueba(size_t n){
    int* arreglo = malloc(n * sizeof(int));
    for(int k = 0; k < n; k++){
        arreglo[k] = k;
    }
    return arreglo;
}

static void apilar_n(pila_t* pila, size_t n, int* arreglo){
    for(int i = 0; i < n; i++){
        pila_apilar(pila, &arreglo[i]);
    }
}

static void apilar_n_punteros(pila_t* pila, size_t n, int** arreglo){
    for(int i = 0; i < n; i++){
        pila_apilar(pila, arreglo[i]);
    }
}

void pila_destruir_wrapper(void* pila){
    pila_destruir((pila_t*)pila);
}

static void pruebas_heap_destruir(){
    printf(PURPLE "PRUEBAS DE DESTRUCCIÓN CON DISTINTOS DATOS DEL HEAP" RESET_COLOR "\n");
    heap_t* heap1 = heap_crear(numcmp_wrapper);
    heap_t* heap2 = heap_crear(pilacmp_wrapper);
    bool ok = true;
    
    //Pido esta memoria para después ver si se libera con free
    int* clave1 = malloc(sizeof(int));
    *clave1 = 4;
    int* clave2 = malloc(sizeof(int)); 
    *clave2 = 43;
    int* clave3 = malloc(sizeof(int));
    *clave3 = 5;
    
    //Pruebo encolando 2 elementos
    ok &= heap_encolar(heap1, clave1);
    ok &= heap_encolar(heap1, clave2);
    ok &= heap_encolar(heap1, clave3);
    print_test("Prueba heap encolar 3 punteros a enteros",ok);
    print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap1) == 3);
    print_test("Prueba heap máximo es clave2", heap_ver_max(heap1) == clave2);
    printf("Prueba destrucción con FREE, chequear Valgrind\n");

    //Se debe destruir con las cadenas dentro
    heap_destruir(heap1, free);

    //Creo pilas con distintos tamaños y topes;
    pila_t* pila1 = pila_crear();
    pila_t* pila2 = pila_crear();
    pila_t* pila3 = pila_crear();
    pila_t* pila4 = pila_crear();
    int* arreglo1 = crear_arreglo_prueba(5);
    int* arreglo2 = crear_arreglo_prueba(10);
    apilar_n(pila1, 5, arreglo1);
    apilar_n(pila2, 10, arreglo2);
    int valor1 = 7;
    pila_apilar(pila3, &valor1);
    int valor2 = 3; 
    pila_apilar(pila4, &valor2);
    
    ok &= heap_encolar(heap2, pila1);
    ok &= heap_encolar(heap2, pila2);
    ok &= heap_encolar(heap2, pila3);
    ok &= heap_encolar(heap2, pila4);
    print_test("Prueba heap encolar 4 pilas",ok);
    print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap2) == 4);
    print_test("Prueba heap máximo es la segunda pila", heap_ver_max(heap2) == pila2);

    //Se deben destruir las pilas que tiene dentro. 
    heap_destruir(heap2, pila_destruir_wrapper);
    printf("Prueba destrucción con PILA_DESTRUIR, chequear con Valgrind\n");
    
    free(arreglo1);
    free(arreglo2);
    putchar('\n');
}

static int** crear_arreglo_punteros(size_t n){
    int** arreglo = malloc(n * sizeof(int*));
    for(int k = 0; k < n; k++){
        int* numero = malloc(sizeof(int));
        *numero = rand(); 
        arreglo[k] = numero;
    }
    return arreglo;
}
static bool arreglo_comprobar_y_destruir(void** arreglo, size_t n, cmp_func_t cmp, void (*destruir)(void *e)){
    bool ok = true;
    for(size_t i = 0; i < n-1; i++){
        int comparacion = cmp(arreglo[i], arreglo[i+1]);
        if (comparacion > 0) ok = false;
        if (destruir) destruir(arreglo[i]);
    }
    if (destruir) destruir(arreglo[n-1]);
    return ok;
}

static void pruebas_heapsort(){
    printf(PURPLE "PRUEBAS HEAPSORT" RESET_COLOR "\n");
    
    //Prueba heapsort con números en orden aleatorio entre 0 y 9
    int** arr1 = crear_arreglo_punteros(10);
    heap_sort((void**) arr1, 10, numcmp_wrapper); 
    bool ordenado1 = arreglo_comprobar_y_destruir((void**) arr1, 10, numcmp_wrapper, free);
    
    print_test("Prueba Heapsort ordenar del 0 al 9", ordenado1);


    //Prueba heapsort con números aleatorios entre 0 y 100
    int** arr2 = crear_arreglo_punteros(100);
    heap_sort((void**) arr2, 100, numcmp_wrapper); 
    bool ordenado2 = arreglo_comprobar_y_destruir((void**) arr2, 100, numcmp_wrapper, free);

    print_test("Prueba Heapsort ordenar del 0 al 99", ordenado2);
    
    //Prueba heapsort con pilas 
    //Creo pilas con distintos tamaños y topes
    pila_t* pila1 = pila_crear();
    pila_t* pila2 = pila_crear();
    pila_t* pila3 = pila_crear();
    pila_t* pila4 = pila_crear();
    int** arreglo1 = crear_arreglo_punteros(5);
    int** arreglo2 = crear_arreglo_punteros(10);
    apilar_n_punteros(pila1, 5, arreglo1);
    apilar_n_punteros(pila2, 10, arreglo2);
    int valor1 = 34;
    pila_apilar(pila3, &valor1);
    int valor2 = 3; 
    pila_apilar(pila4, &valor2);

    pila_t* arr3[] = {pila3, pila4, pila1, pila2};
    heap_sort((void**) arr3, 4, pilacmp_wrapper); 
    bool ordenado3 = arreglo_comprobar_y_destruir((void**) arr3, 4, pilacmp_wrapper, pila_destruir_wrapper);     

    print_test("Prueba Heapsort ordenar pilas", ordenado3);

    for(int j = 0; j < 5; j++) free(arreglo1[j]);
    for(int k = 0; k < 10; k++) free(arreglo2[k]);
    free(arreglo1);
    free(arreglo2);    
    free(arr1);
    free(arr2);

    putchar('\n');
}

static bool arreglo_comprobar_volumen(heap_t* heap, size_t n, cmp_func_t cmp){
    void* anterior = heap_desencolar(heap);
    for(size_t i = 1; i < n; i++){
        void* actual = heap_desencolar(heap);
        int comparacion = cmp(actual, anterior); 
        free(anterior);
        if(comparacion > 0) return false;
        anterior = actual;
    }
    free(anterior);
    return true;
}

static void pruebas_heap_volumen(size_t n){
    printf(PURPLE "PRUEBAS HEAP VOLUMEN" RESET_COLOR "\n");
    heap_t* heap = heap_crear(numcmp_wrapper);
    int** arr = crear_arreglo_punteros(n);

    for(size_t i = 0; i < n; i++) heap_encolar(heap, arr[i]);

    print_test("Prueba heap encolo los elementos", heap_cantidad(heap) == n);

    bool ok = arreglo_comprobar_volumen(heap, n, numcmp_wrapper);

    printf("Prueba heap volumen con %ld elementos", n);
    print_test("", ok);

    heap_destruir(heap, free); //Por si falla la prueba, al destruir el heap libero lo que queda
    free(arr);
    putchar('\n');
}

static void pruebas_heap_volumen_arr(size_t n){
    printf(PURPLE "PRUEBAS HEAP VOLUMEN CON ARREGLO" RESET_COLOR "\n");
    
    int** arr1 = crear_arreglo_punteros(n);
    heap_t* heap1 = heap_crear_arr((void**) arr1, n, numcmp_wrapper);
    print_test("Prueba heap encolo los elementos", heap_cantidad(heap1) == n);
    bool ok1 = arreglo_comprobar_volumen(heap1, n, numcmp_wrapper); 

    printf("Prueba heap volumen con %ld elementos", n);
    print_test("", ok1);

    heap_destruir(heap1, NULL);
    free(arr1);
    putchar('\n');
}

void pruebas_heap_estudiante(){
    /* Ejecuta todas las pruebas unitarias. */
    pruebas_heap_vacio();
    pruebas_heap_clave_vacia();
    pruebas_heap_invariante();
    pruebas_heap_destruir();
    pruebas_heapsort();
    pruebas_heap_volumen(50); 
    pruebas_heap_volumen_arr(50); 
}

#ifndef CORRECTOR
int main(){
    pruebas_heap_estudiante();
    return failure_count() > 0;
}
#endif

