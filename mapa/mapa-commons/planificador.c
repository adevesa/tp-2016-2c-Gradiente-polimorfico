/*
 * planificador.c
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */
#include "planificador.h"
/*--------------------------------------------CREATES---------------------------------------------------------------*/

t_planificador* planificador_create()
{
	extern t_mapa *mapa;
	t_planificador *planificador_new = malloc(sizeof(t_planificador));
	planificador_new->cola_entrenadores_bloqueados=mapa->entrenadores->cola_entrenadores_bloqueados;
	planificador_new->cola_entrenadores_listos=mapa->entrenadores->cola_entrenadores_listos;
	planificador_new->lista_entrenadores_a_planificar=mapa->entrenadores->lista_entrenadores_a_planificar;
	planificador_new->lista_entrenadores_finalizados=mapa->entrenadores->lista_entrenadores_finalizados;
	return planificador_new;
}

t_planificador_rr* planificador_rr_create(void *arg)
{
	extern t_mapa *mapa;
	t_planificador_rr *planif_new = malloc(sizeof(t_planificador_rr));
	planif_new->planificador = planificador_create();
	planif_new->quantum = mapa->info_algoritmo->quamtum;
	planif_new->retardo = mapa->info_algoritmo->retardo;
	return planif_new;
}

t_planificador_srdf* planificador_srdf_create(void *arg)
{
	t_planificador_srdf *planif_new = malloc(sizeof(t_planificador_srdf));
	planif_new->planificador=planificador_create();
	return planif_new;
}




