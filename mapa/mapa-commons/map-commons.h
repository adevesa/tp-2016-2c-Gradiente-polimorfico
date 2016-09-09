/*
 * map-commons.h
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */

#ifndef MAPA_COMMONS_MAP_COMMONS_H_
#define MAPA_COMMONS_MAP_COMMONS_H_

/* ----------------------------------------STRUCTS----------------------------------------------------------------*/
typedef struct posicion t_posicion;
typedef struct pokemon t_pokemon;


typedef struct controllers
{
	t_queue *cola_entrenadores_listos;
	t_queue *cola_entrenadores_bloqueados;
	t_list *lista_entrenadores_finalizados;
	t_list *lista_entrenadores_a_planificar;
}t_controllers;

typedef struct info_socket
{
	char *ip;
	int puerto;
}t_info_socket;

typedef struct info_algoritmo
{
	char *algoritmo;
	int quamtum;
	int retardo;
}t_info_algoritmo;

/*
//MUDADO A POKENEST-COMMMONS
typedef struct pokemon
{
	int nivel;

}t_pokemon;

typedef struct posicion
{
	int x;
	int y;
}t_posicion;

typedef struct pokeNest
{
	char *tipo;
	t_posicion *posicion;
	char identificador;
	t_list *pokemones;
}t_pokeNest;
//HASTA ACA
*/

//SOLO SE GUARDA LO MINIMO NECESARIO DEL ENTRENADOR//
typedef struct entrenador
{
	int id_proceso; //<--- Es el dato más importante que nos interesa del entrenador;
	t_posicion *posicion_actual;
	int objetivo_cumplido; //<-- será 1 si ya se cumplió el objetivo, 0 caso default;
	int estado; //<-- será 1 = listo, 0 = bloquedo, -1 = muerto;
	int tiempo_consumido;
	t_list *pokemones_capturados;
}t_entrenador;

typedef struct mapa
{
	char *nombre;
	char *ruta_pokedex;
	t_config *configuracion;
	t_controllers *entrenadores;
	int tiempo_chequeo_deadlock;
	int batalla;
	t_info_algoritmo *info_algoritmo;
	t_info_socket *info_socket;
	t_list *pokeNests;
}t_mapa;





/* ----------------	HEADERS FUNCIONES----------------------*/

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_mapa* mapa_create(char *nombre, char *rutaPokedex);
t_controllers* controllers_create();
t_info_algoritmo* info_algoritmo_create(char *algoritmo, int quamtum, int retardo);
t_info_socket* info_socket_create(int puerto, char *ip);

/*
t_pokemon* pokemon_create(int nivel);
t_posicion* posicion_create(int x, int y);
t_pokeNest* pokenest_create(char *nombreDelMapa, char *rutaPokedex);
*/

t_entrenador* entrenador_create(int id_proceso);
/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* mapa_dame_ubicacion_pokenest(t_list *pokenest,char *nombrePokemon);
t_pokemon* mapa_atrapa_pokemon(char *nombrePokemon);

/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/
/*
 * FUNCIONES PARA OBTENER DATOS BASICOS DE UN MAPA
 */

t_info_socket* obtener_info_mapa_socket(t_config *configuracion);
t_info_algoritmo* obtener_info_mapa_algoritmo(t_config *configuracion);
int obtener_info_mapa_batalla(t_config *configuracion);
int obtener_info_mapa_tiempo_deadlock(t_config *configuracion);

/*
char* obtener_info_pokenest_tipo(char *nombreMapa, char *rutaPokedex);
t_posicion* obtener_info_pokenest_posicion(char *nombreMapa, char *rutaPokedex);
int obtener_info_pokenest_id(char *nombreMapa, char *rutaPokedex);
t_list* obtener_info_pokenest_pokemones(char *nombreMapa, char *rutaPokedex);
*/

t_list* obtener_info_mapa_pokenest(char *nombreMapa, char *rutaPokedex);

char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo);

/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void ejecutar_hilo_conexiones_entrenadores(t_mapa *mapa);



/*
 * FUNCIONES PARA MANIPULACION DEL PLANIFICADOR
 */
void planificador_modela_nuevo_entrenador_y_encolalo(int id_proceso,t_queue *cola_listos);
void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones);
void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores);
void planificador_crea_nuevo_entrenador_en_mapa_si_es_necesario(t_controllers *listas_y_colas);
void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*));




#endif /* MAPA_COMMONS_MAP_COMMONS_H_ */
