/*
 * map-commons.h
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_MAP_COMMONS_H_
#define MAPA_MAPA_COMMONS_MAP_COMMONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include "string.h"
#include "nivel.h"
#include <pthread.h>
#include "stdbool.h"
#include "planificador.h"
#include "basic-structs.h"
#include "comunication-point.h"

void ejecutar_mapa(char *nombre, char *rutaPokedex);
/* ----------------	HEADERS FUNCIONES----------------------*/

/*--------------------------------------------CREATES---------------------------------------------------------------*/

void planificador_create_segun_cual_seas();


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* buscar_coordenadas(char* pokemon_buscado);
t_pokeNest* buscar_pokenest(char *identificador_pokemon);
int hay_pokemones_en_pokenest(t_pokeNest *pokenest);
char* buscar_medalla_del_mapa();

/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/

/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores();
t_pokemon* pokenest_dame_pokemon(t_pokeNest *pokenest);
void pokenest_actualiza_tu_cantidad_pokemones_disponibles(t_pokeNest *pokenest);



#endif /* MAPA_MAPA_COMMONS_MAP_COMMONS_H_ */
