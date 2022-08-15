#include "pila.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL 6
#define CANTIDAD_INICIAL 0

/* Definición del struct pila proporcionado por la cátedra.
 */
typedef struct pila {
    void** datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
} pila_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));
    if (pila == NULL){
        return NULL;
    }
    pila->cantidad = CANTIDAD_INICIAL;
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->datos = malloc((pila->capacidad) * sizeof(void*));
    return pila;
}

bool pila_redimensionar(pila_t* pila, size_t capacidad_final){
    void** datos_nuevos = realloc(pila->datos, capacidad_final * sizeof(void*));
    if (!datos_nuevos) return false;
    pila->capacidad = capacidad_final;
    pila->datos = datos_nuevos;
    return true;
}

void pila_destruir(pila_t *pila){
    free((pila->datos));
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
    if ((pila->cantidad) == (pila->capacidad)){ //Redimensiono para mayor si es necesario.
        if (!pila_redimensionar(pila, 2*(pila->capacidad))){
            return false; //Si falla devuelvo error.
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;
    return pila->datos[(pila->cantidad)-1];
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;
    void* valor = (pila->datos)[(pila->cantidad)-1];
    pila->cantidad--;
    if((pila->cantidad) > 0 && ((pila->cantidad)*4 <= pila->capacidad)){ //Si veo después de desapilar que hay mucha capacidad, la bajo.
        pila_redimensionar(pila, (pila->capacidad)/ 2); //No hago nada si falla ya que no tengo que informar de la implementación interna de la pila.
    }
    return valor;
}
