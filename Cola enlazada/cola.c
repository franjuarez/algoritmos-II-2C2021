#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
    void* dato;
    struct nodo* prox;
} nodo_t;

typedef struct cola{
    nodo_t* primero;
    nodo_t* ultimo;
} cola_t;

nodo_t* crear_nodo(void* dato){ //No paso referencia a prox ya que este siempre va a estar al final, es decir, va a ser NULL.
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}

void destruir_nodo(nodo_t* nodo){
    free(nodo);
}

cola_t* cola_crear(void){
    cola_t* cola_nueva = malloc(sizeof(cola_t));
    if(!cola_nueva) return NULL;
    cola_nueva->primero = NULL;
    cola_nueva->ultimo = NULL;
    return cola_nueva;
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){
    nodo_t* nodo_nuevo = crear_nodo(valor);
    if(!nodo_nuevo) return false;
    if (cola_esta_vacia(cola)){
        cola->primero = nodo_nuevo;
        cola->ultimo = nodo_nuevo;
        return true;
    }
    nodo_t* anterior = cola->ultimo;
    anterior->prox = nodo_nuevo;
    cola->ultimo = nodo_nuevo;
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    void* dato = (cola->primero)->dato;
    return dato;
}

void *cola_desencolar(cola_t *cola){ //destruir el nodo y ojo cuando el primero == ultimo.
    if (cola_esta_vacia(cola)) return NULL;
    void* valor = (cola->primero)->dato;
    if (!(cola->primero->prox)){
        destruir_nodo((cola->primero));
        cola->primero = NULL; //No hace falta hacer cola->ultimo = NULL ya que si entro a este if ya sé que es NULL.
        return valor;
    }
    nodo_t* nuevo_primero = (cola->primero)->prox;
    destruir_nodo((cola->primero));
    cola->primero = nuevo_primero;
    return valor;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    while(!cola_esta_vacia(cola)){
        if(destruir_dato){
           destruir_dato(cola_ver_primero(cola));
           }
        cola_desencolar(cola);
    }
    free(cola);
    }