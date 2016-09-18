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
#include <nivel-gui/nivel.h>
#include<nivel-gui/tad_items.h>
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
int mapa_decime_si_entrenador_esta_bloqueado(t_entrenador *entrenador);
int mapa_decime_si_entrenador_estaba_bloqueado(t_entrenador *entrenador);
int mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(t_entrenador *entrenador);
void mapa_cambiale_estado_a_entrenador(t_entrenador *entrenador, int estado_entrante, int estado_saliente);
void mapa_devolve_pokemon_a_pokenest(t_pokemon *pokemon);
int mapa_decime_si_planificador_es_rr();
int mapa_decime_si_entrenador_finalizo_su_objetivo(int socket_entrenador);

/*--------------------------------------------------- FUNCIONES PARA GRAFICAR--------------------------------------------*/
void mapa_mostrate_en_pantalla();
void mapa_mostra_actualizacion_de_entrenador(t_entrenador *entrenador);
void mapa_agrega_pokenest_a_items_para_pantalla();
void closure(char *identificador, void *data );
void mapa_mostra_nuevo_entrenador_en_pantalla(t_entrenador *entrenador);
void mapa_borra_entrenador_de_pantalla(t_entrenador *entrenador);

/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores();

#endif /* MAPA_MAPA_COMMONS_MAP_COMMONS_H_ */
