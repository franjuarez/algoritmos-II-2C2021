#include "usuario.h"
#define _POSIX_C_SOURCE 200809L
#include "hash.h"

// Estructura de la red social
typedef struct red_social red_social_t;

/* Crea una red social, recibe por parámetro
 * un path para cargar los usuarios.
 * devuelve un puntero a una red social, que se
 * destruye con red_social_destruir()
*/
red_social_t* red_social_crear(char* path);

/* Destruye la red social recibida por parámetro y queda
 * inaccesible
 * Pre: la red social fue creada exitosamente
 * Post: la red social fue destruida
*/
void red_social_destruir(red_social_t* red_social);

/* Recibe un usuario por parámetro y se encarga de
 * iniciar su sesion en la red social.
 * Pre: la red social fue creada
 * Post: el usuario indicado pasa a estar loggeado
*/
void red_social_login(red_social_t* red_social, char* usuario);

/* Muestra el nombre del usuario actualmente loggeado en
 * la red social o NULL en caso de no haber uno
 * Pre: la red social fue creada
*/
char* red_social_ver_loggeado(red_social_t* red_social);

/* Recibe un usuario por parámetro y se encarga de
 * terminar su sesion en la red social.
 * Pre: la red social fue creada
 * Post: el usuario termina su sesión en la red social 
*/
void red_social_logout(red_social_t* red_social);

/* Se encarga de publicar un post indicado con el usuario actualmente
 * loggeado, agregandolo a los feeds del resto de usuarios
 * Pre: la red social fue creada
 * Post: el post fue subido y agregado a los feeds de los demás usuarios
*/
void red_social_publicar(red_social_t* red_social, char* contenido); //Hacer bool?

/* Actualiza el feed del usuario actualmente loggeado, mostrando el actual
 * y avanzando al siguiente en el feed. Muestra un error si no hay más posts
 * para ver.
 * Pre: la red social fue creada y existen publicaciones
 * Post: se avanza en el feed del usuario actualmente loggeado
*/
void red_social_ver_siguiente_feed(red_social_t* red_social);

/* Se encarga de likear un post indicado teniendo en cuenta
 * el usuario actualmente loggeado en la red social
 * Pre: la red social fue creada
 * Post: el post indicado tiene un like más, del usuario actual
*/
bool red_social_likear_post(red_social_t* red_social, char* id);

/* Muestra los nombres de los usuarios que likearon el post indicado
 * ordenados alfabéticamente.
 * Pre: la red social fue creada
*/
void red_social_mostrar_likes(red_social_t* red_social, char* id);








