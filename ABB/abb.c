#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
} nodo_t;

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t funcion_destruir;
};

struct abb_iter{
    pila_t* pila;
    const abb_t* abb;
};

static nodo_t* nodo_crear(const char* clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    char* copia = malloc(sizeof(char) * (strlen(clave) + 1));
    if(!copia){
        free(nodo);
        return NULL;
    }
    strcpy(copia,clave);
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = copia;
    nodo->dato = dato; 
    return nodo;
}

static void nodo_destruir(nodo_t* nodo){
    free(nodo->clave);
    free(nodo);
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;
    arbol->comparar = cmp;
    arbol->funcion_destruir = destruir_dato;
    arbol->cantidad = 0;
    arbol->raiz = NULL;
    return arbol;
}

static nodo_t** _obtener_nodo(nodo_t** nodo, const char* clave, abb_comparar_clave_t comparar){
    if(!*nodo) return nodo;
    int comparacion = comparar((*nodo)->clave, clave);
    if(comparacion == 0){
        return nodo;
    }
    else if (comparacion > 0){
        return _obtener_nodo(&(*nodo)->izq, clave, comparar);
    } else {
        return _obtener_nodo(&(*nodo)->der, clave, comparar);
    }
}

static nodo_t** obtener_nodo(abb_t* arbol, const char* clave){  
    return _obtener_nodo(&arbol->raiz, clave, arbol->comparar);
}   

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t** nodo = obtener_nodo(arbol, clave);
    if (!*nodo){
        nodo_t* nodo_nuevo = nodo_crear(clave, dato);
        if(!nodo_nuevo) return false;
        *nodo = nodo_nuevo;
        arbol->cantidad++;
    } else{
        void* dato_anterior = (*nodo)->dato;
        if (arbol->funcion_destruir) (arbol->funcion_destruir)(dato_anterior);
        (*nodo)->dato = dato;
    }
    return true;
}

void* abb_obtener(const abb_t *arbol, const char *clave){
    abb_t* arbol_a_buscar = (abb_t*)arbol;
    nodo_t** nodo = obtener_nodo(arbol_a_buscar, clave);
    if(!*nodo) return NULL;
    return (*nodo)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_t* arbol_a_buscar = (abb_t*)arbol;
    nodo_t** nodo = obtener_nodo(arbol_a_buscar, clave);
    return (*nodo) ? true : false;
}

static nodo_t* _obtener_reemplazo(nodo_t* nodo){
    if(!nodo->izq) return nodo;
    nodo = nodo->izq;
    return _obtener_reemplazo(nodo);
}

static nodo_t* obtener_reemplazo(const abb_t* arbol, nodo_t* nodo_actual){
    return _obtener_reemplazo(nodo_actual->der);
}

void* abb_borrar(abb_t *arbol, const char *clave){
    if(!abb_pertenece(arbol, clave)) return NULL;
    nodo_t** nodo = obtener_nodo(arbol, clave);
    void* dato_viejo = (*nodo)->dato;
    if(!(*nodo)->izq || !(*nodo)->der){
        nodo_t* hijo = (*nodo)->der ? (*nodo)->der : (*nodo)->izq;
        nodo_destruir(*nodo);
        *nodo = hijo;
        arbol->cantidad--;
    } else{
        nodo_t* reemplazante = obtener_reemplazo(arbol, *nodo);
        char* clave_reemplazante = malloc(sizeof(char) * (strlen(reemplazante->clave) + 1));
        if(!clave_reemplazante) return NULL;
        strcpy(clave_reemplazante,reemplazante->clave);
        void* dato = abb_borrar(arbol, clave_reemplazante);
        free((*nodo)->clave);
        (*nodo)->clave = clave_reemplazante;
        (*nodo)->dato = dato;
    }
    return dato_viejo;
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(abb_t* abb, nodo_t* nodo_actual){
    if(!nodo_actual) return;
    _abb_destruir(abb, nodo_actual->izq);
    _abb_destruir(abb, nodo_actual->der);
    if(abb->funcion_destruir) (abb->funcion_destruir)(nodo_actual->dato);
    nodo_destruir(nodo_actual);
}

void abb_destruir(abb_t *arbol){
    if(arbol->raiz){
        _abb_destruir(arbol, arbol->raiz);
    }
    free(arbol);
}

//PRIMITIVAS ITERADOR INTERNO ABB

static bool _abb_in_order(nodo_t* nodo, bool visitar(const char*, void*, void*), void* extra){
    if(!nodo) return true;
    bool continuar = _abb_in_order(nodo->izq, visitar, extra);
    if(!continuar) return false;
    continuar &= visitar(nodo->clave, nodo->dato, extra);
    if(!continuar) return false;
    continuar &= _abb_in_order(nodo->der, visitar, extra);
    return continuar;
}

void abb_in_order(abb_t *arbol, bool visitar(const char*, void*, void*), void* extra){
    _abb_in_order(arbol->raiz, visitar, extra);
} 

//PRIMITIVAS ITERADOR EXTERNO ABB

static void apilar_izquierdos(pila_t* pila, nodo_t* nodo){
    if(!nodo) return;
    pila_apilar(pila, nodo);
    apilar_izquierdos(pila, nodo->izq);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iterador = malloc(sizeof(abb_iter_t));
    if(!iterador) return NULL;
    pila_t* pila = pila_crear();
    apilar_izquierdos(pila, arbol->raiz);
    iterador->pila = pila;
    iterador->abb = arbol;
    return iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;
    nodo_t* nodo_actual = pila_desapilar(iter->pila);
    apilar_izquierdos(iter->pila, nodo_actual->der);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return NULL;
    nodo_t* nodo_actual = pila_ver_tope(iter->pila);
    return nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}