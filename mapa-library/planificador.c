/*
 * planificador.c
 *
 *  Created on: 11/9/2016
 *      Author: utnso
 */
#include "planificador.h"

extern t_mapa *mapa;
extern sem_t semaforo_entrenadores_listos;
extern sem_t semaforo_cola_bloqueados;
extern pthread_mutex_t mutex_manipular_cola_listos;
extern pthread_mutex_t mutex_manipular_cola_bloqueados;


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
void planificador_push_entrenador_a_bloqueado(t_entrenador *entrenador)
{
	pthread_mutex_lock(&mutex_manipular_cola_bloqueados);
	queue_push(mapa->entrenadores->cola_entrenadores_bloqueados,entrenador);
	pthread_mutex_unlock(&mutex_manipular_cola_bloqueados);
}

void planificador_push_entrenador_a_listo(t_entrenador *entrenador)
{
	pthread_mutex_lock(&mutex_manipular_cola_listos);
	queue_push(mapa->entrenadores->cola_entrenadores_listos, entrenador);
	pthread_mutex_unlock(&mutex_manipular_cola_listos);
}

t_entrenador* planificador_pop_entrenador_listo()
{
	pthread_mutex_lock(&mutex_manipular_cola_listos);
	t_entrenador *entrenador_que_tiene_el_turno = (t_entrenador*) queue_pop(mapa->entrenadores->cola_entrenadores_listos);
	pthread_mutex_unlock(&mutex_manipular_cola_listos);
	return entrenador_que_tiene_el_turno;
}

t_entrenador* planificador_pop_entrenador_bloqueado()
{
	pthread_mutex_lock(&mutex_manipular_cola_bloqueados);
	t_entrenador *entrenador = (t_entrenador *) queue_pop(mapa->entrenadores->cola_entrenadores_bloqueados);
	pthread_mutex_unlock(&mutex_manipular_cola_bloqueados);
	return entrenador;
}

/*-------------------------------------------FUNCIONES GENERALES--------------------------------------------------------*/
void planificador_dale_coordenadas_a_entrenador(t_entrenador *entrenador)
{
	char *identificador_pokenest = escuchar_mensaje_entrenador(entrenador, ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST);
	t_pokeNest *pokenest_buscada = mapa_buscame_pokenest(identificador_pokenest);
	entrenador->pokenest_objetivo = pokenest_buscada;
	char *coordendas_pokenest = armar_coordenada(entrenador->pokenest_objetivo->posicion->x,entrenador->pokenest_objetivo->posicion->y);
	enviar_mensaje_a_entrenador(entrenador, OTORGAR_COORDENADAS_POKENEST, coordendas_pokenest);
}

void planificador_entrenador_se_mueve(t_entrenador *entrenador)
{
	char *posicion_destino = escuchar_mensaje_entrenador(entrenador, ENTRENADOR_QUIERE_MOVERSE);
	entrenador->posicion_actual = desarmar_coordenada(posicion_destino);
	mapa_mostra_actualizacion_de_entrenador(entrenador);
}

void planificador_entrenador_quiere_capturar_pokemon(t_entrenador *entrenador)
{
	planificador_bloquea_entrenador(entrenador);
	planificador_trata_captura_pokemon(entrenador);
}

void planificador_trata_captura_pokemon(t_entrenador *entrenador)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		//AVISAR POR ARCHIVO LOG TODA LA HISTORIA
		planificador_desbloqueame_a(entrenador);
		mapa_cambiale_estado_a_entrenador(entrenador, LISTO, BLOQUEADO);
		planificador_push_entrenador_a_listo(entrenador);
		enviar_mensaje_a_entrenador(entrenador,OTORGAR_POKEMON,mapa_dame_pokemon_de_pokenest(entrenador->pokenest_objetivo)->ruta_en_pokedex);
		mapa_actualiza_pokemones_disponibles_de_pokenest(entrenador->pokenest_objetivo);
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador, BLOQUEADO, LISTO);
	}
}

void planificador_bloquea_entrenador(t_entrenador *entrenador)
{
	enviar_mensaje_a_entrenador(entrenador, AVISAR_BLOQUEO_A_ENTRENADOR, NULL);
	planificador_push_entrenador_a_bloqueado(entrenador);
}

void planificador_desbloqueame_a(t_entrenador *entrenador)
{
	enviar_mensaje_a_entrenador(entrenador, AVISAR_DESBLOQUEO_A_ENTRENADOR, NULL);
	sem_wait(&semaforo_cola_bloqueados);
	cola_bloqueados_quita_entrenador_especifico(mapa->entrenadores->cola_entrenadores_bloqueados,entrenador->id_proceso);
	sem_post(&semaforo_cola_bloqueados);
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

void planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores()
{
	int cantidad_bloqueados = queue_size(mapa->entrenadores->cola_entrenadores_bloqueados);
	if(cantidad_bloqueados != 0)
	{
		sem_wait(&semaforo_cola_bloqueados);
		planificador_desbloquea_entrenador_si_es_posible(cantidad_bloqueados);
		sem_post(&semaforo_cola_bloqueados);
	}
	else {}
}

void planificador_desbloquea_entrenador_si_es_posible(int cantidad_bloqueados)
{
	int i;
	for(i=0; i<cantidad_bloqueados; i++)
	{
		t_entrenador *entrenador =  planificador_pop_entrenador_bloqueado();
		if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
		{
			mapa_cambiale_estado_a_entrenador(entrenador,LISTO, BLOQUEADO);
			planificador_push_entrenador_a_listo(entrenador);
		}
		else
		{
			planificador_push_entrenador_a_bloqueado(entrenador);
		}
	}
}

/*---------------------------------------FINALIZADO---------------------------------------------------------*/
void planificador_finaliza_entrenador(t_entrenador *entrenador)
{
	mapa_cambiale_estado_a_entrenador(entrenador, MUERTO, LISTO);
	entrenador->objetivo_cumplido = 1;
	char *ruta_medalla_del_mapa = mapa_dame_medalla();
	enviar_mensaje_a_entrenador(entrenador, OTORGAR_MEDALLA_DEL_MAPA,ruta_medalla_del_mapa);
	planificador_espera_que_entrenador_se_desconecte(entrenador);
	mapa_borra_entrenador_de_pantalla(entrenador);
}

void planificador_espera_que_entrenador_se_desconecte(t_entrenador *entrenador)
{
	int payloadSize = 3;
	char * payload = malloc(payloadSize);
	int resultado = recv(entrenador->socket_entrenador, payload, payloadSize,0);
	if(resultado == DESCONECTADO)
	{
		server_pthread_cerra_cliente(entrenador->socket_entrenador);
		char *key = string_itoa(entrenador->socket_entrenador);
		dictionary_put(mapa->entrenadores->lista_entrenadores_finalizados,key, entrenador);
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
		mapa_devolve_pokemon_a_pokenest(list_get(entrenador->pokemones_capturados,i));
	}
}

/*---------------------------------------NUEVO->LISTO---------------------------------------------------------*/
void planificador_encola_nuevos_entrenadores()
{
	extern t_mapa *mapa;
	sem_wait(&semaforo_entrenadores_listos);
	if(!list_is_empty(mapa->entrenadores->lista_entrenadores_a_planificar))
	{
		pthread_mutex_lock(&mutex_manipular_cola_listos);
		foreach(mapa->entrenadores->lista_entrenadores_a_planificar,planificador_modela_nuevo_entrenador_y_encolalo);
		list_clean(mapa->entrenadores->lista_entrenadores_a_planificar);
		pthread_mutex_unlock(&mutex_manipular_cola_listos);
	}
}

void planificador_modela_nuevo_entrenador_y_encolalo(void *entrenador)
{
	t_entrenador_nuevo *entrenador_a_modelar = (t_entrenador_nuevo *) entrenador;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_entrenador);
	planificador_push_entrenador_a_listo(new_entrenador);
	mapa_mostra_nuevo_entrenador_en_pantalla(new_entrenador);
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
