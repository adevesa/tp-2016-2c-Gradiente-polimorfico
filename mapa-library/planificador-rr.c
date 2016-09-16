/*
 * planificador-rr.c
 *
 *  Created on: 16/9/2016
 *      Author: utnso
 */
#include "planificador-rr.h"

t_planificador_rr *planificador;

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg)
{
	planificador = planificador_rr_create();
	while(1)
	{
		planificador_rr_ordena();
	}
	pthread_exit(NULL);
}

/*--------------------------------------------ROUND ROBBIN----------------------------------------------------------*/
void quamtum_disminuite(int *q)
{
	*q = *q -1;
}

int quamtum_se_termino(int q)
{
	if(q == 0) {return 0;}
	else {return 1;}
}

void planificador_rr_organiza_entrenadores()
{
	while(1)
	{
		t_entrenador *entrenador_listo = planificador_rr_pop_entrenador_listo(planificador);
		int quamtum_restante = planificador->quantum;
		planificador_rr_es_el_turno_de(entrenador_listo, &quamtum_restante);
		planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores();
	}
}

void planificador_rr_es_el_turno_de(t_entrenador *entrenador_listo, int *quamtum)
{
	while(!quamtum_se_termino(*quamtum))
	{
		if(mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(entrenador_listo))
		{
			planificador_volve_a_bloquear_a_entrenador_si_es_necesario(entrenador_listo, quamtum);
		}
		else
		{
			planificador_rr_dale_nuevo_turno_a_entrenador(entrenador_listo,quamtum);
		}
	}
}

void planificador_rr_dale_nuevo_turno_a_entrenador(t_entrenador *entrenador_listo, int *quamtum_restante)
{
	otorgar_turno_a_entrenador(entrenador_listo);
	char * mensaje_del_entrenador = escuchar_al_entrenador(entrenador_listo);
	switch(tratar_respuesta(mensaje_del_entrenador,entrenador_listo))
	{
		case (ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST):
			{
				planificador_dale_coordenadas_a_entrenador(entrenador_listo);
				quamtum_disminuite(quamtum_restante);
			} break;
		case(ENTRENADOR_QUIERE_MOVERSE):
			{
				planificador_entrenador_se_mueve(entrenador_listo);
				quamtum_disminuite(quamtum_restante);
			} break;
		case(ENTRENADOR_QUIERE_CAPTURAR_POKEMON):
			{
				escuchar_captura_pokemon(entrenador_listo->socket_entrenador);
				planificador_bloquea_entrenador(entrenador_listo);
				planificador_trata_captura_pokemon(entrenador_listo);
				quamtum_restante--;
			} break;
		case(ENTRENADOR_FINALIZO_OBJETIVOS):
		{
			planificador_finaliza_entrenador(entrenador_listo);
			quamtum_restante = 0;
		} break;
		default: perror("No se puede interpretar lo que quiere el");
	}
}

/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/













void planificador_trata_captura_pokemon(t_entrenador *entrenador)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		//AVISAR POR ARCHIVO LOG TODA LA HISTORIA
		planificador_desbloqueame_a(entrenador);
		mapa_cambiale_estado_a_entrenador(entrenador, LISTO, BLOQUEADO);
		planificador_rr_push_entrenador_a_listo(entrenador);
		dar_pokemon_a_entrenador(entrenador, mapa_dame_pokemon_de_pokenest(entrenador->pokenest_objetivo));
		mapa_actualiza_pokemones_disponibles_de_pokenest(entrenador->pokenest_objetivo);
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador, BLOQUEADO, LISTO);
	}
}

void planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores()
{
	int cantidad_bloqueados = queue_size(planificador->listas_y_colas->cola_entrenadores_bloqueados);
	if(cantidad_bloqueados != 0)
	{	planificador_desbloquea_entrenador_si_es_posible(cantidad_bloqueados);
	}
	else {}
}

void planificador_desbloquea_entrenador_si_es_posible(int cantidad_bloqueados)
{
	int i;
	for(i=0; i<cantidad_bloqueados; i++)
	{
		t_entrenador *entrenador =  planificador_rr_pop_entrenador_bloqueado();
		if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
		{
			mapa_cambiale_estado_a_entrenador(entrenador,LISTO, BLOQUEADO);
			planificador_rr_push_entrenador_a_listo(entrenador);
		}
		else
		{
			planificador_rr_push_entrenador_a_bloqueado(entrenador);
		}
	}
}

void planificador_volve_a_bloquear_a_entrenador_si_es_necesario(t_entrenador *entrenador, int *quamtum)
{
	if(mapa_decime_si_hay_pokemones_en_pokenestt(entrenador->pokenest_objetivo))
	{
		avisar_desbloqueo_a_entrenador(entrenador->socket_entrenador);
		dar_pokemon_a_entrenador(entrenador, mapa_dame_pokemon_de_pokenest(entrenador->pokenest_objetivo));
		*quamtum = *quamtum -1;
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador,BLOQUEADO,LISTO);
		planificador_rr_push_entrenador_a_bloqueado(entrenador);
	}

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
		foreach(planificador->listas_y_colas->lista_entrenadores_a_planificar,planificador_modela_nuevo_entrenador_y_encolalo);
		list_clean(mapa->entrenadores->lista_entrenadores_a_planificar);
	}
}

void planificador_modela_nuevo_entrenador_y_encolalo(void *entrenador)
{
	t_entrenador_nuevo *entrenador_a_modelar = (t_entrenador_nuevo *) entrenador;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_entrenador);
	planificador_rr_push_entrenador_a_listo(new_entrenador);
}

void foreach(void *lista,void(*funcion_de_lista)(void*))
{
	t_list *lista_a_planificar = (t_list*)lista;
	int tamanio = list_size(lista);
	int i;
	for(i=0; i<tamanio;i++)
	{
		funcion_de_lista(list_get(lista_a_planificar, i));
	}
}


/*---------------------------------------BLOQUEADO -> EXECUTE---------------------------------------------------------*/
void planificador_desbloqueame_a(t_entrenador *entrenador)
{
	avisar_desbloqueo_a_entrenador(entrenador->socket_entrenador);
	cola_bloqueados_quita_entrenador_especifico(planificador->listas_y_colas->cola_entrenadores_bloqueados,entrenador->id_proceso);

}

void cola_bloqueados_quita_entrenador_especifico(t_queue *cola, int id_proceso)
{
	t_queue *aux_entrenador = queue_create();
	t_queue *aux = queue_create();
	int tamanio_de_cola = queue_size(cola);
	int i;
	for(i=0; i<tamanio_de_cola; i++)
	{
		t_entrenador *entrenador =queue_pop(cola);
		if(entrenador->id_proceso == id_proceso)
		{
			queue_push(aux_entrenador,entrenador);
		}
		else {queue_push(aux,entrenador);}
	}
	int u;
	for(u=0; u<tamanio_de_cola-1; u++)
	{
		queue_push(cola, queue_pop(aux));
	}
	queue_destroy(aux_entrenador);
	queue_destroy(aux);
}

/*---------------------------------------EXECUTE->FINALIZADO---------------------------------------------------------*/
void planificador_finaliza_entrenador(t_entrenador *entrenador)
{
	mapa_cambiale_estado_a_entrenador(entrenador, MUERTO, LISTO);
	entrenador->objetivo_cumplido = 1;
	char *ruta_medalla_del_mapa = buscar_medalla_del_mapa();
	otorgar_ruta_medalla_a_entrenador(entrenador->socket_entrenador, ruta_medalla_del_mapa);
	planificador_espera_que_entrenador_se_desconecte(entrenador);
}

void planificador_espera_que_entrenador_se_desconecte(t_entrenador *entrenador)
{
	int payloadSize = 3;
	char * payload = malloc(payloadSize);
	int resultado = recv(entrenador->socket_entrenador, payload, payloadSize,0);
	if(resultado == DESCONECTADO)
	{
		server_pthread_cerra_cliente(entrenador->socket_entrenador);
		list_add(planificador->listas_y_colas->lista_entrenadores_finalizados, entrenador);
		free(payload);
		planificador_extraele_pokemones_a_entrenador(entrenador);
	}
}

void planificador_extraele_pokemones_a_entrenador(t_entrenador *entrenador)
{
	int cantidad_pokemones_que_tenia = list_size(entrenador->pokemones_capturados);
	int i;
	for(i=0; i<cantidad_pokemones_que_tenia; i++)
	{
		queue_push(entrenador->pokenest_objetivo->pokemones, list_get(entrenador->pokemones_capturados,i));
	}
}

