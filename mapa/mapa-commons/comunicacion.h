/*
 * comunicacion.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef MAPA_COMMONS_COMUNICACION_H_
#define MAPA_COMMONS_COMUNICACION_H_
#include "comunication-point.h"
#include "string.h"
#include <commons/string.h>
#include "basic-structs.h"
#include "map-commons.h"

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
void tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador);

/*-------------------------------------------OTORGAR TURNO-----------------------------------------------------------*/
void otorgar_turno_a_entrenador(t_entrenador *entrenador);
char* armar_mensaje(char *header, char *payload);

/*-------------------------------------------ESCUCHA AL ENTRENADOR-----------------------------------------------------------*/
char* escuchar_al_entrenador(t_entrenador *entrenador);

/*-------------------------------------------RECIBE Y RESPONDE-----------------------------------------------------------*/

/*--------------------UBICACION POKENEST---------------------------------------------------------------------------------*/
void entrenador_busca_coordenadas_pokenest(t_entrenador *entrenador);
char* escuchar_que_pokemon_busca(t_entrenador *entrenador);
void otorgar_posicion_pokenest_a_entrenador(int entrenador, int x, int y);

/*--------------------MOVIMIENTO DEL ENTRENADOR---------------------------------------------------------------------------*/
void escuchar_a_que_direccion_se_mueve(t_entrenador *entrenador);
void decodificar_coordenadas(char *payload, int *x, int *y);
void entrenador_se_va_a_mover(t_entrenador *entrenador);

/*--------------------CAPTURA POKEMON---------------------------------------------------------------------------------------*/
void entrenador_quiere_capturar_pokemon(t_entrenador *entrenador);
char* escuchar_captura_pokemon(int entrenador);
void avisar_bloqueo_a_entrenador(int entrenador);
/*----------------OTORGANDO POKEMON------------------------------------------------------------------------------------------*/
void dar_pokemon_a_entrenador(t_entrenador *entrenador, t_pokemon *pokemon, int tiempo_bloqueado);
void avisar_desbloqueo_a_entrenador(int entrenador, int tiempo_bloqueado);
void otorgar_pokemon_a_entrenador(int entrenador, int nivelPokemon);


/*--------------------FIN DE OBJETIVOS DE ENTRENADOR-------------------------------------------------------------------------*/
void entrenador_quiere_finalizar_objetivos(t_entrenador *entrenador);
void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla);


#endif /* MAPA_COMMONS_COMUNICACION_H_ */
