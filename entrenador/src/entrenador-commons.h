/*
 * entrenador-commons.h
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#ifndef ENTRENADOR_COMMONS_H_
#define ENTRENADOR_COMMONS_H_
#include <so-commons/log.h>

#include "comunication.h"
#include "basic-structs.h"
#include "time.h"
#include "semaphore.h"
#include "pthread.h"
#include "movimiento.h"

t_log *info_entrenador;
t_entrenador *entrenador;

enum time
{
	INICIO,
	FIN
};

char *hora_de_inicio;
char *hora_de_fin;

//void iniciar_semaforos();
void iniciar_log(char *nombre_del_entrenador);
void entrenador_registra_hora(int rango);

void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex);
void entrenador_recorre_hoja_de_viaje();
void entrenador_cumpli_objetivos_del_mapa();
void entrenador_cumpli_objetivo(int indice_objetivo);
void entrenador_busca_mapa(int index);
void entrenador_copia_medalla_del_mapa();


void entrenador_espera_turno();
void entrenador_espera_a_que_mapa_te_bloquee();
void entrenador_espera_a_que_mapa_te_desbloquee();



void entrenador_pedi_ubicacion_pokenest(int indice_objetivo);
void entrenador_recibi_y_copia_pokemon(char *solicitud);
void entrenador_registra_tiempo_bloqueo(char *hora_inicio, char *hora_fin);

void entrenador_informa_movimiento();
void entrenador_avisa_que_terminaste_en_este_mapa();
void entrenador_captura_pokemon(int indice_obejtivo);;

#endif /* ENTRENADOR_COMMONS_H_ */
