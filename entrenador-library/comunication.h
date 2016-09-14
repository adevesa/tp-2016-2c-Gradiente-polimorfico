/*
 * comunication.h
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#include "comunication-point.h"
#include "basic-structs.h"


#define TRUE 1
#define FALSE 0
/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_mapa(t_mapa *mapa);

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
void tratar_respuesta(char* respuesta_del_entrenador,t_entrenador *entrenador);
char* esperar_respuesta(t_mapa *mapa);

/*--------------------------------------MENSAJES QUE RECIBE EL ENTRENADOR----------------------------------------*/
void mapa_me_da_coordendas_pokenest(t_entrenador *entrenador);
void decodificar_coordenadas(char *payload, int *x, int*y);

void mapa_me_da_pokemon(t_entrenador *entrenador);

void mapa_me_da_medalla( t_entrenador *entrenador);

int mapa_me_bloqueo( t_entrenador *entrenador);

int mapa_me_desbloqueo( t_entrenador *entrenador);

void mapa_me_da_turno(t_entrenador *entrenador);


/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR----------------------------------------*/
void solicitar_ubicacion_pokenest(t_mapa *mapa,char *pokemonBuscado);

void solicitar_moverse(t_mapa *mapa,char *coordenadaDestino);

void solicitar_captura_pokemon(t_mapa *mapa, char *pokemonAcapturar);

void notificar_fin_objetivos(t_mapa *mapa);

char* armar_mensaje(char *header, char *payload);


#endif /* COMUNICATION_H_ */
