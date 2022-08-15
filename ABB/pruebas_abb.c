
#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>  // For ssize_t in Linux.
#define PURPLE "\033[1;35m" //Para escribir de este color 
#define RESET_COLOR "\x1b[0m" //Para reiniciar a blanco

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio()
{
    printf(PURPLE "PRUEBAS DE CREAR Y VERIFICAR ABB VACÍO" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba ABB crear vacio", abb);
    print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba ABB obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba ABB pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba ABB borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);

    putchar('\n');
}


static void prueba_abb_insertar()
{
    printf(PURPLE "PRUEBAS DE INSERTAR EN ABB" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    // Inserta 1 valor y luego lo borra
    print_test("Prueba ABB insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba ABB obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba ABB obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba ABB pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba ABB borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    //Inserta otros 2 valores y no los borra (se destruyen con el abb)
    print_test("Prueba ABB insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba ABB obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba ABB obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba ABB pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba ABB insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba ABB obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba ABB obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba ABB pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);

    putchar('\n');
}

static void prueba_abb_reemplazar()
{
    printf(PURPLE "PRUEBAS DE REEMPLAZAR EN EL ABB" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    // Inserta 2 valores y luego los reemplaza
    print_test("Prueba ABB insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba ABB insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba ABB insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba ABB insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
    
    putchar('\n');
}

static void prueba_abb_reemplazar_con_destruir()
{
    printf(PURPLE "PRUEBAS DE ABB REEMPLAZAR CON DESTRUIR" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    // Pide memoria para 4 valores
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    // Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza)
    print_test("Prueba ABB insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba ABB obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba ABB insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba ABB obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba ABB insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba ABB obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba ABB insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba ABB obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    // Se destruye el abb (se debe liberar lo que quedó dentro)
    abb_destruir(abb);

    putchar('\n');
}

static void prueba_abb_borrar()
{
    printf(PURPLE "PRUEBAS DE ABB BORRAR" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    // Inserta 3 valores y luego los borra
    print_test("Prueba ABB insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba ABB insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba ABB insertar clave3", abb_guardar(abb, clave3, valor3));

    // Al borrar cada elemento comprueba que ya no está pero los otros sí.
    print_test("Prueba ABB pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba ABB borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba ABB borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba ABB pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba ABB obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba ABB la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba ABB pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba ABB borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba ABB borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba ABB pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba ABB obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba ABB pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba ABB borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba ABB borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba ABB pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba ABB obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);

    putchar('\n');    
}

static void prueba_abb_clave_vacia()
{
    printf(PURPLE "PRUEBAS DE CLAVE DE ABB VACÍA" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba ABB insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba ABB obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba ABB pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba ABB borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
    
    putchar('\n');
}

static void prueba_abb_valor_null()
{
    printf(PURPLE "PRUEBAS DE ABB CON EL VALOR NULL" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    // Inserta 1 valor y luego lo borra 
    print_test("Prueba ABB insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba ABB la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba ABB obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba ABB pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba ABB borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);

    putchar('\n');
}

bool visitar_suma_pares(const char* clave, void* dato, void* extra) {
    if ((*(int*) dato) % 2 == 0){
        *(int*) extra += *(int*) dato; 
    }
    return true;
}

bool visitar_primeras_cinco_claves_con_r(const char* clave, void* dato, void* extra) {
    if (*(int*) extra > 4) return false;
    for(int i = 0; clave[i] != '\0';  i++){
        if (tolower(clave[i]) == 'r'){
            *(int*) extra+=1;
            return true;
        }
    }
    return true;
}

bool visitar_clave_mayor_que_dato(const char* clave, void* dato, void* extra) {
    int comparacion = strcmp(clave,(char*) dato);
    if (comparacion < 0){
        *(int*) extra+=1;
    }
    return true;
}

bool crear_arreglo(char* claves[], size_t** valores, abb_t* abb, size_t largo){ 
    bool ok = true;
    size_t j;
    for (size_t i = 0; i < largo; i++) {
        j = (size_t) rand(); //Rand da un número de más de 20 cifras (largo_clave)
        valores[i] = malloc(sizeof(size_t));
        sprintf(claves[i], "%ld", j);
        if(abb_pertenece(abb, claves[i])){
            free(valores[i]);
            i--;
            continue;
        }    
        *valores[i] = j;
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }
    return ok;
}

static void prueba_abb_volumen(size_t largo, bool debug){
    printf(PURPLE "PRUEBAS DE VOLUMEN DEL ABB" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 20;
    char** claves = malloc(largo * sizeof(char*));
    for(size_t i = 0; i < largo;i++){
        claves[i] = malloc((largo_clave+1) * sizeof(char));
    }

    size_t* valores[largo];

    //Inserta 'largo' parejas en el abb
    bool ok = crear_arreglo(claves, valores, abb, largo);

    if (debug) print_test("Prueba ABB almacenar muchos elementos", ok);
    if (debug) print_test("Prueba ABB la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    //Verifica que devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba ABB pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba ABB la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    //Verifica que borre y devuelva los valores correctos
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba ABB borrar muchos elementos", ok);
    if (debug) print_test("Prueba ABB la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    //Destruye el abb y crea uno nuevo que sí libera 
    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    //Inserta 'largo' parejas en el abb
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }
    
    //Libero las claves
    for(int i = 0; i < largo; i++) free(claves[i]);
    free(claves);   

    //Destruye el abb - debería liberar los size_t
    abb_destruir(abb);

    putchar('\n');
}

void pruebas_abb_iterador_interno(){
    printf(PURPLE "PRUEBAS DEL ITERADOR INTERNO DEL ABB" RESET_COLOR "\n");

    abb_t* abb1 = abb_crear(strcmp, NULL);
    abb_t* abb2 = abb_crear(strcmp, NULL);
    abb_t* abb3 = abb_crear(strcmp, NULL);
    abb_t* abb4 = abb_crear(strcmp, NULL);

    //Itero un abb vacío.
    int extra1 = 0;
    abb_in_order(abb1, visitar_suma_pares, &extra1);
    print_test("Prueba con ABB vacío, suma de elementos pares es igual a 0", extra1 == 0);

    //Inserto números pares e impares al ABB
    int extra2 = 0;
    char* clave1 = "a";
    char* clave2 = "b";
    char* clave3 = "c";
    char* clave4 = "d";
    char* clave5 = "e";
    int valor1 = 1;
    int valor2 = 2;
    int valor3 = 3;
    int valor4 = 4;
    int valor5 = 5;
    abb_guardar(abb2,clave1, &valor1);
    abb_guardar(abb2,clave2, &valor2);
    abb_guardar(abb2,clave3, &valor3);
    abb_guardar(abb2,clave4, &valor4);
    abb_guardar(abb2,clave5,&valor5);
    abb_in_order(abb2, visitar_suma_pares, &extra2);
    //La suma de elementos pares del 1 al 5, es 6
    print_test("Prueba con varios elementos, suma de elementos pares de 1 al 5 es igual a 6", extra2 == 6);
    
    //Inserto 5 cadenas de las cuales 3 tienen la letra R en ellas 
    int extra3 = 0;
    char* clave6 = "Roberto";
    char* clave7 = "Pedro";
    char* clave8 = "Lacalle Pou";
    char* clave9 = "No puede hacer tanto CALOOOOOOOOOOOOOOOOOOOOOOOR";
    char* clave10 = "Quememos el depto de Física";
    int valor6 = 1;
    int valor7 = 1;
    int valor8 = 1; //No importan los valores ya que chequeo las claves
    int valor9 = 1;
    int valor10 = 1;
    abb_guardar(abb2,clave6, &valor6);
    abb_guardar(abb2,clave7, &valor7);
    abb_guardar(abb2,clave8, &valor8);
    abb_guardar(abb2,clave9, &valor9);
    abb_guardar(abb2,clave10,&valor10);

    abb_in_order(abb2, visitar_primeras_cinco_claves_con_r, &extra3);
    
    print_test("Contador al verificar 5 cadenas de las cuales 3 tienen R, es igual a 3", extra3 == 3);

    //Inserto 8 cadenas de las cuales 7 tienen la letra R en ellas 
    int extra4 = 0;
    char* clave11 = "La derivada de mis ganas de vivir es 0, porque mi nihilismo es constante";
    char* clave12 = "Fullmetal alchemist: Brotherhood";
    char* clave13 = "Calamardo";
    char* clave14 = "Koe no Katachi";
    char* clave15 = "Rovira";
    char* clave16 = "Aguirre";
    char* clave17 = "Como sufro Análsis II";
    char* clave18 = "Hora de no cursar álgebra 2";
    int valor11 = 1;
    int valor12 = 1;
    int valor13 = 1;
    int valor14 = 1; //No importan los valores ya que chequeo las claves
    int valor15 = 1;
    int valor16 = 1;
    int valor17 = 1;
    int valor18 = 1;
    abb_guardar(abb3,clave11, &valor11);
    abb_guardar(abb3,clave12, &valor12);
    abb_guardar(abb3,clave13, &valor13);
    abb_guardar(abb3,clave14, &valor14);
    abb_guardar(abb3,clave15, &valor15);
    abb_guardar(abb3,clave16, &valor16);
    abb_guardar(abb3,clave17, &valor17);
    abb_guardar(abb3,clave18, &valor18);

    abb_in_order(abb3, visitar_primeras_cinco_claves_con_r, &extra4);
    
    print_test("Contador al verificar 8 cadenas de las cuales 7 tienen R, es igual a 5", extra4 == 5);

    int extra5 = 0;
    char* clave19 = "aa";
    char* clave20 = "Berto";
    char* clave21 = "Carlos";
    char* clave22 = "Fort";
    char* clave23 = "EpsilonCero";
    char* clave24 = "Toro";
    char* valor19 = "pAbLo";
    char* valor20 = "Chino";
    char* valor21 = "Azucar";
    char* valor22 = "OSO";
    char* valor23 = "Mousepad";
    char* valor24 = "Zapallo";
    abb_guardar(abb4,clave19, valor19);
    abb_guardar(abb4,clave20, valor20);
    abb_guardar(abb4,clave21, valor21);
    abb_guardar(abb4,clave22, valor22);
    abb_guardar(abb4,clave23, valor23);
    abb_guardar(abb4,clave24, valor24);

    abb_in_order(abb4, visitar_clave_mayor_que_dato, &extra5);
    
    print_test("Contador al verificar 5 claves con mayores a los datos", extra5 == 5);

    //Destruyo los abbs
    abb_destruir(abb1);
    abb_destruir(abb2);
    abb_destruir(abb3);
    abb_destruir(abb4);

    putchar('\n');
}

static void prueba_iterar_externo_abb_vacio(){
    printf(PURPLE "PRUEBAS DE ITERADOR EXTERNO EN ABB VACÍO" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
    putchar('\n');
}

static ssize_t buscar(const char* clave, char* claves[], size_t largo){
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_abb_iterar(){
    printf(PURPLE "PRUEBAS DE ITERADOR EXTERNO EN ABB CON ELEMENTOS" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    //Inserta 3 valores
    print_test("Prueba ABB insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba ABB insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba ABB insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Primer valor
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba ABB iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Segundo valor
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba ABB iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Tercer valor
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba ABB iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba ABB iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

    //Vuelve a tratar de avanzar, por las dudas
    print_test("Prueba ABB iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba ABB iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
    putchar('\n');
}

static void prueba_abb_iterar_volumen(size_t largo){
    printf(PURPLE "PRUEBAS DE ITERADOR EXTERNO DE VOLUMEN" RESET_COLOR "\n");
    abb_t* abb = abb_crear(strcmp, free);

    const size_t largo_clave = 20;
    char** claves = malloc(largo * sizeof(char*));
    for(size_t i = 0; i < largo;i++){
        claves[i] = malloc((largo_clave+1) * sizeof(char));
    }

    size_t* valores[largo];

    //Inserta 'largo' parejas en el abb
    bool ok = crear_arreglo(claves, valores, abb, largo);

    //Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba ABB iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba ABB iteración en volumen", ok);
    print_test("Prueba ABB iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba ABB iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( *valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba ABB iteración en volumen, se cambiaron todo los elementos", ok);

    for(i = 0; i < largo; i++) free(claves[i]);
    free(claves);

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

int pruebas_abb_estudiante(){
    //Ejecuta todas las pruebas unitarias.
    prueba_crear_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    pruebas_abb_iterador_interno();
    prueba_iterar_externo_abb_vacio();
    prueba_abb_iterar();
    prueba_abb_volumen(5000, true);
    prueba_abb_iterar_volumen(5000);
    return 0;
}
#ifndef CORRECTOR

int main(){
    pruebas_abb_estudiante();
    return failure_count() > 0;
}

#endif
