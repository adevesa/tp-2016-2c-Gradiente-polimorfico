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
bool pokenest_tipoo(void *arg);
/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/

/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores();
char* buscar_medalla_del_mapa();

/*
 * FUNCIONES PARA MANIPULACION DEL PLANIFICADOR
 */


#endif /* MAPA_MAPA_COMMONS_MAP_COMMONS_H_ */
