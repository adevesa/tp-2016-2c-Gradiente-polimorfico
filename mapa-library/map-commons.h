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
char* mapa_dame_medalla();
t_pokeNest* mapa_buscame_pokenest(char *identificador_pokemon);
t_posicion* mapa_dame_coordenadas_de_pokenest(char* identificador_pokemon);
int mapa_decime_si_hay_pokemones_en_pokenest(t_pokeNest *pokenest);
t_pokemon* mapa_dame_pokemon_de_pokenest(t_pokeNest *pokenest);
void mapa_actualiza_pokemones_disponibles_de_pokenest(t_pokeNest *pokenest);

/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/
int mapa_decime_si_entrenador_esta_bloqueado(t_entrenador *entrenador);

int mapa_decime_si_entrenador_estaba_bloqueado(t_entrenador *entrenador);

int mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(t_entrenador *entrenador);

void mapa_cambiale_estado_a_entrenador(t_entrenador *entrenador, int estado_entrante, int estado_saliente);

/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores();
t_pokemon* pokenest_dame_pokemon(t_pokeNest *pokenest);
void pokenest_actualiza_tu_cantidad_pokemones_disponibles(t_pokeNest *pokenest);



#endif /* MAPA_MAPA_COMMONS_MAP_COMMONS_H_ */
