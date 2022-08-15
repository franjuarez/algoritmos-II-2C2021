#include "lista.h"
#include "pila.h"
#include "testing.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ctype.h"

#define PURPLE "\033[1;35m" //Para escribir de este color 
#define RESET_COLOR "\x1b[0m" //Para reiniciar a blanco

typedef bool (*funcion_insertar)(lista_t* lista, void* dato);
typedef void* (*funcion_ver_lista)(const lista_t* lista);

static void pruebas_lista_vacia(void){
    printf(PURPLE "PRUEBAS SOBRE UNA LISTA VACÍA" RESET_COLOR "\n");

    //lista a utilizar
    lista_t* lista = lista_crear();

    //Veo que esté vacía la lista recién creada
    print_test("Chequeo que al crear la lista esté vacía", lista_esta_vacia(lista));

    //Intento borrar de una lista recién creada y devuelve NULL
    print_test("Chequeo que no me deje borrar en la lista recién creada", lista_borrar_primero(lista) == NULL);

    //Intento ver_primero en una lista vacía y devuelve NULL
    print_test("ver_primero en una lista vacía devuelve NULL", lista_ver_primero(lista) == NULL);
    
    //Intento ver_ultimo en una lista vacía y devuelve NULL
    print_test("ver_ultimo en una lista vacía devuelve NULL", lista_ver_ultimo(lista) == NULL);

    //Destruyo la lista utilizada
    lista_destruir(lista, NULL);

    putchar('\n');
}

static void prueba_ver_primero_y_ultimo(void){
    printf(PURPLE "PRUEBA DE VER PRIMERO EN UNA LISTA" RESET_COLOR "\n");

    //lista a utilizar
    lista_t* lista = lista_crear();

    //Inserto NULL como primer elemento y compruebo que es efectivamente el primero
    lista_insertar_primero(lista, NULL);
    print_test("Insertar NULL primero hace que sea el nuevo primero", lista_ver_primero(lista) == NULL);

    //Inserto un entero al final y verifico que el primero sigue siendo NULL y el último es el entero
    int a = 5;
    lista_insertar_ultimo(lista, &a);
    print_test("Al insertar un entero al final, el primero sigue siendo NULL", lista_ver_primero(lista) == NULL);
    print_test("El último es el entero", lista_ver_ultimo(lista) == &a);

    //Borro el primero y el nuevo primero es el elemento int
    void* devuelto = lista_borrar_primero(lista);
    print_test("Al borrar el primero, se devuelve el elemento NULL", devuelto == NULL);
    print_test("El nuevo primero es el entero", lista_ver_primero(lista) == &a);

    //Borro el primero y vuelve a estar vacía
    lista_borrar_primero(lista);
    print_test("Al borrar el primero de vuelta, la lista vuelve a estar vacía", lista_esta_vacia(lista));
    print_test("ver_primero devuelve NULL", lista_ver_primero(lista) == NULL);
    print_test("ver_ultimo devuelve NULL", lista_ver_ultimo(lista) == NULL);

    //Destruyo la lista utilizada
    lista_destruir(lista, NULL);

    putchar('\n');
}

//Crea un arreglo para pruebas con elementos de 0 a n-1, alojados en memoria solicitada
//por un malloc
static int *crear_arreglo_prueba(size_t n){
    int *arreglo = malloc(n * sizeof(int));
    for(int k = 0; k < n; k++){
        arreglo[k] = k;
    }
    return arreglo;
}

//Inserta elementos (punteros a valores) de un arreglo en una lista y comprueba que esta no esté vacía
//Dependiendo de los punteros a función insertar al principio o al final.
static void insertar_n(lista_t *lista, int* arreglo, size_t n, funcion_insertar insertar, funcion_ver_lista lista_ver){
    int i = 0;
    for(; i < n; i++){
        bool insertado = insertar(lista, &arreglo[i]);
        if(!insertado) break;
        if(lista_ver(lista) != &arreglo[i]) break;
    }
    printf("Insertar %zu elementos, lista no vacía, %zu elementos", n, n);
    print_test("", i == n);
}

//Elimina elementos de 0 a n de una lista y chaquea elemento por elemento que el valor apuntado por el puntero
//borrado sea igual número de iteración contando desde 0. Comprueba también que el nuevo primero de la lista
//sea el siguiente al elemento recién eliminado.
static void borrar_ultimos_n(lista_t* lista, int n){
    int k = 0;
    for(; k <= n; k++){
        int* borrado = lista_borrar_primero(lista);
        if(*borrado != k) break;
        if(lista_esta_vacia(lista)) break;
        int* primero = lista_ver_primero(lista);
        if(*primero != k + 1) break;
    }
    print_test("Cada primero coincide con el elemento siguiente al borrado y el orden de los elementos borrados es correcto", k == n);
    printf("La lista de %d elementos ahora está vacía", n + 1);
    print_test("", lista_esta_vacia(lista));
}

//Elimina elementos de n a 0 de una lista y chaquea elemento por elemento que el valor apuntado por el puntero
//borrado sea igual número de iteración contando desde n a 0. Comprueba también que el nuevo primero de la lista
//sea el siguiente al elemento recién eliminado.
static void borrar_primeros_n(lista_t* lista, int n){
    int k = n;
    for(; k >= 0; k--){
        int* borrado = lista_borrar_primero(lista);
        if(*borrado != k) break;
        if(lista_esta_vacia(lista)) break;
        int* primero = lista_ver_primero(lista);
        if(*primero != k - 1) break;
    }
    print_test("Cada primero coincide con el elemento siguiente al borrado y el orden de los elementos borrados es correcto", k == 0);
    printf("La lista de %d elementos ahora está vacía", n + 1);
    print_test("", lista_esta_vacia(lista));
}

static void prueba_insertar_borrar_ultimos(){
    printf(PURPLE "PRUEBA DE INSERTAR Y BORRAR LOS ÚLTIMOS ELEMENTOS EN UNA LISTA" RESET_COLOR "\n");

    //Listas que uso en esta prueba
    lista_t *lista1 = lista_crear();
    lista_t *lista2 = lista_crear();
    lista_t *lista3 = lista_crear();

    //Pruebo insertar 10 elementos al último de la lista y compruebo que el orden sea el correcto
    size_t n1 = 10;
    int* arreglo1 = crear_arreglo_prueba(n1);
    insertar_n(lista1, arreglo1, n1, lista_insertar_ultimo, lista_ver_ultimo);

    //Elimino los 10 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int i = 9; 
    borrar_ultimos_n(lista1, i);

    //Pruebo insertar 10000 elementos y compruebo que el orden sea el correcto
    size_t n2 = 10000;
    int* arreglo2 = crear_arreglo_prueba(n2);
    insertar_n(lista2, arreglo2, n2, lista_insertar_ultimo, lista_ver_ultimo);

    //Elimino los 10000 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int j = 9999; 
    borrar_ultimos_n(lista2, j);

    //Pruebo insertar 100000 elementos al último de la lista y compruebo que el orden sea el correcto
    size_t n3 = 100000;
    int* arreglo3 = crear_arreglo_prueba(n3);
    insertar_n(lista3, arreglo3, n3, lista_insertar_ultimo, lista_ver_ultimo);

    //Elimino los 100000 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int k = 99999; 
    borrar_ultimos_n(lista3, k);

    //Destruyo las listas utilizadas
    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    lista_destruir(lista3, NULL);

    //Destruyo los arreglos utilizados
    free(arreglo1);
    free(arreglo2);
    free(arreglo3);

    putchar('\n');
}

static void prueba_insertar_borrar_primeros(){
    printf(PURPLE "PRUEBA DE INSERTAR Y BORRAR LOS PRIMEROS ELEMENTOS EN UNA LISTA" RESET_COLOR "\n");

    //Listas que uso en esta prueba
    lista_t *lista1 = lista_crear();
    lista_t *lista2 = lista_crear();
    lista_t *lista3 = lista_crear();

    //Pruebo insertar primeros 10 elementos en la lista y compruebo que el orden sea el correcto
    size_t n1 = 10;
    int* arreglo1 = crear_arreglo_prueba(n1);
    insertar_n(lista1, arreglo1, n1, lista_insertar_primero, lista_ver_primero);

    //Elimino los 10 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int i = 9; 
    borrar_primeros_n(lista1, i);

    //Pruebo insertar primeros 10000 elementos en la lista y compruebo que el orden sea el correcto
    size_t n2 = 10000;
    int* arreglo2 = crear_arreglo_prueba(n2);
    insertar_n(lista2, arreglo2, n2, lista_insertar_primero, lista_ver_primero);

    //Elimino los 10000 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int j = 9999; 
    borrar_primeros_n(lista2, j);

    //Pruebo insertar primeros 100000 elementos en la lista y compruebo que el orden sea el correcto
    size_t n3 = 100000;
    int* arreglo3 = crear_arreglo_prueba(n3);
    insertar_n(lista3, arreglo3, n3, lista_insertar_primero, lista_ver_primero);

    //Elimino los 100000 elementos y compruebo que cada primero coincida con el número de iteración y el nuevo primero
    //coincida con el elemento siguiente al recién borrado.
    int k = 99999; 
    borrar_primeros_n(lista3, k);

    //Destruyo las listas utilizadas
    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    lista_destruir(lista3, NULL);

    //Destruyo los arreglos utilizados
    free(arreglo1);
    free(arreglo2);
    free(arreglo3);

    putchar('\n');
}

static void pila_destruir_wrapper(void* dato){
    pila_t* pila = dato;
    pila_destruir(pila);
}

static void prueba_destruir_lista(void){

    //Creo las listas que utilizaré en la prueba
    lista_t *lista_vacia = lista_crear();
    lista_t *lista_enteros = lista_crear();
    lista_t *lista_chars = lista_crear();
    lista_t *lista_de_pilas = lista_crear();
    
    //Pila1 con números
    pila_t *pila1 = pila_crear();
    int numero1 = 1;
    int numero2 = 2;
    int numero3 = 3;
    pila_apilar(pila1, &numero1);
    pila_apilar(pila1, &numero2);
    pila_apilar(pila1, &numero3);
    //Pila2 vacía
    pila_t *pila2 = pila_crear();
    //Pila3 con un char
    pila_t *pila3 = pila_crear();
    char* cadena_random = "Barasch besto bald";
    pila_apilar(pila3, cadena_random);

    /* Inserto los elementos respectivos a cada lista para la prueba */
    //lista_enteros
    int numero4 = 4;
    int numero5 = 5;
    int numero6 = 6;
    lista_insertar_primero(lista_enteros, &numero4);
    lista_insertar_primero(lista_enteros, &numero5);
    lista_insertar_primero(lista_enteros, &numero6);

    //lista_chars
    char* cadena1 = "Roberto el dinosaurio";
    char* cadena2 = "BOCA YO TE AMO";
    char* cadena3 = "Here it comes the parcialito 2";
    char* cadena4 = "El dulce de leche, el gran colectivo, alpargatas, soda y alfajoreeeee";
    lista_insertar_primero(lista_chars, cadena1);
    lista_insertar_primero(lista_chars, cadena2);
    lista_insertar_primero(lista_chars, cadena3);
    lista_insertar_primero(lista_chars, cadena4);

    //lista_de_pilas
    lista_insertar_ultimo(lista_de_pilas, pila1);
    lista_insertar_ultimo(lista_de_pilas, pila2);
    lista_insertar_ultimo(lista_de_pilas, pila3);

    //Destruyo las listas
    lista_destruir(lista_vacia, NULL);  //No necesita ninguna función externa
    lista_destruir(lista_enteros, NULL); //Idem
    lista_destruir(lista_chars, NULL);    //Idem
    lista_destruir(lista_de_pilas, pila_destruir_wrapper);

    //Al compilar con valgrind la cantidad de mallocs es igual a la cantidad de frees, eso prueba que lista_destruir funciona ;)
}

static void prueba_invariante(void){
    printf(PURPLE "PRUEBAS DE CONSERVACIÓN DE INVARIANTE DE LA LISTA" RESET_COLOR "\n");
    //Creo la lista que usaré para la prueba
    lista_t *lista = lista_crear();

    //Compruebo que la lista está iniciamente vacía
    print_test("La lista recién creada está vacía",lista_esta_vacia(lista));

    //Inserto un elemento char* y compruebo que el primero sea coincidente con el puntero a char y que
    //la lista no está vacía
    char *cadena = "Hola, como estás?";
    lista_insertar_primero(lista, cadena);
    char *primero = lista_ver_primero(lista);
    print_test("Insertando primero un elemento char, el nuevo primero es correcto", primero == cadena);
    print_test("La lista no está vacía", !lista_esta_vacia(lista));
    
    //Inserto al final NULL y compruebo que el primero sigue siendo la cadena
    //Elimino primero y el nuevo primero es NULL
    lista_insertar_ultimo(lista, NULL);
    print_test("Al insertar al final NULL el primero sigue siendo el elemento char", lista_ver_primero(lista) == cadena);
    lista_borrar_primero(lista);
    print_test("Al eliminar el primer elemento, el nuevo primero es NULL", lista_ver_primero(lista) == NULL);
    void* eliminado = lista_borrar_primero(lista);
    print_test("Eliminando de nuevo, el elemento borrado es NULL", eliminado == NULL);
    print_test("La lista está vacía", lista_esta_vacia(lista));

    //Inserto un int al final y compruebo el último
    int a = 1;
    lista_insertar_ultimo(lista, &a);
    int* ultimo = lista_ver_ultimo(lista);
    print_test("Al insertar al final el puntero al int, el nuevo primero es correcto", ultimo == &a);

    //elimino y compruebo que vuelve a estar vacía
    lista_borrar_primero(lista); //Es indistinto si borro el primero o el último ya que tiene 1 elemento
    print_test("Al eliminar primero de nuevo, vuelve a estar vacía", lista_esta_vacia(lista));

    //Destruyo la lista utilizada
    lista_destruir(lista, NULL);

    putchar('\n');
}

static void prueba_casos_borde(void){
    printf(PURPLE "PRUEBAS DE CASOS BORDE DE LA LISTA" RESET_COLOR "\n");
    //Creo la lista que utilizaré en la prueba
    lista_t *lista = lista_crear();

    //Caso borde: al estar vacía la lista tanto ver_primero como borrar_primero devuelven NULL
    print_test("eliminar cuando la lista está vacía no es válido", lista_borrar_primero(lista) == NULL);
    print_test("La lista está vacía, ver_primero devuleve NULL", lista_ver_primero(lista) == NULL);

    //Caso borde: el enlistamiento de NULL es válido
    lista_insertar_primero(lista, NULL);
    print_test("Al insertar NULL, ver_primero devuleve NULL", lista_ver_primero(lista) == NULL);
    lista_insertar_primero(lista, NULL);
    lista_borrar_primero(lista);
    print_test("Al insertar un nuevo elemento NULL y eliminar el primero, ver_primero devuleve NULL nuevamente", lista_ver_primero(lista) == NULL);
    //elimino NULL para la siguiente prueba
    lista_borrar_primero(lista);

    //Caso borde: eliminar en una lista con un solo elemento, la lista estará vacía, ver_primero devuelve NULL
    char* cadena = "Soy una cadena de prueba";
    lista_insertar_ultimo(lista, cadena);
    print_test("Al insertar puntero a cadena, ver_primero devuleve el puntero a esa cadena", lista_ver_ultimo(lista) == cadena);
    lista_borrar_primero(lista);
    print_test("Al eliminar, ver_primero devuleve NULL porque la lista está vacía", lista_ver_primero(lista) == NULL);
    print_test("La lista está vacía nuevamente", lista_esta_vacia(lista));

    //Destruyo la lista utilizada
    lista_destruir(lista, NULL);

    putchar('\n');
}

bool visitar_suma_pares(void* dato, void* extra) {
    if ((*(int*) dato) % 2 == 0){
        *(int*) extra += *(int*) dato; 
    }
    return true;
}

bool visitar_primeras_cinco_cadenas_con_r(void* dato, void* extra) {
    if (*(int*) extra > 4) return false;
    char* cadena = dato;
    for(int i = 0; cadena[i] != '\0';  i++){
        if (tolower(cadena[i]) == 'r'){
            *(int*) extra+=1;
            return true;
        }
    }
    return true;
}

void pruebas_iterador_interno(void){
    printf(PURPLE "PRUEBAS DEL ITERADOR INTERNO DE LA LISTA" RESET_COLOR "\n");
    //Creo las listas que utilizaré en esta prueba
    lista_t* lista1 = lista_crear();
    lista_t* lista2 = lista_crear();
    lista_t* lista3 = lista_crear();

    //Itero una lista vacía.
    int extra1 = 0;
    lista_iterar(lista1, visitar_suma_pares, &extra1);
    print_test("Prueba con lista vacía, suma de elementos pares es igual a 0", extra1 == 0);

    //Inserto números pares e impares a la lista
    int extra2 = 0;
    size_t n1 = 7;
    int* arreglo_prueba = crear_arreglo_prueba(n1);
    insertar_n(lista1, arreglo_prueba, n1, lista_insertar_ultimo, lista_ver_ultimo);
    lista_iterar(lista1, visitar_suma_pares, &extra2);
    //La suma de elementos pares del 0 al n1-1 (6), es 12
    print_test("Prueba con varios elementos, suma de elementos pares de 0 a 6 es igual a 12", extra2 == 12);
    
    //Inserto 5 cadenas de las cuales 3 tienen la letra R en ellas 
    int extra3 = 0;
    char* cadena1 = "Roberto";
    char* cadena2 = "Pedro";
    char* cadena3 = "Josefina";
    char* cadena4 = "No puede hacer tanto CALOOOOOOOOOOOOOOOOOOOOOOOR";
    char* cadena5 = "Quememos el depto de Física";
    lista_insertar_primero(lista2,cadena1);
    lista_insertar_primero(lista2,cadena2);
    lista_insertar_primero(lista2,cadena3);
    lista_insertar_primero(lista2,cadena4);
    lista_insertar_primero(lista2,cadena5);

    lista_iterar(lista2, visitar_primeras_cinco_cadenas_con_r, &extra3);
    
    print_test("Contador al verificar 5 cadenas de las cuales 3 tienen R, es igual a 3", extra3 == 3);

    //Inserto 8 cadenas de las cuales 7 tienen la letra R en ellas 
    int extra4 = 0;
    char* cadena6 = "La derivada de mis ganas de vivir es 0, porque mi nihilismo es constante";
    char* cadena7 = "Fullmetal alchemist: Brotherhood";
    char* cadena8 = "Calamardo";
    char* cadena9 = "Koe no Katachi";
    char* cadena10 = "Rovira";
    char* cadena11 = "Aguirre";
    char* cadena12 = "Como sufro Análsis II";
    char* cadena13 = "Hora de no cursar álgebra 2";
    lista_insertar_primero(lista3,cadena6);
    lista_insertar_primero(lista3,cadena7);
    lista_insertar_primero(lista3,cadena8);
    lista_insertar_primero(lista3,cadena9);
    lista_insertar_primero(lista3,cadena10);
    lista_insertar_primero(lista3,cadena11);
    lista_insertar_primero(lista3,cadena12);
    lista_insertar_primero(lista3,cadena13);

    lista_iterar(lista3, visitar_primeras_cinco_cadenas_con_r, &extra4);
    
    print_test("Contador al verificar 8 cadenas de las cuales 7 tienen R, es igual a 5", extra4 == 5);

    //Destruyo la lista y el arreglo utilizado
    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    lista_destruir(lista3, NULL);
    free(arreglo_prueba);

    putchar('\n');
}

void pruebas_iterador_externo(){
    printf(PURPLE "PRUEBAS DEL ITERADOR EXTERNO DE LA LISTA" RESET_COLOR "\n");

    //Creo las listas e iteradores a utilizar en las pruebas
    lista_t* lista1 = lista_crear();
    size_t n1 = 5;
    int* arreglo_prueba1 = crear_arreglo_prueba(n1);
    insertar_n(lista1, arreglo_prueba1, n1, lista_insertar_ultimo, lista_ver_ultimo);
    lista_iter_t* iterador1 = lista_iter_crear(lista1);

    lista_t* lista2 = lista_crear();
    size_t n2 = 10;
    int* arreglo_prueba2 = crear_arreglo_prueba(n2);
    insertar_n(lista2, arreglo_prueba2, n2, lista_insertar_ultimo, lista_ver_ultimo);
    lista_iter_t* iterador2 = lista_iter_crear(lista2);

    lista_t* lista3 = lista_crear();
    size_t n3 = 5;
    int* arreglo_prueba3 = crear_arreglo_prueba(n3);
    insertar_n(lista3, arreglo_prueba3, n3, lista_insertar_ultimo, lista_ver_ultimo);
    lista_iter_t* iterador3 = lista_iter_crear(lista3);
    lista_iter_t* iterador4 = lista_iter_crear(lista3);
    lista_iter_t* iterador5 = lista_iter_crear(lista3);

    //Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista.
    int* borrado = lista_iter_borrar(iterador1);
    print_test("Borrando el primer elemento (0), devuelve el elemento correcto", *borrado == 0);
    print_test("Habiendo borrado el primer elemento, el primero pasa a ser el siguiente elemento (1)", *(int*) lista_ver_primero(lista1) == 1);

    //Inserto un elemento en la posición donde se crea el iterador y chequeo que
    //efectivamente es el primer elemento.
    int cero = 0;
    lista_iter_insertar(iterador1, &cero);
    print_test("Insertar un elemento en la posición inicial del iterador, el primer elemento es correcto", lista_ver_primero(lista1) == &cero);

    //Avanzamos hasta el final de la lista con el iterador.
    while(!lista_iter_al_final(iterador1)) lista_iter_avanzar(iterador1);
    //Insertamos un elemento cuando el iterador está al final de la lista
    char* cadena = "Roberto el dinosaurio";
    lista_iter_insertar(iterador1, &cadena);
    print_test("Insertar un elemento al final de la lista, el último elemento es correcto", lista_ver_ultimo(lista1) == &cadena);

    //Me posiciono en el medio para insertar un elemento y corroboro que el actual del iterador sea el elemento que acabo de insertar
    for(int i = 0; i < 5;i++) lista_iter_avanzar(iterador2);
    int num = 420;
    lista_iter_insertar(iterador2, &num);
    print_test("Insertar un elemento a la mitad de la lista devuelve como actual el elemento insertado", lista_iter_ver_actual(iterador2) == &num);
    
    //Remover el último elemento con el iterador cambia el último de la lista.
    for(int j = 0; j < 5; j++) lista_iter_avanzar(iterador2);
    int* borrado2 = lista_iter_borrar(iterador2);
    print_test("Borrando el último elemento (9), devuelve el elemento correcto", *borrado2 == 9);
    print_test("Remover el último elemento con el iterador cambia el último elemento de la lista", *(int*) lista_ver_ultimo(lista2) == 8);

    //Verificar que al remover un elemento del medio, este no está. Utilizando primero un iterador para borrar el del medio, y otro para verificar
    //que no esté
    for(int j = 0; j < 2; j++) lista_iter_avanzar(iterador3);
    int* borrado3 = lista_iter_borrar(iterador3);
    print_test("Borrando un elemento del medio entre 0 y 5 (2), devuelve 2", *borrado2 == 9);
    bool existe = false;
    while(!lista_iter_al_final(iterador4) && !existe){
        if (lista_iter_ver_actual(iterador4) == borrado3){
            existe = true;
        }
        lista_iter_avanzar(iterador4);
    }
    print_test("El elemento borrado del medio (2), ya no está en la lista", !existe);

    //Remuevo todos los elementos de la lista con el iterador y chequeo que quede vacía
    while(!lista_iter_al_final(iterador5)){
        lista_iter_borrar(iterador5);
    }
    print_test("Remuevo todos los elementos con el iterador y chequeo que la Lista quede vacía", lista_esta_vacia(lista3));

    //Destruyo las estructuras utilizadas
    free(arreglo_prueba1);
    free(arreglo_prueba2);
    free(arreglo_prueba3);
    lista_iter_destruir(iterador1);
    lista_iter_destruir(iterador2);
    lista_iter_destruir(iterador3);
    lista_iter_destruir(iterador4);
    lista_iter_destruir(iterador5);
    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    lista_destruir(lista3, NULL);
    
    putchar('\n');
}

void pruebas_lista_estudiante(void){
    //Pruebas de primitivas de la lista
    pruebas_lista_vacia();
    prueba_ver_primero_y_ultimo();
    prueba_insertar_borrar_ultimos();
    prueba_insertar_borrar_primeros();
    prueba_destruir_lista();
    prueba_invariante();
    prueba_casos_borde();
    
    //Pruebas de primitivas de los iteradores de la lista
    pruebas_iterador_interno();
    pruebas_iterador_externo();
}

#ifndef CORRECTOR

int main(void){
    pruebas_lista_estudiante();
    return failure_count() > 0;
}

#endif
