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
#include "comunicacion.h"
#include "basic-structs.h"

enum RESPUESTA_DEL_ENTRENADOR
{
	ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST,
	ENTRENADOR_QUIERE_MOVERSE,
	ENTRENADOR_QUIERE_CAPTURAR_POKEMON,
	ENTRENADOR_FINALIZO_OBJETIVOS
} ;

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

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg);

/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg);

/*--------------------------------------------ROUND ROBBIN----------------------------------------------------------*/
void planificador_rr_ordena(t_planificador_rr *planificador);
int quamtum_se_termino(int q);
t_entrenador* planificador_rr_pop_entrenador_listo(t_planificador_rr *planificador);


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/

void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones);
void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores);
void planificador_trata_captura_pokemon(t_planificador_rr *planificador, t_entrenador *entrenador);

/*---------------------------------------NUEVO->LISTO---------------------------------------------------------*/
void planificador_encola_nuevos_entrenadores();
void planificador_modela_nuevo_entrenador_y_encolalo(void *id_proceso,void*cola_listos);
void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*));

/*---------------------------------------EXECUTE->BLOQUEADO---------------------------------------------------------*/
void planificador_bloquea_entrenador(t_entrenador *entrenador, t_planificador_rr *planificador);
void planificador_bloquea_definitivamente_entrenador(t_entrenador *entrenador);

/*---------------------------------------BLOQUEADO -> EXECUTE---------------------------------------------------------*/
void planificador_desbloquea_entrenador(t_planificador_rr *planificador);

/*---------------------------------------EXECUTE->LISTO---------------------------------------------------------*/
void planificador_pone_a_listo_entrenador(t_planificador_rr *planificador, t_entrenador *entrenador);

/*---------------------------------------EXECUTE->FINALIZADO---------------------------------------------------------*/
void planificador_finaliza_entrenador(t_planificador_rr *planificador, t_entrenador *entrenador);
#endif /* MAPA_MAPA_COMMONS_PLANIFICADOR_H_ */
