/*
 * planificador.h
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_PLANIFICADOR_H_
#define MAPA_MAPA_COMMONS_PLANIFICADOR_H_
#include <so-commons/log.h>
#include <stdio.h>
#include <stdlib.h>
#include "map-commons.h"
#include "planificador-rr.h"
#include "basic-structs.h"
#include "socket/serializacion_mapa_entrenador.h"

#define DESCONECTADO 0

t_log *informe_planificador;
extern t_log *informe_cola_listos;
extern t_log *informe_cola_bloqueados;
extern t_log *informe_cola_nuevos;


/* ----------------------------------------STRUCTS----------------------------------------------------------------*/
typedef struct listas_y_colas
{
	t_queue *cola_entrenadores_listos;
	t_queue *cola_entrenadores_bloqueados;
	t_dictionary *lista_entrenadores_finalizados;
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
void planificador_push_entrenador_a_bloqueado(t_entrenador *entrenador);
void planificador_push_entrenador_a_listo(t_entrenador *entrenador);
t_entrenador* planificador_pop_entrenador_bloqueado();
t_entrenador* planificador_pop_entrenador_listo();

/*-------------------------------------------FUNCIONES GENERALES--------------------------------------------------------*/
void planificador_dale_coordenadas_a_entrenador(t_entrenador *entrenador);
void planificador_entrenador_se_mueve(t_entrenador *entrenador);
void planificador_entrenador_quiere_capturar_pokemon(t_entrenador *entrenador);
void planificador_trata_captura_pokemon(t_entrenador *entrenador);
void planificador_bloquea_entrenador(t_entrenador *entrenador);
void planificador_desbloqueame_a(t_entrenador *entrenador);
void cola_bloqueados_quita_entrenador_especifico(t_queue *cola, int id_proceso);
void planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores();
void planificador_desbloquea_entrenador_si_es_posible(int cantidad_bloqueados);

/*---------------------------------------FINALIZADO---------------------------------------------------------*/
void planificador_finaliza_entrenador(t_entrenador *entrenador);
void planificador_espera_que_entrenador_se_desconecte(t_entrenador *entrenador);
void planificador_extraele_pokemones_a_entrenador(t_entrenador *entrenador);

/*---------------------------------------NUEVO->LISTO---------------------------------------------------------*/
void planificador_inicia_encolacion_nuevos_entrenadores();
void* planificador_encola_nuevos_entrenadores();
void planificador_modela_nuevo_entrenador_y_encolalo(void *id_proceso);
void foreach(void *lista,void(*funcion_de_lista)(void*));

/*---------------------------------------EXECUTE->LISTO---------------------------------------------------------*/
void planificador_volve_a_encolar_a_listo_si_es_necesario(t_entrenador *entrenador);

#endif /* MAPA_MAPA_COMMONS_PLANIFICADOR_H_ */
