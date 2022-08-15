#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

size_t CAPACIDAD_INICIAL = 10;
size_t REDIMENSION = 2;
size_t MULTIPLICAR = 4;

struct heap{
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

void swap(void** arr, size_t padre, size_t hijo){
    void* aux = arr[padre];
    arr[padre] = arr[hijo];
    arr[hijo] = aux;
}

static void upheap(cmp_func_t cmp, int hijo, void** arr){
    if(hijo < 1) return;
    int padre = (hijo - 1) / 2;
    int comparacion = cmp(arr[hijo], arr[padre]);
    if (comparacion > 0){
        swap(arr, (size_t) padre, (size_t) hijo);
        upheap(cmp, padre, arr);
    }
}

static size_t maximo(void** arr, cmp_func_t cmp, size_t h_der, size_t h_izq, size_t padre){
    size_t prim_max = cmp(arr[h_der], arr[h_izq]) > 0 ? h_der : h_izq;
    return (cmp(arr[prim_max], arr[padre]) > 0) ? prim_max : padre;
}

static void downheap(cmp_func_t cmp, void** arr, size_t padre, size_t cant){
    if(2 * padre >= cant-1) return; 
    size_t h_izq = (2 * padre) + 1;
    size_t h_der = (2 * padre) + 2; 
    size_t max;
    if(h_der > cant-1) max = cmp(arr[h_izq], arr[padre]) > 0 ? h_izq : padre;
    else max = maximo(arr, cmp, h_der, h_izq, padre);
    if(max != padre){
        swap(arr, padre, max);
        downheap(cmp, arr, max, cant);
    }
}

void heapify(void* elementos[], size_t cant, cmp_func_t cmp){
    int i = (int) cant/2;
    for(; i >= 0; i--){
        downheap(cmp, elementos, (size_t) i, cant);
    }
}

void _heap_sort(void* elementos[], size_t cant, cmp_func_t cmp){
    if(cant < 2) return;
    swap(elementos, 0, cant-1);
    downheap(cmp, elementos, 0, cant-1);
    _heap_sort(elementos, cant-1, cmp);
}

void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, cant, cmp);
    _heap_sort(elementos, cant, cmp);
}

static heap_t* _heap_crear(size_t capacidad, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    heap->datos = malloc(capacidad * sizeof(void*));
    if(!heap->datos){
        free(heap);
        return NULL;
    }
    heap->cantidad = 0; 
    heap->capacidad = capacidad;
    heap->cmp = cmp;
    return heap;
}

heap_t* heap_crear(cmp_func_t cmp){
    return _heap_crear(CAPACIDAD_INICIAL, cmp);
}

heap_t* heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = _heap_crear(n + CAPACIDAD_INICIAL, cmp);
    for(int i = 0; i < n; i++) heap->datos[i] = arreglo[i];
    heapify(heap->datos, n, cmp);
    heap->cantidad = n;
    return heap;
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
    return (heap->cantidad == 0) ? true : false;
}

void* heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    return heap->datos[0]; 
}

bool heap_redimensionar(heap_t* heap, size_t capacidad_final){
    void** datos_nuevos = realloc(heap->datos, capacidad_final * sizeof(void*));
    if (!datos_nuevos) return false;
    heap->capacidad = capacidad_final;
    heap->datos = datos_nuevos;
    return true;
}

bool heap_encolar(heap_t *heap, void *elem){
    if(heap->cantidad == heap->capacidad){
        if (!heap_redimensionar(heap, REDIMENSION * heap->capacidad)) return false;
    }
    heap->datos[heap->cantidad] = elem;
    heap->cantidad++;
    int hijo = (int) heap->cantidad - 1;
    upheap(heap->cmp, hijo, heap->datos); 
    return true;
}

void* heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    void* dato = heap->datos[0];
    heap->cantidad--;
    if(!heap->cantidad) return dato;
    swap(heap->datos, 0, heap->cantidad);
    downheap(heap->cmp, heap->datos, 0, heap->cantidad);
    if(heap->capacidad > CAPACIDAD_INICIAL && (heap->cantidad)*MULTIPLICAR <= heap->capacidad){
        heap_redimensionar(heap, heap->capacidad / REDIMENSION);
    }
    return dato;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for(int i = 0; i < heap->cantidad; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}
