/*
 * planificador.c
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */
#include "planificador.h"

extern t_mapa *mapa;
/*--------------------------------------------CREATES---------------------------------------------------------------*/

t_listas_y_colas* listas_y_colas_creense()
{
	t_listas_y_colas *planificador_new = malloc(sizeof(t_listas_y_colas));
	planificador_new->cola_entrenadores_bloqueados=mapa->entrenadores->cola_entrenadores_bloqueados;
	planificador_new->cola_entrenadores_listos=mapa->entrenadores->cola_entrenadores_listos;
	planificador_new->lista_entrenadores_a_planificar=mapa->entrenadores->lista_entrenadores_a_planificar;
	planificador_new->lista_entrenadores_finalizados=mapa->entrenadores->lista_entrenadores_finalizados;
	return planificador_new;
}

t_planificador_rr* planificador_rr_create()
{
	extern t_mapa *mapa;
	t_planificador_rr *planif_new = malloc(sizeof(t_planificador_rr));
	planif_new->listas_y_colas = listas_y_colas_creense();
	planif_new->quantum = mapa->info_algoritmo->quamtum;
	planif_new->retardo = mapa->info_algoritmo->retardo;
	return planif_new;
}

t_planificador_srdf* planificador_srdf_create()
{
	t_planificador_srdf *planif_new = malloc(sizeof(t_planificador_srdf));
	planif_new->listas_y_colas = listas_y_colas_creense();
	return planif_new;
}


/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg)
{
	t_planificador_srdf *planificador = planificador_srdf_create();

	return NULL;
}

/*---------------------------------------PUSH Y POPS DE COLAS---------------------------------------------------------*/
void planificador_rr_push_entrenador_a_bloqueado(t_entrenador *entrenador)
{
	queue_push(mapa->entrenadores->cola_entrenadores_bloqueados,entrenador);
}

void planificador_rr_push_entrenador_a_listo(t_entrenador *entrenador)
{
	queue_push(mapa->entrenadores->cola_entrenadores_listos, entrenador);
}

t_entrenador* planificador_rr_pop_entrenador_listo()
{
	t_entrenador *entrenador_que_tiene_el_turno = (t_entrenador*) queue_pop(mapa->entrenadores->cola_entrenadores_listos);
	return entrenador_que_tiene_el_turno;
}

t_entrenador* planificador_rr_pop_entrenador_bloqueado()
{
	return (queue_pop(mapa->entrenadores->cola_entrenadores_bloqueados));
}

/*-------------------------------------------FUNCIONES GENERALES--------------------------------------------------------*/
void planificador_dale_coordenadas_a_entrenador(t_entrenador *entrenador)
{
	darle_coordenadas_pokenst_a_entrenador(entrenador);
}

void planificador_entrenador_se_mueve(t_entrenador *entrenador)
{
	escuchar_a_que_direccion_se_mueve_entrenador(entrenador);
	//graficar entrenador
}

void planificador_entrenador_quiere_capturar_pokemon(t_entrenador *entrenador)
{
	escuchar_captura_pokemon(entrenador->socket_entrenador);
	planificador_bloquea_entrenador(entrenador);
}

void planificador_bloquea_entrenador(t_entrenador *entrenador)
{
	avisar_bloqueo_a_entrenador(entrenador->socket_entrenador);
	planificador_rr_push_entrenador_a_bloqueado(entrenador);
}
