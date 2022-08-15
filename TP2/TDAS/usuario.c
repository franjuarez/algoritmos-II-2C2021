#include "usuario.h"

struct usuario{
    size_t posicion;
    heap_t* feed;
    char* nombre;
};

struct post{
    size_t id;
    abb_t* likes;
    char* contenido;
    char* autor;
};

static char* copiar_a_direccion_nueva(char* cadena){
    char* copia = malloc(sizeof(char) * (strlen(cadena) + 1));
    if(!copia) return NULL;
    strcpy(copia, cadena);
    return copia;
}

usuario_t* usuario_crear(size_t posicion, char* nombre, cmp_func_t cmp){
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if(!usuario) return NULL;
    usuario->nombre = copiar_a_direccion_nueva(nombre);
    if(!usuario->nombre){
        free(usuario);
        return NULL;
    }
    usuario->posicion = posicion;
    usuario->feed = heap_crear(cmp);
    return usuario;
}

void usuario_destruir(usuario_t* usuario, destruir_elemento_t destruir){
    heap_destruir(usuario->feed, destruir);
    free(usuario->nombre);
    free(usuario);
}

size_t usuario_ver_posicion(usuario_t* usuario){
    return usuario->posicion;
}

char* usuario_ver_nombre(usuario_t* usuario){
    return usuario->nombre;
}

void* usuario_ver_y_avanzar_feed(usuario_t* usuario){
    if(heap_esta_vacio(usuario->feed)) return NULL;
    return heap_desencolar(usuario->feed);
}

bool usuario_agregar_al_feed(usuario_t* usuario, void* elemento){
    return heap_encolar(usuario->feed, elemento);
}

void usuario_post_destruir(post_t* post){
    abb_destruir(post->likes);
    free(post->contenido);
    free(post->autor);
    free(post);
}

post_t* usuario_post_crear(char* nombre_usuario, char* contenido, size_t id){
    post_t* post = malloc(sizeof(post_t));
    if(!post) return NULL;
    post->contenido = copiar_a_direccion_nueva(contenido);
    if(!post->contenido){
        free(post);
        return NULL;
    }
    post->autor = copiar_a_direccion_nueva(nombre_usuario);
    if(!post->autor){ 
        free(post);
        free(post->contenido);
        return NULL;
    }
    post->id = id;
    post->likes = abb_crear(strcmp, NULL); //set con prioridad
    return post;
}

bool usuario_likear_post(post_t* post, char* usuario){
    if(abb_pertenece(post->likes, usuario)) return false;
    return abb_guardar(post->likes, usuario, NULL);
}

typedef struct arr_tam{
    char** arr;
    size_t tam;
}arr_tam_t;

static bool visitar_nombres_usuarios_likes(const char* clave, void* dato, void* extra){
    arr_tam_t* arr_tam = extra;
    arr_tam->arr[arr_tam->tam] = copiar_a_direccion_nueva((char*)clave);
    arr_tam->tam++;
    return true;
}

char** usuario_mostrar_likes(post_t* post){
    char** nombres = malloc(sizeof(char*) * usuario_post_cant_likes(post)); 
    arr_tam_t* arr_tam = malloc(sizeof(arr_tam_t));
    arr_tam->arr = nombres;
    arr_tam->tam = 0;
    abb_in_order(post->likes, visitar_nombres_usuarios_likes, (void*)arr_tam);
    free(arr_tam);
    return nombres;
}

size_t usuario_post_ver_id(post_t* post){
    return post->id;
}

char* usuario_post_ver_autor(post_t* post){
    return post->autor;
}

char* usuario_post_ver_contenido(post_t* post){
    return post->contenido;
}

size_t usuario_post_cant_likes(post_t* post){
    return abb_cantidad(post->likes);
}
