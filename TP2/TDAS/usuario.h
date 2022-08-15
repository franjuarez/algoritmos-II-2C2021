#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "heap.h"
#include "abb.h"

typedef void (*destruir_elemento_t)(void *e);
typedef struct usuario usuario_t;
typedef struct post post_t;

/* Crea un Usuario. Recibe por parámetro la función de comparación a
 * utilizar en su feed y la posicion. 
 * Devuelve un puntero al usuario, el cual debe ser destruido con
 * usuario_destruir().
 */
usuario_t* usuario_crear(size_t posicion, char* nombre, cmp_func_t cmp);

/* Elimina el usuario, llamando a la función dada para cada elemento del feed. 
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del feed. El usuario
 * dejó de ser válido. */
void usuario_destruir(usuario_t* usuario, destruir_elemento_t destruir);

/* Devuelve la posición en la que entró el usuario
 * Pre: el usuario fue creado
*/
size_t usuario_ver_posicion(usuario_t* usuario);

/* Devuelve el nombre del usuario
 * Pre: el usuario fue creado
*/
char* usuario_ver_nombre(usuario_t* usuario);

/* Avanza del elemento actual del feed al próximo con más prioridad 
 * acorde a la función de comparación proporcionada, devuelve el post
 * actual.
 */
void* usuario_ver_y_avanzar_feed(usuario_t* usuario);

/* Agregar un elemento al feed.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el usuario fue creado.
 * Post: se agregó un nuevo elemento al feed.
*/ 
bool usuario_agregar_al_feed(usuario_t* usuario, void* elemento);

/* Crea un post nuevo con la frase y el id pasados por parámetro
 * Pre: el usuario existe
 * Post: Devuelve un post creado con los parámetros 
*/
post_t* usuario_post_crear(char* nombre_usuario, char* contenido, size_t id);

/* Destruye el post pasado por parámetro
 * Pre: el post fue creado
 * Post: el post es destruido e inaccesible 
*/
void usuario_post_destruir(post_t* post);

/* Agrega un like al post indicado por el usuario
 * Pre: el post fue creado
 * Post: el post fue likeado por el usuario
*/
bool usuario_likear_post(post_t* post, char* usuario);

/* Muestra el id del post indicado
 * Pre: el post fue creado
*/
size_t usuario_post_ver_id(post_t* post);

/* Muestra el autor del post indicado
 * Pre: el post fue creado
*/
char* usuario_post_ver_autor(post_t* post);

/* Muestra el contenido del post indicado
 * Pre: el post fue creado
*/
char* usuario_post_ver_contenido(post_t* post);

/* Muestra el id del post indicado
 * Pre: el post fue creado
*/
size_t usuario_post_cant_likes(post_t* post);

/* Muestra los nombres de las personas que likearon
 * el post indicado por parámetro, ordenados alfabéticamente
 * Pre: el post fue creado
*/
char** usuario_mostrar_likes(post_t* post);




