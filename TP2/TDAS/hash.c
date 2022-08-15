#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"

size_t CAP_INICIAL = 10; 
float FACTOR_CARGA_MINIMO = 0.1f; 
float FACTOR_CARGA_MAXIMO = 2.5f;
size_t REDIM = 2;
int CANTIDAD_CARACTERES = 100;

//Función de hash llamada djb2, fuente: http://www.cse.yorku.ca/~oz/hash.html

unsigned long funcion_hash(const char *str, unsigned long m){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned long)c; /* hash * 33 + c */
    return hash % m;
}

//Implementaremos Hash Abierto.
//DEFINICIÓN DE ESTUCTURAS

struct hash{
    lista_t** tabla;
    size_t capacidad;
    size_t cantidad;
    hash_destruir_dato_t destruir_dato;
};

typedef struct campo{
    char* clave;
    void* dato;
}campo_t;

struct hash_iter{
    const hash_t* hash;
    lista_iter_t* lista_iter;
    int posicion_arreglo;
};


static campo_t* campo_crear(const char* clave, void* dato){
    campo_t* campo = malloc(sizeof(campo_t));
    if(!campo) return NULL;
    char* copia = malloc(sizeof(char) * (strlen(clave) + 1));
    if(!copia) return NULL;
    strcpy(copia,clave);
    campo->clave = copia;
    campo->dato = dato;
    return campo;
}

void campo_destruir(campo_t* campo){
    free(campo->clave);
    free(campo);
}

static float calcular_factor_de_carga(hash_t* hash){
    return ((float) hash->cantidad) / ((float) hash->capacidad);
}

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    lista_t** arreglo_listas = malloc(sizeof(lista_t*) * CAP_INICIAL);
    if(!arreglo_listas){
        free(hash);
        return NULL;
    }
    hash->tabla = arreglo_listas;
    hash->cantidad = 0;
    hash->capacidad = CAP_INICIAL;
    hash->destruir_dato = destruir_dato;
    for(int i = 0; i < CAP_INICIAL;i++){
        lista_t* lista_vacia = lista_crear();
        hash->tabla[i] = lista_vacia;
    }
    return hash;
}

bool visitar_redimension(void* campo, void* hash){
    campo_t* campo_actual = campo;
    hash_t* hash_aux = hash;
    const char* clave = campo_actual->clave;
    void* dato_campo = campo_actual->dato;
    hash_guardar(hash, clave, dato_campo);
    campo_destruir(campo_actual);
    hash_aux->cantidad--; 
    return true;
}

static bool hash_redimensionar(hash_t* hash, size_t capacidad_nueva){
    lista_t** tabla_nueva = malloc(capacidad_nueva * sizeof(lista_t*)); 
    if(!tabla_nueva) return false;
    lista_t** tabla_vieja = hash->tabla;
    size_t capacidad_vieja = hash->capacidad;
    for(int j = 0; j < capacidad_nueva; j++){
        lista_t* lista_vacia = lista_crear();
        tabla_nueva[j] = lista_vacia;
    }
    hash->tabla = tabla_nueva;
    hash->capacidad = capacidad_nueva;
    for(int i = 0; i < capacidad_vieja; i++){
        lista_t* lista_actual = tabla_vieja[i];
        if(lista_esta_vacia(lista_actual)){
            lista_destruir(lista_actual, NULL);
            continue;
        }
        lista_iterar(lista_actual, visitar_redimension, hash);
        lista_destruir(lista_actual, NULL); 
    }
    free(tabla_vieja);
    return true;
}

static campo_t* obtener_campo(const hash_t *hash, const char *clave, unsigned long clave_hasheada, bool borrar){
    lista_t* lista_actual = hash->tabla[clave_hasheada];
    if(lista_esta_vacia(lista_actual)) return NULL;
    lista_iter_t* iter = lista_iter_crear(lista_actual);
    campo_t* campo_buscado = NULL;
    while(!lista_iter_al_final(iter)){
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if(strcmp(campo_actual->clave, clave) == 0){ 
            campo_buscado = campo_actual;
            if(borrar) lista_iter_borrar(iter);
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return campo_buscado;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    unsigned long clave_hasheada = funcion_hash(clave, hash->capacidad);
    if(!hash_pertenece(hash, clave)){
        campo_t* campo = campo_crear(clave, dato);
        if(!campo) return false;
        lista_insertar_ultimo(hash->tabla[clave_hasheada], campo);
    } else{
        campo_t* campo_renovar = obtener_campo(hash, clave, clave_hasheada, false);
        void* dato_anterior = campo_renovar->dato;
        if(hash->destruir_dato) (hash->destruir_dato)(dato_anterior);
        campo_renovar->dato = dato;
        return true;
    }
    hash->cantidad++;
    if(calcular_factor_de_carga(hash) > FACTOR_CARGA_MAXIMO){
        hash_redimensionar(hash, (hash->capacidad) * REDIM);
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){ 
    unsigned long clave_hasheada = funcion_hash(clave, hash->capacidad);
    campo_t* campo_buscado = obtener_campo(hash, clave, clave_hasheada, true);
    if(!campo_buscado) return NULL;
    void* dato = campo_buscado->dato;
    campo_destruir(campo_buscado);
    hash->cantidad--;
    if(calcular_factor_de_carga(hash) <= FACTOR_CARGA_MINIMO && hash->cantidad != 0){
        hash_redimensionar(hash, (hash->capacidad) / REDIM);
    }
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    unsigned long clave_hasheada = funcion_hash(clave, hash->capacidad);
    campo_t* campo_buscado = obtener_campo(hash, clave, clave_hasheada, false);
    if(!campo_buscado) return NULL;
    return campo_buscado->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    unsigned long clave_hasheada = funcion_hash(clave, hash->capacidad);
    campo_t* campo_buscado = obtener_campo(hash, clave, clave_hasheada, false);
    if(!campo_buscado) return false;
    return true;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for(int i = 0; i < hash->capacidad; i++){
        lista_t* lista_actual = hash->tabla[i];
        if(lista_esta_vacia(lista_actual)){
            lista_destruir(lista_actual, NULL);
            continue;
        }
        lista_iter_t* iter = lista_iter_crear(lista_actual);
        while(!lista_iter_al_final(iter)){
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            if(hash->destruir_dato) (hash->destruir_dato)(campo_actual->dato);
            campo_destruir(campo_actual);
            lista_iter_borrar(iter);
        }
        lista_iter_destruir(iter);
        lista_destruir(lista_actual, NULL);
    }
    free(hash->tabla);
    free(hash);
}

//PRIMITIVAS DEL ITERADOR EXTERNO

static void buscar_lista(hash_iter_t* iter){
    int i = iter->posicion_arreglo+1;
    for(; i < iter->hash->capacidad; i++) if(!lista_esta_vacia(iter->hash->tabla[i])) break;
    iter->posicion_arreglo = i;
}

hash_iter_t* hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(!iter) return NULL;
    iter->hash = hash;
    iter->posicion_arreglo = -1;
    buscar_lista(iter);
    iter->lista_iter = (!hash_iter_al_final(iter)) ? lista_iter_crear(hash->tabla[iter->posicion_arreglo]) : NULL;
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(!iter->lista_iter) return false;
    if(hash_iter_al_final(iter)) return false;
    lista_iter_avanzar(iter->lista_iter);    
    if(lista_iter_al_final(iter->lista_iter)){
        buscar_lista(iter);
        lista_iter_destruir(iter->lista_iter);
        iter->lista_iter = NULL;
        if(hash_iter_al_final(iter)) return false;
        iter->lista_iter = lista_iter_crear(iter->hash->tabla[iter->posicion_arreglo]);
    } 
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(!iter->lista_iter || hash_iter_al_final(iter)) return NULL;
    campo_t* campo_actual = lista_iter_ver_actual(iter->lista_iter);
    return campo_actual->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->posicion_arreglo == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t *iter){
    if(iter->lista_iter) lista_iter_destruir(iter->lista_iter);
    free(iter);
}
