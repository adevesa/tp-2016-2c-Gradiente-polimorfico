/*
 * comunication.h
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#include "basic-structs.h"
#include "socket/serializacion_mapa_entrenador.h"
#include "socket/cliente.h"
#include "commons/log.h"

enum RESPUESTAS_DEL_MAPA
{
	MAPA_ME_DA_TURNO = 1,
	MAPA_ME_DA_COORDENADAS_POKENEST = 2,
	MAPA_ME_DA_MEDALLA = 3,
	MAPA_ME_BLOQUEA = 4,
	MAPA_ME_DESBLOQUEA = 5,
	MAPA_ME_DA_POKEMON = 6,
	MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR = 7
};

enum ENVIOS_AL_MAPA
{
	OTORGAR_SIMBOLO_ENTRENADOR = 10,
	SOLICITAR_COORDENADAS_POKENEST = 11,
	SOLICITAR_CAPTURA_POKEMON = 12,
	REPORTAR_MOVIMIENTO = 13,
	REPORTAR_FIN_OBJETIVOS = 14
};

#define MAX_BYTES_COORDENADA 5
#define MAX_BYTES_TOTAL_A_ENVIAR 100

#define TRUE 1
#define FALSE 0

extern t_log *info_entrenador;
/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_mapa(t_mapa *mapa);
void entrenador_dale_a_mapa_tu_simbolo();
char* escuchar_mensaje_mapa(t_mapa *mapa, int header);
void enviar_mensaje_a_mapa(t_mapa *mapa, int header, char *payload);
int mapa_me_dice(char *mapa_dice);

/*--------------------------------------MENSAJES QUE RECIBE EL ENTRENADOR----------------------------------------*/
void mapa_me_da_coordendas_pokenest(t_entrenador *entrenador);

void decodificar_coordenadas(char *payload, int *x, int*y);

void mapa_me_da_pokemon(t_entrenador *entrenador);

void mapa_me_da_medalla( t_entrenador *entrenador);

void mapa_me_da_turno(t_entrenador *entrenador);


/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR----------------------------------------*/
void solicitar_ubicacion_pokenest(t_mapa *mapa,char *pokemonBuscado);

void solicitar_moverse(t_mapa *mapa,char *coordenadaDestino);

void solicitar_captura_pokemon(t_mapa *mapa, char *pokemonAcapturar);

void notificar_fin_objetivos(t_mapa *mapa);

/*---------------------------------------SECUNDARIOS----------------------------------------*/;
t_ubicacion* desarmar_coordenada(char *coordenada);
void copiar(char* origen, char* destino);
void eliminar(char* elemento);

#endif /* COMUNICATION_H_ */
