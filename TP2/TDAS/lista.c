#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Definición de nodo
typedef struct nodo{
    void* dato;
    struct nodo* prox;
} nodo_t;

// Definición de lista
struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter{
    nodo_t* anterior;
    nodo_t* actual;
    lista_t* lista; 
};

static nodo_t* crear_nodo(void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}

static void destruir_nodo(nodo_t* nodo){
    free(nodo);
}

// PRIMITIVAS DE MANIPULACIÓN DE LA LISTA

lista_t *lista_crear(void){
    lista_t* lista_nueva = malloc(sizeof(lista_t));
    if(!lista_nueva) return NULL;
    lista_nueva->primero = NULL;
    lista_nueva->ultimo = NULL;
    lista_nueva->largo = 0;
    return lista_nueva;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    while(!lista_esta_vacia(lista)){
        if(destruir_dato) destruir_dato(lista_ver_primero(lista));
        lista_borrar_primero(lista);
    }
    free(lista);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato);
    if (!nodo_nuevo) return false;
    if(lista_esta_vacia(lista)) lista->ultimo = nodo_nuevo;
    nodo_nuevo->prox = lista->primero; 
    lista->primero = nodo_nuevo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato);
    if (!nodo_nuevo) return false;
    if(lista_esta_vacia(lista)){
        lista->primero = nodo_nuevo;
        lista->ultimo = nodo_nuevo;
    }
    else {
        lista->ultimo->prox = nodo_nuevo;
        lista->ultimo = nodo_nuevo;
    }
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    nodo_t* nodo_borrado = lista->primero; 
    if(!nodo_borrado) return NULL;
    if (lista->primero == lista->ultimo) lista->ultimo = NULL;
    void* dato = nodo_borrado->dato;
    lista->primero = nodo_borrado->prox;
    destruir_nodo(nodo_borrado);
    lista->largo--;
    return dato;
}

// PRIMITIVAS DE VERIFICACIÓN DE LA LISTA

bool lista_esta_vacia(const lista_t* lista){
    return lista->largo == 0;
}

size_t lista_largo(const lista_t* lista){
    return lista->largo;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    if (lista_esta_vacia(lista)) return NULL;
    return lista->ultimo->dato;
}

// PRIMITIVAS ITERADOR INTERNO 

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    bool iteracion = true;
    nodo_t* actual = lista->primero;
    while(iteracion && actual){
        iteracion &= visitar(actual->dato, extra);
        actual = actual->prox;
    }
}


// PRIMITIVAS ITERADOR EXTERNO 

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
    if(!iterador) return NULL;
    iterador->anterior = NULL;
    iterador->actual = lista->primero;
    iterador->lista = lista;
    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void* lista_iter_ver_actual(const lista_iter_t *iter){
    if(!iter->actual) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato);
    if(!nodo_nuevo) return false;
    nodo_nuevo->prox = iter->actual;
    if(!iter->actual) iter->lista->ultimo = nodo_nuevo;
    iter->actual = nodo_nuevo;
    if(!iter->anterior) iter->lista->primero = nodo_nuevo;
    else iter->anterior->prox = nodo_nuevo;
    iter->lista->largo++;
    return true;
}

void* lista_iter_borrar(lista_iter_t *iter){
    if(lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)) return NULL;
    void* dato = iter->actual->dato;
    nodo_t* actual_aux = iter->actual;
    if(!actual_aux->prox) iter->lista->ultimo = iter->anterior;
    if(!iter->anterior) iter->lista->primero = actual_aux->prox;
    else iter->anterior->prox = actual_aux->prox;
    iter->actual = actual_aux->prox;
    destruir_nodo(actual_aux);
    iter->lista->largo--;
    return dato;
}

