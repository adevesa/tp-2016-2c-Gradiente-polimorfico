/*
 * map-commons.h
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */

#ifndef MAPA_COMMONS_MAP_COMMONS_H_
#define MAPA_COMMONS_MAP_COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include "string.h"
#include <servidor.h>
#include <nivel.h>
#include <pthread.h>
#include "stdbool.h"
#include "planificador.h"
#include "basic-structs.h"

/* ----------------	HEADERS FUNCIONES----------------------*/

/*--------------------------------------------CREATES---------------------------------------------------------------*/
/*
t_mapa* mapa_create(char *nombre, char *rutaPokedex);
t_controllers* controllers_create();
t_info_algoritmo* info_algoritmo_create(char *algoritmo, int quamtum, int retardo);
t_info_socket* info_socket_create(int puerto, char *ip);
t_entrenador* entrenador_create(int id_proceso, int socket_entrenador);
t_config* configuracion_metadata_create(char *nombre, char *ruta);
*/
void planificador_create_segun_cual_seas();
/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* buscar_coordenadas(char* pokemon_buscado);
bool pokenest_tipo(void *arg);
/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/
/*
 * FUNCIONES PARA OBTENER DATOS BASICOS DE UN MAPA
 */
/*
t_info_socket* obtener_info_mapa_socket(t_config *configuracion);
t_info_algoritmo* obtener_info_mapa_algoritmo(t_config *configuracion);
int obtener_info_mapa_batalla(t_config *configuracion);
int obtener_info_mapa_tiempo_deadlock(t_config *configuracion);
t_list* obtener_info_mapa_pokenest(char *nombreMapa, char *rutaPokedex);
char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo);
*/
/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores();
bool pokenest_tipo(void *arg);


/*
 * FUNCIONES PARA MANIPULACION DEL PLANIFICADOR
 */
void mapa_modela_nuevo_entrenador_y_encolalo(void *id_proceso,void*cola_listos);
void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones);
void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores);
void mapa_encola_nuevos_entrenadores(t_controllers *listas_y_colas);
void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*));




#endif /* MAPA_COMMONS_MAP_COMMONS_H_ */
