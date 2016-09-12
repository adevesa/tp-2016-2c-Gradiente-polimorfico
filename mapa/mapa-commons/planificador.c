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

t_planificador_rr* planificador_rr_create()
{
	extern t_mapa *mapa;
	t_planificador_rr *planif_new = malloc(sizeof(t_planificador_rr));
	planif_new->planificador = planificador_create();
	planif_new->quantum = mapa->info_algoritmo->quamtum;
	planif_new->retardo = mapa->info_algoritmo->retardo;
	return planif_new;
}

t_planificador_srdf* planificador_srdf_create()
{
	t_planificador_srdf *planif_new = malloc(sizeof(t_planificador_srdf));
	planif_new->planificador=planificador_create();
	return planif_new;
}

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg)
{
	t_planificador_rr *planificador = planificador_rr_create();

	return NULL;
}

/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg)
{
	t_planificador_srdf *planificador = planificador_srdf_create();

	return NULL;
}


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones)
{
	list_add_all(lista_pokemones, entrenador->pokemones_capturados);
	list_destroy(entrenador->pokemones_capturados); //<-- debe usarse destroy&destroyElements
}

void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores)
{

}

void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*))
{
	t_list *lista_a_planificar = (t_list*)lista;
	t_queue *cola_listos = (t_queue*)cola;
	int tamanio = list_size(lista);
	int i;
	for(i=0; i<tamanio;i++)
	{
		funcion_de_lista(list_get(lista_a_planificar, i),cola_listos);
	}
}

void mapa_encola_nuevos_entrenadores(t_controllers *listas_y_colas)
{
	/*
	 * OJO! DEBE USARSE SEMAFORO PARA QUE NO OCURRA ERROR.
	 * UN NUEVO ENTRENADOR PORDRIA QUERER ACCEDER MIENTRAS MOVEMOS A LOS QUE YA ESTABAN
	 */

	if(!list_is_empty(listas_y_colas->lista_entrenadores_a_planificar))
	{
		foreach(listas_y_colas->lista_entrenadores_a_planificar, listas_y_colas->cola_entrenadores_listos,mapa_modela_nuevo_entrenador_y_encolalo);
		list_clean(listas_y_colas->lista_entrenadores_a_planificar);
	}
}

void mapa_modela_nuevo_entrenador_y_encolalo(void *entrenador,void*cola_listos)
{
	t_entrenador *entrenador_a_modelar = (t_entrenador *) entrenador;
	t_queue *cola = (t_queue*) cola_listos;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_etrenador);
	queue_push(cola,new_entrenador);
}

