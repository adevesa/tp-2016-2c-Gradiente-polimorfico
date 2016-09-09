/*
 * pokenest-commons.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef MAPA_COMMONS_POKENEST_COMMONS_H_
#define MAPA_COMMONS_POKENEST_COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>

/* ----------------------------------------ESTRUCTURAS--------------------------------------------------*/
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
	t_config *configuracion;
	char *identificador;
	t_list *pokemones;
}t_pokeNest;


/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_pokeNest* pokenest_create(char *nombre, char *ruta);

t_posicion* posicion_create(int x, int y);

t_pokemon* pokemon_create(char *ruta);


/* ---------------------------------------	HEADERS FUNCIONES----------------------------------------------------------*/
char* obtener_info_pokenest_tipo(t_config *configuracion);

t_posicion* obtener_info_pokenest_posicion(t_config *configuracion);

char* obtener_info_pokenest_id(t_config *configuracion);

t_list* obtener_info_pokenest_pokemones(char *nombrePokenest, char *ruta);

char* obtener_ruta_determinada(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo);

#endif /* MAPA_COMMONS_POKENEST_COMMONS_H_ */
