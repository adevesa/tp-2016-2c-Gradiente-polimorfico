/*
 * basic-structs.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_BASIC_STRUCTS_H_
#define MAPA_MAPA_COMMONS_BASIC_STRUCTS_H_
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <commons/config.h>
#include <commons/txt.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include "string.h"
#include <dirent.h>

/* ----------------------------------------STRUCTS----------------------------------------------------------------*/
typedef struct posicion
{
	int x;
	int y;
}t_posicion;


typedef struct entrenador_nuevo
{
	int id_proceso;
	int socket_entrenador;
}t_entrenador_nuevo;

typedef struct pokemon
{
	int nivel;
	char *ruta_en_pokedex;

}t_pokemon;

typedef struct pokeNest
{
	char *tipo;
	char *ruta_en_pokeDex;
	t_posicion *posicion;
	t_config *configuracion;
	char *identificador;
	t_queue *pokemones;
	int cantidad_pokemones_disponibles;
}t_pokeNest;

typedef struct entrenador
{
	int id_proceso; //<--- Es el dato más importante que nos interesa del entrenador;
	int socket_entrenador;
	t_posicion *posicion_actual;
	t_pokeNest *pokenest_objetivo;
	int distancia_hasta_objetivo;
	int objetivo_cumplido; //<-- será 1 si ya se cumplió el objetivo, 0 caso default;
	int estado; //<-- será 1 = listo, 0 = bloquedo, -1 = muerto;
	int tiempo_consumido;
	t_list *pokemones_capturados;
}t_entrenador;

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
	t_dictionary *pokeNests;
}t_mapa;

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_mapa* mapa_create(char *nombre, char *rutaPokedex);
t_controllers* controllers_create();
t_info_algoritmo* info_algoritmo_create(char *algoritmo, int quamtum, int retardo);
t_info_socket* info_socket_create(int puerto, char *ip);
t_entrenador* entrenador_create(int id_proceso, int socket_entrenador);
t_config* configuracion_metadata_create(char *nombre, char *ruta);

t_pokeNest* pokenest_create(char *nombre, char *ruta);
t_posicion* posicion_create(int x, int y);
t_pokemon* pokemon_create(char *ruta);

/*--------------------------------FUNCIONES PARA OBTENER DATOS BASICOS DE UN MAPA-----------------------------------*/

t_info_socket* obtener_info_mapa_socket(t_config *configuracion);
t_info_algoritmo* obtener_info_mapa_algoritmo(t_config *configuracion);
int obtener_info_mapa_batalla(t_config *configuracion);
int obtener_info_mapa_tiempo_deadlock(t_config *configuracion);
t_dictionary* obtener_info_mapa_pokenest(char *nombreMapa, char *rutaPokedex);
void foreach_pokenest_modelate(void *lista_origen,void *lista_destino, void *ruta);
char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo);
t_list* nombre_de_archivos_del_directorio(char *ruta);
/* ---------------------------------FUNCIONES PARA OBTENER DATOS DE UN POKENEST------------------------------------------*/
char* obtener_info_pokenest_tipo(t_config *configuracion);
t_posicion* obtener_info_pokenest_posicion(t_config *configuracion);
char* obtener_info_pokenest_id(t_config *configuracion);
t_queue* obtener_info_pokenest_pokemones(char *nombrePokenest, char *ruta);
void foreach_pokenest(void *lista_origen,void *lista_destino, void *ruta);

#endif /* MAPA_MAPA_COMMONS_BASIC_STRUCTS_H_ */
