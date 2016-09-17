/*
 * comunicacion.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_COMUNICACION_H_
#define MAPA_MAPA_COMMONS_COMUNICACION_H_
#include "comunication-point.h"
#include "string.h"
#include <commons/string.h>
#include "basic-structs.h"
#include "map-commons.h"

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
int tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador);

/*-------------------------------------------OTORGAR TURNO-----------------------------------------------------------*/
void otorgar_turno_a_entrenador(t_entrenador *entrenador);
char* armar_mensaje(char *header, char *payload);

/*-------------------------------------------ESCUCHA AL ENTRENADOR-----------------------------------------------------------*/
char* escuchar_al_entrenador(t_entrenador *entrenador);

/*-------------------------------------------RECIBE Y RESPONDE-----------------------------------------------------------*/

/*--------------------UBICACION POKENEST---------------------------------------------------------------------------------*/
void darle_coordenadas_pokenst_a_entrenador(t_entrenador *entrenador);
char* escuchar_que_pokemon_busca(t_entrenador *entrenador);
void otorgar_posicion_pokenest_a_entrenador(t_entrenador *entrenador, t_pokeNest *pokenest_buscado);

/*--------------------MOVIMIENTO DEL ENTRENADOR---------------------------------------------------------------------------*/
void escuchar_a_que_direccion_se_mueve_entrenador(t_entrenador *entrenador);
void decodificar_coordenadas(char *payload, int *x, int *y);
void entrenador_se_va_a_mover(t_entrenador *entrenador);

/*--------------------CAPTURA POKEMON---------------------------------------------------------------------------------------*/
char* escuchar_captura_pokemon(int entrenador);
void avisar_bloqueo_a_entrenador(int entrenador);
/*----------------OTORGANDO POKEMON------------------------------------------------------------------------------------------*/
void dar_pokemon_a_entrenador(t_entrenador *entrenador, t_pokemon *pokemon);
void avisar_desbloqueo_a_entrenador(int entrenador);
void otorgar_pokemon_a_entrenador(t_entrenador *entrenador,t_pokemon *pokemon);


/*--------------------FIN DE OBJETIVOS DE ENTRENADOR-------------------------------------------------------------------------*/
void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla);


#endif /* MAPA_MAPA_COMMONS_COMUNICACION_H_ */
