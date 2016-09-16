/*
 * planificador.c
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */
#include "planificador.h"

/*--------------------------------------------CREATES---------------------------------------------------------------*/

t_listas_y_colas* listas_y_colas_creense()
{
	extern t_mapa *mapa;
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

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg)
{
	t_planificador_rr *planificador = planificador_rr_create();
	planificador_rr_ordena(planificador);

	return NULL;
}

/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg)
{
	t_planificador_srdf *planificador = planificador_srdf_create();

	return NULL;
}


/*--------------------------------------------ROUND ROBBIN----------------------------------------------------------*/
void planificador_rr_ordena(t_planificador_rr *planificador)
{
	while(1)
	{
		t_entrenador *entrenador_listo = planificador_rr_pop_entrenador_listo(planificador);
		int quamtum_restante = planificador->quantum;
		while(!quamtum_se_termino(quamtum_restante))
			{
				otorgar_turno_a_entrenador(entrenador_listo);
				char * mensaje_del_entrenador = escuchar_al_entrenador(entrenador_listo);
				switch(tratar_respuesta(mensaje_del_entrenador,entrenador_listo))
				{
					case (ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST):
						{
							darle_coordenadas_pokenst_a_entrenador(entrenador_listo);
							quamtum_restante--;
						} break;
					case(ENTRENADOR_QUIERE_MOVERSE):
						{
							escuchar_a_que_direccion_se_mueve_entrenador(entrenador_listo);
							//GRAFICAR POISICON NUEVA!
							quamtum_restante--;
						} break;
					case(ENTRENADOR_QUIERE_CAPTURAR_POKEMON):
						{
							escuchar_captura_pokemon(entrenador_listo->socket_entrenador);
							planificador_trata_captura_pokemon(planificador,entrenador_listo);
							quamtum_restante--;
						} break;
					case(ENTRENADOR_FINALIZO_OBJETIVOS):
						{
							planificador_finaliza_entrenador(planificador, entrenador_listo);
							quamtum_restante = 0;
						} break;
					default: perror("No se puede interpretar lo que quiere el");
				}
			}
	}
}


int quamtum_se_termino(int q)
{
	if(q == 0) {return 0;}
	else {return 1;}

}

t_entrenador* planificador_rr_pop_entrenador_listo(t_planificador_rr *planificador)
{
	t_entrenador *entrenador_que_tiene_el_turno = (t_entrenador*) queue_pop(planificador->listas_y_colas->cola_entrenadores_listos);
	return entrenador_que_tiene_el_turno;
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

void planificador_trata_captura_pokemon(t_planificador_rr *planificador, t_entrenador *entrenador)
{
	planificador_bloquea_entrenador(entrenador, planificador);
	if(hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		//AVISAR POR ARCHIVO LOG TODA LA HISTORIA
		planificador_desbloquea_entrenador(planificador);
		dar_pokemon_a_entrenador(entrenador, pokenest_dame_pokemon(entrenador->pokenest_objetivo));
	}
	else
	{
		planificador_bloquea_definitivamente_entrenador(entrenador);
	}
}

void planificador_bloquea_definitivamente_entrenador(t_entrenador *entrenador)
{
	entrenador->estado = 0;
}

/*---------------------------------------NUEVO->LISTO---------------------------------------------------------*/
void planificador_encola_nuevos_entrenadores()
{
	/*
	 * OJO! DEBE USARSE SEMAFORO PARA QUE NO OCURRA ERROR.
	 * UN NUEVO ENTRENADOR PORDRIA QUERER ACCEDER MIENTRAS MOVEMOS A LOS QUE YA ESTABAN
	 */
	extern t_mapa *mapa;
	if(!list_is_empty(mapa->entrenadores->lista_entrenadores_a_planificar))
	{
		foreach(mapa->entrenadores->lista_entrenadores_a_planificar, mapa->entrenadores->cola_entrenadores_listos,planificador_modela_nuevo_entrenador_y_encolalo);
		list_clean(mapa->entrenadores->lista_entrenadores_a_planificar);
	}
}

void planificador_modela_nuevo_entrenador_y_encolalo(void *entrenador,void*cola_listos)
{
	t_entrenador_nuevo *entrenador_a_modelar = (t_entrenador_nuevo *) entrenador;
	t_queue *cola = (t_queue*) cola_listos;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_entrenador);
	queue_push(cola,new_entrenador);
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

/*---------------------------------------EXECUTE->BLOQUEADO---------------------------------------------------------*/
void planificador_bloquea_entrenador(t_entrenador *entrenador, t_planificador_rr *planificador)
{
	avisar_bloqueo_a_entrenador(entrenador->socket_entrenador);
	queue_push(planificador->listas_y_colas->cola_entrenadores_bloqueados, entrenador);
}

/*---------------------------------------BLOQUEADO -> EXECUTE---------------------------------------------------------*/
void planificador_desbloquea_entrenador(t_planificador_rr *planificador)
{
	queue_pop(planificador->listas_y_colas->cola_entrenadores_bloqueados);

}

/*---------------------------------------EXECUTE->LISTO---------------------------------------------------------*/
void planificador_pone_a_listo_entrenador(t_planificador_rr *planificador, t_entrenador *entrenador)
{
	queue_push(planificador->listas_y_colas->cola_entrenadores_listos, entrenador);
}

/*---------------------------------------EXECUTE->FINALIZADO---------------------------------------------------------*/
void planificador_finaliza_entrenador(t_planificador_rr *planificador, t_entrenador *entrenador)
{
	entrenador->estado = -1;
	entrenador->objetivo_cumplido = 1;
	list_add(planificador->listas_y_colas->lista_entrenadores_finalizados, entrenador);
}
