#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
//La lista estará planteada como una lista de punteros genéricos (void *)

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada exitosamente.
bool lista_esta_vacia(const lista_t *lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada exitosamente.
size_t lista_largo(const lista_t* lista);

// Devuelve el primer elemento de la lista o NULL en caso
// de que la lista se encuentre vacía.
// Pre: La lista fue creada exitosamente.
// Post: Se devolvió el primer elemento de la lista, en caso de no estar vacía.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el último elemento de la lista o NULL en caso
// de que la lista se encuentre vacía.
// Pre: La lista fue creada exitosamente.
// Post: Se devolvió el último elemento de la lista, en caso de no estar vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Inserta un elemento en el primer lugar de la lista, devuelve
// true en caso de ser exitoso o false en caso de haber fallado
// Pre: La lista fue creada exitosamente.
// Post: Se insertó el elemento en el primer lugar y pasó a ser
// el nuevo primero de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un elemento en el último lugar de la lista, devuelve
// true en caso de ser exitoso o false en caso de haber fallado
// Pre: La lista fue creada exitosamente.
// Post: Se insertó el elemento en el último lugar y pasó a ser
// el nuevo último de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista y devuelve el dato alojado
// en el mismo. En caso de estar vacía devuelve NULL.
// Pre: La lista fue creada exitosamente.
// Post: Elimina el primer elemento de la lista y actualiza el valor
// del primero de la lista al siguiente del eliminado.
void *lista_borrar_primero(lista_t *lista);

// Destruye los elementos de la lista si recibe NULL por parámetro.
// En caso de pasarse un puntero a función por parámetro, utilizará la función provista
// por el usuario para destruir el dato de cada elemento.
// En ámbos casos la lista es destruida.
// Pre: la lista fue creada; destuir_dato es una función capaz de destruir el tipo
// de dato correspondiente a cada elemento o NULL en caso de no utilizarse.
// Post: los elementos de la lista fueron destruidos, la lista fue destruida.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

//ITERADOR INTERNO DE LA LISTA
//Pre: La lista fue creada exitosamente.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//ITERADOR EXTERNO DE LISTA

//Crea un iterador externo de la lista.
//Pre: La lista fue creada exitosamente y no se
//modifica por fuera del iterador.  
//Post: Devuelve un iterador de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza una posición en la lista al elemento próximo del iterador.
//Devuelve true en caso de avanzar exitosamente o false en caso
//de estar al final de la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

//Devuelve el elemento donde está parado el iterador.
//Pre: el iterador fue creado.
//Post: Devuelvió el elemento donde estaba parado el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve true si el elemento actual del iterador es NULL
//caso contrario false.
//Pre: La lista existe y se creó exitosamente.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
//Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta un elemento en la posición donde está parado el iterador.
//Devuelve true en caso de ser exitoso o false en caso de haber fallado.
//Pre: el iterador ha sido creado exitosamente.
//Post: Se insertó un elemento en la posición del iterador y pasó
//a ser el elemento actual.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Borra un elemento en la posición donde está parado el iterador.
//Devuelve el elemento borrado o NULL si está al final.
//Pre: el iterador ha sido creado exitosamente.
//Post: eliminó el elemento sobre la posición actual del iterador.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);

#endif