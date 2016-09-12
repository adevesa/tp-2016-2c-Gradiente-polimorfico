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
#include "basic-structs.h"
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
t_planificador_rr* planificador_rr_create();
t_planificador_srdf* planificador_srdf_create();

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg);

/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg);


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/

void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones);
void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores);


/*---------------------------------------NUEVO->LISTO---------------------------------------------------------*/
void planificador_encola_nuevos_entrenadores();
void planificador_modela_nuevo_entrenador_y_encolalo(void *id_proceso,void*cola_listos);
void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*));

#endif /* MAPA_COMMONS_PLANIFICADOR_H_ */
