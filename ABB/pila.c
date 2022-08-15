#include "pila.h"
#include <stdlib.h>
size_t VALOR_INICIAL = 20;
size_t MULTIPLICADOR = 4;

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

//Recibe la pila a redimensionar y una capacidad nueva solicitada y actualiza el tamaño en memoria solicitado por los datos y 
//la capacidad de la pila
static bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva);

//PRIMITIVAS DE MANIPULACIÓN DE LA PILA

pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    
    if(pila == NULL)
        return NULL;
    pila->datos = malloc(VALOR_INICIAL * sizeof(void*));

    pila->capacidad = VALOR_INICIAL; //Valor arbitrario
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad)
        if(!pila_redimensionar(pila, pila->cantidad * 2)) return false;
    if(pila == NULL) return false;

    size_t pos = pila->cantidad;
    pila->datos[pos] = valor;
    pila->cantidad++;
    return true;
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila))
        return NULL;
    void *desapilado = pila->datos[pila->cantidad - 1]; 
    pila->cantidad--;
    size_t cantidad = pila->cantidad;
    size_t capacidad = pila->capacidad;
    if(cantidad * MULTIPLICADOR <= capacidad && capacidad > VALOR_INICIAL)
        pila_redimensionar(pila, pila->capacidad / 2);
    return desapilado;
}

static bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva){
    void *datos_redim = realloc(pila->datos, sizeof(void*) * capacidad_nueva);
    if (datos_redim == NULL) return false;
    pila->datos = datos_redim;
    pila->capacidad = capacidad_nueva;
    return true;
}

//PRIMITIVAS DE VERIFICACIÓN DE LA PILA

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0); 
}

void *pila_ver_tope(const pila_t *pila){
    return (!pila_esta_vacia(pila)) ? pila->datos[pila->cantidad - 1] : NULL;     
}
