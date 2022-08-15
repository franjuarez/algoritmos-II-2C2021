#ifndef _POSIX_C_SOURCE 
#define _POSIX_C_SOURCE 200809L     //Para que ande getline
#include "red_social.h"
#include "../constantes.h"
#endif

struct red_social{
    hash_t* usuarios;
    hash_t* posts;
    size_t ultimo_id_post;
    char* usuario_loggeado;
};

typedef struct post_feed{
    void* post;   
    int afinidad;
}post_feed_t;

static post_feed_t* post_feed_crear(void* post, int afinidad){
    post_feed_t* post_feed = malloc(sizeof(post_feed_t));
    if(!post_feed) return NULL;
    post_feed->post = post;
    post_feed->afinidad = afinidad;
    return post_feed;
}

static void post_feed_destruir(post_feed_t* post_feed){
    free(post_feed);
}

static void post_feed_destruir_wrapper(void* elemento){
    post_feed_destruir((post_feed_t*)elemento);
}

static int afinidadcmp(const void* post1, const void* post2){
    int afi1 = ((post_feed_t*)post1)->afinidad;
    int afi2 = ((post_feed_t*)post2)->afinidad;
    int distancia = afi1 - afi2;
    if(distancia > 0) return -1;
    else if(distancia < 0) return 1;
    else{
        size_t id1 = usuario_post_ver_id(((post_feed_t*)post1)->post);
        size_t id2 = usuario_post_ver_id(((post_feed_t*)post2)->post);
        if(id1 < id2) return 1;
        else return -1;
    }
}

static void wrapper_usuario_destruir(void* elemento){
    usuario_destruir((usuario_t*)elemento, post_feed_destruir_wrapper);
}

static hash_t* cargar_usuarios(char* archivo){
    FILE* mi_archivo = fopen(archivo, "r");
    hash_t* usuarios = hash_crear(wrapper_usuario_destruir);
    size_t longitud = 100; //Igualmente getline me hace un realloc si lo necesita
    char* cadena = malloc(sizeof(char) * longitud);
    size_t pos = 0;
    while ( getline(&cadena, &longitud, mi_archivo) != EOF){ 
        char* caracter = "\n";
        size_t indice_caracter = strcspn(cadena, caracter);
        cadena[indice_caracter] = 0;
        usuario_t* usuario = usuario_crear(pos, cadena, afinidadcmp);
        hash_guardar(usuarios, cadena, usuario);
        pos++;
    }
    free(cadena);
    fclose(mi_archivo);
    return usuarios;
}

static void post_destruir_wrapper(void* elemento){
    usuario_post_destruir((post_t*)elemento);
}

red_social_t* red_social_crear(char* path){
    red_social_t* red_social = malloc(sizeof(red_social_t)); 
    hash_t* usuarios = cargar_usuarios(path);
    hash_t* posts = hash_crear(post_destruir_wrapper);
    red_social->usuarios = usuarios;
    red_social->posts = posts;
    red_social->usuario_loggeado = NULL;
    red_social->ultimo_id_post = 0;
    return red_social;
}

void red_social_destruir(red_social_t* red_social){
    hash_destruir(red_social->usuarios);
    hash_destruir(red_social->posts);
    if(red_social->usuario_loggeado) free(red_social->usuario_loggeado);
    free(red_social);
}

char* red_social_ver_loggeado(red_social_t* red_social){
    return red_social->usuario_loggeado;
}

void red_social_login(red_social_t* red_social, char* usuario){
    if(!hash_pertenece(red_social->usuarios, usuario)){
        fprintf(stdout, "%s\n", USUARIO_INEXISTENTE);
        return;
    }
    if (red_social_ver_loggeado(red_social)){
        fprintf(stdout, "%s\n", USUARIO_YA_LOGGEADO);
        return;
    }
    char* copia = malloc(sizeof(char) * (strlen(usuario) + 1));
    if(!copia) return;
    strcpy(copia, usuario);
    printf("Hola %s\n", usuario);
    red_social->usuario_loggeado = copia;
}

void red_social_logout(red_social_t* red_social){
    if (!red_social_ver_loggeado(red_social)){
        fprintf(stdout, "%s\n", USUARIO_NO_LOGGEADO);
        return;
    }
    free(red_social->usuario_loggeado);
    red_social->usuario_loggeado = NULL;
    fprintf(stdout, "%s\n", ADIOS);
}

static int calcular_afinidad(usuario_t* usuario_posteador, usuario_t* usuario_actual){
    int afinidad = (int) usuario_ver_posicion(usuario_posteador) - (int)usuario_ver_posicion(usuario_actual);
    if(afinidad < 0) afinidad *= -1;
    return afinidad;
}

void red_social_publicar(red_social_t* red_social, char* contenido){
    if (!red_social_ver_loggeado(red_social)){
        fprintf(stdout, "%s\n", USUARIO_NO_LOGGEADO);
        return;
    }
    hash_t* hash_usuarios = red_social->usuarios;
    hash_iter_t* iter = hash_iter_crear(hash_usuarios);
    char* usuario_nombre_posteador = red_social->usuario_loggeado; 
    usuario_t* usuario_posteador = hash_obtener(hash_usuarios, usuario_nombre_posteador);
    post_t* post = usuario_post_crear(usuario_nombre_posteador, contenido, red_social->ultimo_id_post); 
    int afinidad;

    while(!hash_iter_al_final(iter)){
        const char* usuario_nombre_actual = hash_iter_ver_actual(iter);

        usuario_t* usuario_actual = hash_obtener(hash_usuarios, usuario_nombre_actual);
        if(usuario_posteador == usuario_actual){
            hash_iter_avanzar(iter);    
            continue;
        }
        afinidad = calcular_afinidad(usuario_posteador, usuario_actual);
        post_feed_t* post_feed_actual = post_feed_crear(post, afinidad);
        usuario_agregar_al_feed(usuario_actual, post_feed_actual);
        hash_iter_avanzar(iter);
    }

    char id_clave[LONGITUD_MAX_ID];
    sprintf(id_clave, "%zu", red_social->ultimo_id_post);
    hash_guardar(red_social->posts, id_clave, post);
    red_social->ultimo_id_post++;

    fprintf(stdout, "%s\n", POST_PUBLICADO);
    hash_iter_destruir(iter);
}

void red_social_ver_siguiente_feed(red_social_t* red_social){
    if (!red_social_ver_loggeado(red_social)){
        fprintf(stdout, "%s\n", USUARIO_NO_POST_O_NO_LOGGEADO);
        return;
    }
    usuario_t* usuario_actual = hash_obtener(red_social->usuarios, red_social->usuario_loggeado);
    post_feed_t* post_feed_actual = usuario_ver_y_avanzar_feed(usuario_actual);
    if(!post_feed_actual){
        fprintf(stdout, "%s\n", USUARIO_NO_POST_O_NO_LOGGEADO);
        return;
    } 
    post_t* post_actual = post_feed_actual->post;
    printf("Post ID %ld\n", usuario_post_ver_id(post_actual));
    printf("%s dijo: %s\n", usuario_post_ver_autor(post_actual), usuario_post_ver_contenido(post_actual));
    printf("Likes: %ld\n", usuario_post_cant_likes(post_actual));
    free(post_feed_actual);
}

bool red_social_likear_post(red_social_t* red_social, char* id){
    if (!red_social_ver_loggeado(red_social) || !hash_pertenece(red_social->posts, id)){
        fprintf(stdout, "%s\n", USUARIO_NO_LOGGEADO_O_POST_INEXISTENTE);
        return false;
    }
    post_t* post_a_likear = hash_obtener(red_social->posts, id);
    bool guardado = usuario_likear_post(post_a_likear, red_social->usuario_loggeado);
    fprintf(stdout, "%s\n",POST_LIKEADO);
    return guardado;
}

void red_social_mostrar_likes(red_social_t* red_social, char* id){
    if (!hash_pertenece(red_social->posts, id)){
        fprintf(stdout, "%s\n", USUARIO_POST_INEXISTENTE_O_NO_LIKES);
        return;
    }
    post_t* post = hash_obtener(red_social->posts, id);
    if(usuario_post_cant_likes(post) == 0){
        fprintf(stdout, "%s\n", USUARIO_POST_INEXISTENTE_O_NO_LIKES);
            return;
    }
    char** nombres = usuario_mostrar_likes(post);
    printf("El post tiene %zu likes:\n", usuario_post_cant_likes(post));
    for(int i = 0; i < usuario_post_cant_likes(post); i++){
        printf("	%s\n", nombres[i]);
    }
    
    for(int i = 0; i < usuario_post_cant_likes(post); i++) free(nombres[i]);
    free(nombres);
}
