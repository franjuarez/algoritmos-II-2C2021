#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Declaración  de estructuras y funciones

//Estructura del ABB
typedef struct abb abb_t;
//Estructura del iterador externo
typedef struct abb_iter abb_iter_t;
//Funciones de comparación de claves y destrucción de dato
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//PRIMITIVAS DEL ÁRBOL BINARIO DE BÚSQUEDA

//Crea un ABB vacío y funciones de comparación y destrucción
//Post: Devuelve el árbol binario de búsqueda vacío.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda en el ABB dentro de una clave, el valor específicado por parámetro
//Pre: El ABB fue creado.
//Post: Se almacenó en el ABB el nuevo dato en donde corresponde la clave.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra la clave especificada por parámtero del ABB y devuelve su valor,
// devuelve NULL en caso de no estar la clave.
// Pre: El ABB fue creado.
// Post: Si la clave existe, la borra y devuelve el dato
// que estaba asociado a esa clave.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene y devuelve el dato asociado a una clave en el ABB.
// En caso de no estar en el ABB devuelve NULL.
// Pre: El ABB fue creado.
//Post: Si la clave existe, devuelve el dato asociado a esa clave
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true si la clave especificada por parámetro pertenece al ABB.
// NULL en caso contrario
// Pre: El ABB fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve el largo del ABB
// Pre: El ABB fue creado.
size_t abb_cantidad(const abb_t *arbol);

// Destruye el ABB y todos sus elementos.
// Pre: El ABB fue creado
// Post: El ABB y todos sus elementos fueron destruidos.
void abb_destruir(abb_t *arbol);

// ITERADOR INTERNO DEL ABB

// Permite iterar internamente el abb de forma IN-ORDER
// Pre: El abb fue creado exitosamente.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// // PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB

// Crea un iterador nuevo sobre un abb
// Pre: El árbol fue creado 
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza el iterador de posción de forma in order
// devolverá true mientras no se esté al final del 
// recorrido in order
// Pre: El iterador fue creado
// Post: La posición del iterador es la siguiente en
// el recorrido in order del árbol
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Muestra la clave del elemento actual sobre el cual se está
// iterando en el ABB, si está al final del iterador, devuelve NULL
// Pre: El iterador fue creado
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador del abb está al final
// Pre: El iterador fue creado exitosamente
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
//Post: se destruyó el iterador.
void abb_iter_in_destruir(abb_iter_t* iter);
