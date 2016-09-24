/*
 * comunicacion.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_COMUNICACION_H_
#define MAPA_MAPA_COMMONS_COMUNICACION_H_
#include "comunication-point.h"
#include "map-commons.h"
#include "basic-structs.h"

enum RESPUESTA_DEL_ENTRENADOR
{
	ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST = 11,
	ENTRENADOR_QUIERE_MOVERSE = 12,
	ENTRENADOR_QUIERE_CAPTURAR_POKEMON = 13,
	ENTRENADOR_FINALIZO_OBJETIVOS = 14,
	ENTRENADOR_OTORGA_SU_SIMBOLO = 15,
	ENTRENADOR_DESCONECTADO = 16,
	SOLICITUD_DEL_ENTRENADOR = 17
} ;

enum ENVIOS_AL_ENTRENADOR
{
	OTORGAR_TURNO = 1,
	OTORGAR_COORDENADAS_POKENEST = 2,
	OTORGAR_MEDALLA_DEL_MAPA = 3,
	OTORGAR_POKEMON = 4,
	AVISAR_BLOQUEO_A_ENTRENADOR = 5,
	AVISAR_DESBLOQUEO_A_ENTRENADOR =6
};

#define MAX_BYTES_COORDENADA 5
#define MAX_BYTES_MEDALLA_DEL_MAPA 40
#define MAX_BYTES_RUTA_POKEMON 40
#define MAX_BYTES_TOTAL_A_ENVIAR 100

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
int tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador);

void enviar_mensaje_a_entrenador(t_entrenador *entrenador, int header, char *payload);

char* escuchar_mensaje_entrenador(t_entrenador *entrenador, int header);

char* armar_mensaje(char *header, char *payload);

char* armar_coordenada(int x, int y);

t_posicion* desarmar_coordenada(char *coordenada);

void dar_pokemon_a_entrenador(t_entrenador *entrenador,char *ruta_pokemon);

void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla);


#endif /* MAPA_MAPA_COMMONS_COMUNICACION_H_ */
