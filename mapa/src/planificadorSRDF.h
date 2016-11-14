/*
 * planificadorSRDF.h
 *
 *  Created on: 30/9/2016
 *      Author: utnso
 */

#ifndef PLANIFICADORSRDF_H_
#define PLANIFICADORSRDF_H_

#include "map-commons.h"
#include "comunicacion.h"
#include "basic-structs.h"
#include "planificador.h"
#include "unistd.h"

void planificador_srdf_reordena_entrenadores_si_es_necesario();
t_list* cola_listos_a_lista(t_queue *cola_listos);
void planificador_push_entrenador_en_cola_sin_objetivos(t_entrenador *entrenador);
void planificador_push_entrenador_a_listo_ordenado(t_entrenador *entrenador);
void planificador_srdf_move_lista_a_cola();
int comparador(void *arg1, void *arg2);

void* ejecutar_planificador_srdf(void* arg);
void planificador_srdf_organiza_entrenadores();
void planificador_srdf_es_el_turno_de(t_entrenador *entrenador);
void planificador_srdf_dale_nuevo_turno_hasta_que_se_bloquee(t_entrenador *entrenador);
void planificador_srdf_dale_pokemon_si_es_posible(t_entrenador *entrenador);
void planificador_srdf_cambia_semaforo_si_es_necesario();
/*--------------------------------------------HILOS--------------------------------------------------------*/
void planificador_srdf_organiza_entrenadores_sin_coordenadas();
void planificador_srdf_dale_coordenadas_a_todos();
void* planificador_srdf_atende_a_entrenadores_sin_coordenadas();
void planificador_srdf_ordena_cola_listos();

int entrenador_es_nuevo(t_entrenador *entrenador);
#endif /* PLANIFICADORSRDF_H_ */
