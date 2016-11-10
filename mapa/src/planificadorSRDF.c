/*
 * planificadorSRDF.c
 *
 *  Created on: 30/9/2016
 *      Author: utnso
 */
#include "planificadorSRDF.h"

t_planificador_srdf *planificador;
extern sem_t semaforo_cola_entrenadores_sin_objetivos;
extern pthread_mutex_t mutex_cola_entrenadores_sin_objetivos;
pthread_mutex_t mutex_manipular_listos_y_odenados = PTHREAD_MUTEX_INITIALIZER;
sem_t semaforo_esperar_por_entrenador_listo;

void planificador_push_entrenador_en_cola_sin_objetivos(t_entrenador *entrenador)
{
	pthread_mutex_lock(&mutex_cola_entrenadores_sin_objetivos);
	queue_push(planificador->cola_entrenadores_sin_objetivo,entrenador);
	int estado_anterior = entrenador->estado;
	mapa_cambiale_estado_a_entrenador(entrenador,SIN_OBJETIVO, estado_anterior);
	pthread_mutex_unlock(&mutex_cola_entrenadores_sin_objetivos);

	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "PUSH (SIN OBJETIVOS) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
}

void planificador_push_entrenador_a_listo_ordenado(t_entrenador *entrenador)
{
	pthread_mutex_lock(&mutex_manipular_listos_y_odenados);
	list_add(planificador->listos_y_ordenados, entrenador);
	pthread_mutex_unlock(&mutex_manipular_listos_y_odenados);
}

int comparador(void *arg1, void *arg2)
{
	t_entrenador *entrenador_1= (t_entrenador*)arg1;
	t_entrenador *entrenador_2= (t_entrenador*)arg2;

	if(entrenador_1->distancia_hasta_objetivo <= entrenador_2->distancia_hasta_objetivo)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void planificador_srdf_ordena_cola_listos()
{
	list_sort(planificador->listos_y_ordenados,comparador);
	planificador_srdf_move_lista_a_cola();
	list_clean(planificador->listos_y_ordenados);
}

void planificador_srdf_move_lista_a_cola()
{
	int size = list_size(planificador->listos_y_ordenados);
	int i;
	for(i=0; i<size; i++)
	{
		t_entrenador  *entrenador = list_get(planificador->listos_y_ordenados,i);
		planificador_push_entrenador_a_listo(entrenador);
	}
}

/*-----------------------------------EXECUTE PLANIFICADOR SRDF--------------------------------------------------------*/
void* ejecutar_planificador_srdf(void* arg)
{
	planificador_inicia_log();
	sem_init(&semaforo_esperar_por_entrenador_listo,1,0);
	planificador = planificador_srdf_create();
	if(encolacion_entrenadores_iniciada == NO_INICIADO)
	{
		planificador_inicia_encolacion_nuevos_entrenadores();
	}
	planificador_srdf_organiza_entrenadores_sin_coordenadas();
	planificador_srdf_organiza_entrenadores();
	return NULL;
}

void planificador_srdf_organiza_entrenadores()
{
	while(mapa_decime_si_planificador_es(PLANIFICADOR_SRDF))
	{
		if(queue_is_empty(planificador->listas_y_colas->cola_entrenadores_listos))
		{
			sem_wait(&semaforo_esperar_por_entrenador_listo);
		}
		t_entrenador *entrenador_listo = planificador_pop_entrenador_listo(planificador);
		int estado_anterior = entrenador_listo->estado;
		mapa_cambiale_estado_a_entrenador(entrenador_listo, EXECUTE, estado_anterior);
		planificador_srdf_es_el_turno_de(entrenador_listo);
		planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores();
	}
}

void planificador_srdf_es_el_turno_de(t_entrenador *entrenador)
{
	if(mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(entrenador))
	{
		planificador_srdf_dale_pokemon_si_es_posible(entrenador);
	}
	else
	{
		planificador_srdf_dale_nuevo_turno_hasta_que_se_bloquee(entrenador);
	}
}

void planificador_srdf_dale_pokemon_si_es_posible(t_entrenador *entrenador)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		planificador_entrega_pokemon_a(entrenador);
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador,BLOQUEADO,LISTO);
		planificador_push_entrenador_a_bloqueado(entrenador);
	}
}

void planificador_srdf_dale_nuevo_turno_hasta_que_se_bloquee(t_entrenador *entrenador)
{
	while(!mapa_decime_si_entrenador_esta_bloqueado(entrenador) || !mapa_decime_si_entrenador_esta_abortado(entrenador))
	{
		usleep(planificador->retardo*1000);
		enviar_mensaje_a_entrenador(entrenador,OTORGAR_TURNO,NULL);
		char *mensaje_del_entrenador = escuchar_mensaje_entrenador(entrenador, SOLICITUD_DEL_ENTRENADOR);
		switch(tratar_respuesta(mensaje_del_entrenador,entrenador))
		{
			case(ENTRENADOR_QUIERE_MOVERSE): planificador_entrenador_se_mueve(entrenador);break;
			case(ENTRENADOR_QUIERE_CAPTURAR_POKEMON):
					{
						planificador_entrenador_quiere_capturar_pokemon(entrenador, BLOQUEAR_DE_TODOS_MODOS);
						//planificador_push_entrenador_en_cola_sin_objetivos(entrenador);
					}break;
			default: planificador_aborta_entrenador(entrenador);
		};
	}

}

/*--------------------------------------------HILOS--------------------------------------------------------*/
void planificador_srdf_organiza_entrenadores_sin_coordenadas()
{
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread,&attr, planificador_srdf_atende_a_entrenadores_sin_coordenadas,NULL);

	pthread_attr_destroy(&attr);
}

void* planificador_srdf_atende_a_entrenadores_sin_coordenadas()
{
	while(1)
	{
		if(queue_is_empty(planificador->cola_entrenadores_sin_objetivo))
		{
			sem_wait(&semaforo_cola_entrenadores_sin_objetivos);
		}
		planificador_srdf_dale_coordenadas_a_todos();
		if(!queue_is_empty(planificador->listas_y_colas->cola_entrenadores_listos))
		{
			sem_post(&semaforo_esperar_por_entrenador_listo);
		}
	}
	pthread_exit(NULL);
}

void planificador_srdf_dale_coordenadas_a_todos()
{
	int cantidad = queue_size(planificador->cola_entrenadores_sin_objetivo);
	int i;
	for(i=0; i<cantidad; i++)
	{
		t_entrenador *entrenador = (t_entrenador*) queue_pop(planificador->cola_entrenadores_sin_objetivo);
		char *mensaje_del_entrenador = escuchar_mensaje_entrenador(entrenador, SOLICITUD_DEL_ENTRENADOR);
		switch(tratar_respuesta(mensaje_del_entrenador, entrenador))
		{
			case(ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST):
					{
						planificador_dale_coordenadas_a_entrenador(entrenador);
						planificador_push_entrenador_a_listo_ordenado(entrenador);
					}break;
			default: planificador_aborta_entrenador(entrenador);
		}
	}
	if(!list_is_empty(planificador->listos_y_ordenados))
	{
		planificador_srdf_ordena_cola_listos();
	}
}


