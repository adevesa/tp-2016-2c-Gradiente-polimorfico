/*
 * planificador.h
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_PLANIFICADOR_H_
#define MAPA_MAPA_COMMONS_PLANIFICADOR_H_
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include "map-commons.h"
#include "planificador-rr.h"
#include "basic-structs.h"



/* ----------------------------------------STRUCTS----------------------------------------------------------------*/
typedef struct listas_y_colas
{
	t_queue *cola_entrenadores_listos;
	t_queue *cola_entrenadores_bloqueados;
	t_list *lista_entrenadores_finalizados;
	t_list *lista_entrenadores_a_planificar;
}t_listas_y_colas;

typedef struct round_robbin
{
	t_listas_y_colas *listas_y_colas;
	int quantum;
	int retardo;
}t_planificador_rr;

typedef struct srdf
{
	t_listas_y_colas *listas_y_colas;

}t_planificador_srdf;

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_listas_y_colas* listas_y_colas_creense();
t_planificador_rr* planificador_rr_create();
t_planificador_srdf* planificador_srdf_create();


/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg);

/*---------------------------------------PUSH Y POPS DE COLAS---------------------------------------------------------*/
void planificador_rr_push_entrenador_a_bloqueado(t_entrenador *entrenador);
void planificador_rr_push_entrenador_a_listo(t_entrenador *entrenador);
t_entrenador* planificador_rr_pop_entrenador_bloqueado();
t_entrenador* planificador_rr_pop_entrenador_listo();

/*-------------------------------------------FUNCIONES GENERALES--------------------------------------------------------*/
void planificador_dale_coordenadas_a_entrenador(t_entrenador *entrenador);
void planificador_entrenador_se_mueve(t_entrenador *entrenador);
void planificador_entrenador_quiere_capturar_pokemon(t_entrenador *entrenador);
void planificador_bloquea_entrenador(t_entrenador *entrenador);

#endif /* MAPA_MAPA_COMMONS_PLANIFICADOR_H_ */
