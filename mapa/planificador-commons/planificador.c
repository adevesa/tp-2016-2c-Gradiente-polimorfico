/*
 * planificador.c
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */
#include "planificador.h"

/*--------------------------------------------CREATES---------------------------------------------------------------*/

t_planificador* planificador_create(t_queue *cola_listos, t_queue *cola_bloqueados, t_list *lista_nuevos, t_list *lista_finalizados)
{
	t_planificador *planificador_new = malloc(sizeof(t_planificador));
	planificador_new->cola_entrenadores_bloqueados=cola_bloqueados;
	planificador_new->cola_entrenadores_listos=cola_listos;
	planificador_new->lista_entrenadores_a_planificar=lista_nuevos;
	planificador_new->lista_entrenadores_finalizados=lista_finalizados;
	return planificador_new;
}

t_planificador_rr* planificador_rr_create(int quantum, int retardo,t_queue *cola_listos, t_queue *cola_bloqueados, t_list *lista_nuevos, t_list *lista_finalizados)
{
	t_planificador_rr *planif_new = malloc(sizeof(t_planificador_rr));
	planif_new->planificador = planificador_create(cola_listos, cola_bloqueados, lista_nuevos, lista_finalizados);
	planif_new->quantum = quantum;
	planif_new->retardo = retardo;
	return planif_new;
}

t_planificador_srdf* planificador_srdf_create(t_queue *cola_listos, t_queue *cola_bloqueados, t_list *lista_nuevos, t_list *lista_finalizados)
{
	t_planificador_srdf *planif_new = malloc(sizeof(t_planificador_srdf));
	planif_new->planificador=planificador_create(cola_listos, cola_bloqueados, lista_nuevos, lista_finalizados);
	return planif_new;
}




