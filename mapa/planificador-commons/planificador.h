/*
 * planificador.h
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */

#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>


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
t_planificador* planificador_create(t_queue *cola_listos, t_queue *cola_bloqueados, t_list *lista_nuevos, t_list *lista_finalizados);
t_planificador_rr* planificador_rr_create(int quantum, int retardo,t_queue *cola_listos, t_queue *cola_bloqueados, t_list *lista_nuevos, t_list *lista_finalizados);

#endif /* PLANIFICADOR_H_ */
