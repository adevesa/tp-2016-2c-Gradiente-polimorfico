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
extern pthread_mutex_t mutex_manipular_cola_nuevos;
extern sem_t semaforo_hay_algun_entrenador_listo;


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
	int estado_anterior = entrenador->estado;
	mapa_cambiale_estado_a_entrenador(entrenador, BLOQUEADO, estado_anterior);
	pthread_mutex_unlock(&mutex_manipular_cola_bloqueados);


	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "PUSH (BLOQUEADO) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
}

void planificador_push_entrenador_a_listo(t_entrenador *entrenador)
{
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "PUSH (LISTO) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);

	pthread_mutex_lock(&mutex_manipular_cola_listos);
	queue_push(mapa->entrenadores->cola_entrenadores_listos, entrenador);
	int estado_anterior = entrenador->estado;
	mapa_cambiale_estado_a_entrenador(entrenador, LISTO, estado_anterior);
	pthread_mutex_unlock(&mutex_manipular_cola_listos);

}

t_entrenador* planificador_pop_entrenador_listo()
{
	pthread_mutex_lock(&mutex_manipular_cola_listos);
	t_entrenador *entrenador_que_tiene_el_turno = (t_entrenador*) queue_pop(mapa->entrenadores->cola_entrenadores_listos);
	pthread_mutex_unlock(&mutex_manipular_cola_listos);

	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "POP (LISTO) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador_que_tiene_el_turno->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);

	return entrenador_que_tiene_el_turno;
}

t_entrenador* planificador_pop_entrenador_bloqueado()
{
	pthread_mutex_lock(&mutex_manipular_cola_bloqueados);
	t_entrenador *entrenador = (t_entrenador *) queue_pop(mapa->entrenadores->cola_entrenadores_bloqueados);
	pthread_mutex_unlock(&mutex_manipular_cola_bloqueados);

	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "POP (BLOQUEADO) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);

	return entrenador;
}

/*-------------------------------------------FUNCIONES GENERALES--------------------------------------------------------*/
void planificador_dale_coordenadas_a_entrenador(t_entrenador *entrenador)
{
	char *identificador_pokenest = escuchar_mensaje_entrenador(entrenador, ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST);


	t_pokeNest *pokenest_buscada = mapa_buscame_pokenest(identificador_pokenest);
	entrenador->pokenest_objetivo = identificador_pokenest;
	char *coordendas_pokenest = armar_coordenada(pokenest_buscada->posicion->x,pokenest_buscada->posicion->y, MAX_BYTES_COORDENADA);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Entrenador identificado por ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	string_append(&mensaje_A_loggear, "está buscando coordenadas de pokenest ");
	string_append(&mensaje_A_loggear, identificador_pokenest);
	t_posicion *posicion = desarmar_coordenada(coordendas_pokenest);
	char *x = string_itoa(posicion->x);
	char *y = string_itoa(posicion->y);
	string_append(&mensaje_A_loggear, " ");
	string_append(&mensaje_A_loggear, "(");
	string_append(&mensaje_A_loggear,x);
	string_append(&mensaje_A_loggear,";");
	string_append(&mensaje_A_loggear,y);
	string_append(&mensaje_A_loggear, ")");

	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
	free(x);
	free(y);
	//FIN LOG

	enviar_mensaje_a_entrenador(entrenador, OTORGAR_COORDENADAS_POKENEST, coordendas_pokenest);

	//INICIO LOG
	char *mensaje = string_new();
	string_append(&mensaje, "La coordenada solicitada fue entregada a entrenador identificado por ");
	string_append(&mensaje, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje);
	free(mensaje);
	//FIN LOG
}

void planificador_entrenador_se_mueve(t_entrenador *entrenador)
{
	char *posicion_destino = escuchar_mensaje_entrenador(entrenador, ENTRENADOR_QUIERE_MOVERSE);
	entrenador->posicion_actual = desarmar_coordenada(posicion_destino);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Entrenador identificado por ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	string_append(&mensaje_A_loggear, " se movió a ");
	char *x = string_itoa(entrenador->posicion_actual->x);
	char *y = string_itoa(entrenador->posicion_actual->y);
	string_append(&mensaje_A_loggear,x);
	string_append(&mensaje_A_loggear,";");
	string_append(&mensaje_A_loggear,y);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
	free(x);
	free(y);
	//FIN LOG

	mapa_mostra_actualizacion_de_entrenador(entrenador);
	sleep(1);
}

void planificador_entrenador_quiere_capturar_pokemon(t_entrenador *entrenador)
{
	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Entrenador identificado por ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	string_append(&mensaje_A_loggear, " está solicitando captura pokemon");
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG

	planificador_bloquea_entrenador(entrenador);
	planificador_trata_captura_pokemon(entrenador);
}

void planificador_trata_captura_pokemon(t_entrenador *entrenador)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		planificador_desbloqueame_a(entrenador);
		enviar_mensaje_a_entrenador(entrenador,OTORGAR_POKEMON,mapa_dame_pokemon_de_pokenest(entrenador->pokenest_objetivo));

		//INICIO LOG
		char *mensaje_A_loggear = string_new();
		string_append(&mensaje_A_loggear, "Entrenador identificado por ");
		string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
		string_append(&mensaje_A_loggear, " recibio RUTA POKEMON");
		log_info(informe_planificador, mensaje_A_loggear);
		free(mensaje_A_loggear);
		//FIN LOG

		mapa_actualiza_pokemones_disponibles_de_pokenest(entrenador->pokenest_objetivo);
	}
	else
	{
		//INICIO LOG
		char *mensaje_A_loggear = string_new();
		string_append(&mensaje_A_loggear, "Entrenador identificado por ");
		string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
		string_append(&mensaje_A_loggear, " quedó bloqueado por no haber pokemons disponibles");
		log_info(informe_planificador, mensaje_A_loggear);
		free(mensaje_A_loggear);
		//FIN LOG

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

	pthread_mutex_lock(&mutex_manipular_cola_bloqueados);
	cola_bloqueados_quita_entrenador_especifico(mapa->entrenadores->cola_entrenadores_bloqueados,entrenador->id_proceso);
	pthread_mutex_unlock(&mutex_manipular_cola_bloqueados);
	mapa_cambiale_estado_a_entrenador(entrenador, EXECUTE, BLOQUEADO);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "POP (BLOQUEADO) entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG
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
		//INICIO LOG
		char *mensaje_A_loggear = string_new();
		string_append(&mensaje_A_loggear, "Se revisa si se puede desbloquear algún entrenador ");
		log_info(informe_planificador, mensaje_A_loggear);
		free(mensaje_A_loggear);
		//FIN LOG

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
	mapa_cambiale_estado_a_entrenador(entrenador, MUERTO, EXECUTE);
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
		server_cerra_cliente(entrenador->socket_entrenador);
		//sem_post(entrenador->semaforo_finalizacon);
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
void planificador_inicia_encolacion_nuevos_entrenadores()
{
	log_info(informe_planificador, "Se crea proceso para encolación de entreanadores");
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread,&attr, planificador_encola_nuevos_entrenadores,NULL);

	pthread_attr_destroy(&attr);
}

void* planificador_encola_nuevos_entrenadores()
{
	extern t_mapa *mapa;
	while(1)
	{
		sem_wait(&semaforo_hay_algun_entrenador_listo);
		pthread_mutex_lock(&mutex_manipular_cola_nuevos);

		log_info(informe_planificador, "Iniciando la encolación de nuevos entrenadores");
		foreach(mapa->entrenadores->lista_entrenadores_a_planificar,planificador_modela_nuevo_entrenador_y_encolalo);
		sem_post(&semaforo_entrenadores_listos);

		list_clean(mapa->entrenadores->lista_entrenadores_a_planificar);
		pthread_mutex_unlock(&mutex_manipular_cola_nuevos);

		log_info(informe_planificador, "Cola de nuevos vaciada por proceso de encolación");
		log_info(informe_planificador, "Finalizando la  encolación de entrenadores");



	}

}

void planificador_modela_nuevo_entrenador_y_encolalo(void *entrenador)
{
	t_entrenador_nuevo *entrenador_a_modelar = (t_entrenador_nuevo *) entrenador;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_entrenador);
	new_entrenador->semaforo_finalizacon=entrenador_a_modelar->semaforo_finalizacion;
	new_entrenador->simbolo_identificador = entrenador_a_modelar->simbolo_identificador;
	mapa_mostra_nuevo_entrenador_en_pantalla(new_entrenador);
	planificador_push_entrenador_a_listo(new_entrenador);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Entrenador conectado por socket ");
	string_append(&mensaje_A_loggear, string_itoa(new_entrenador->socket_entrenador));
	string_append(&mensaje_A_loggear, " modelado");
	log_info(informe_planificador, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG


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

/*---------------------------------------EXECUTE->LISTO---------------------------------------------------------*/
void planificador_volve_a_encolar_a_listo_si_es_necesario(t_entrenador *entrenador)
{
	if(mapa_decime_si_entrenador_esta_bloqueado(entrenador))
	{

	}
	else
	{
		planificador_push_entrenador_a_listo(entrenador);
	}
}
