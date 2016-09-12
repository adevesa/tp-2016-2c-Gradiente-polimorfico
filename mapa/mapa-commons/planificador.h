/*
 * planificador.h
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */

#ifndef MAPA_COMMONS_PLANIFICADOR_H_
#define MAPA_COMMONS_PLANIFICADOR_H_
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include "map-commons.h"
#include "comunicacion.h"
/* ----------------------------------------STRUCTS----------------------------------------------------------------*/
typedef struct planificador
{
	t_queue *cola_entrenadores_listos;
	t_queue *cola_entrenadores_bloqueados;
	t_list *lista_entrenadores_finalizados;
	t_list *lista_entrenadores_a_planificar;
}t_planificador;

typedef struct round_robbin
{
	t_planificador *planificador;
	int quantum;
	int retardo;
}t_planificador_rr;

typedef struct srdf
{
	t_planificador *planificador;

}t_planificador_srdf;

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_planificador* planificador_create();
t_planificador_rr* planificador_rr_create(void *arg);
t_planificador_srdf* planificador_srdf_create(void *arg);

#endif /* MAPA_COMMONS_PLANIFICADOR_H_ */
