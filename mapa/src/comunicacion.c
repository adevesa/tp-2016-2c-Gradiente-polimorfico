/*
 * comunicacion.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "comunicacion.h"

void* ejecutar_servidor(void *argumento)
{
	t_info_socket *info_sock = (t_info_socket*) argumento;

	t_server *server_pthread = server_create(info_sock->puerto,info_sock->ip, BACKLOG);
	server_escucha(server_pthread);
	int se_puede_ejecutar = 1;

	while(se_puede_ejecutar > 0)
	{
		int conexion = server_acepta_conexion_cliente(server_pthread);
		if(conexion == SERVER_DESCONECTADO)
		{
			se_puede_ejecutar = 0;
		}
		else {conexion_create(&conexion);}
	}

	pthread_exit(NULL);
}

void conexion_create(int *conexion)
{
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread,&attr,atender_cliente,(void*)conexion);

	pthread_attr_destroy(&attr);

}

void* atender_cliente(void* argumento)
{
	int *conexion = (int*) argumento;
	sem_t semaforo_termina_proceso;
	sem_init(&semaforo_termina_proceso,0,0);
	agregar_proceso_a_lista(conexion,&semaforo_termina_proceso);
	pthread_exit(NULL);
}

void agregar_proceso_a_lista(int *socket_cliente, sem_t *semaforo_finalizacion)
{
	t_entrenador_nuevo *entrenador = malloc(sizeof(t_entrenador_nuevo));
	entrenador->id_proceso = (int)process_get_thread_id();
	entrenador->socket_entrenador = *socket_cliente;
	entrenador->simbolo_identificador = recibir_mensaje(*socket_cliente,1);
	entrenador->semaforo_finalizacion = semaforo_finalizacion;

	pthread_mutex_lock(&mutex_manipular_cola_nuevos);
	list_add(mapa->entrenadores->lista_entrenadores_a_planificar,entrenador);
	pthread_mutex_unlock(&mutex_manipular_cola_nuevos);

	//INICIO LOG
		char *mensaje_A_loggear = string_new();
		string_append(&mensaje_A_loggear, "PUSH (NUEVO) entrenador identificado con el simbolo ");
		string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
		string_append(&mensaje_A_loggear, " y por el socket ");
		string_append(&mensaje_A_loggear,string_itoa(entrenador->socket_entrenador));
		log_info(informe_planificador, mensaje_A_loggear);
		free(mensaje_A_loggear);
	//FIN LOG

	sem_post(&semaforo_hay_algun_entrenador_listo);


}
/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
int tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador)
{
	if(string_equals_ignore_case(respuesta_del_entrenador, "up;"))
	{
		return ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador,"mp;"))
	{
		return ENTRENADOR_QUIERE_MOVERSE;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "cp;"))
	{
		return ENTRENADOR_QUIERE_CAPTURAR_POKEMON;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "fp;"))
	{
		return ENTRENADOR_FINALIZO_OBJETIVOS;
	}
	else {return 0;}
}

void enviar_mensaje_a_entrenador(t_entrenador *entrenador, int header, char *payload)
{
	switch(header)
	{
		case(OTORGAR_TURNO):enviar_mensaje(entrenador->socket_entrenador, "tr;"); break;
		case(OTORGAR_COORDENADAS_POKENEST): enviar_mensaje(entrenador->socket_entrenador, armar_mensaje("ur",payload,MAX_BYTES_TOTAL_A_ENVIAR)); break;
		case(OTORGAR_MEDALLA_DEL_MAPA): otorgar_ruta_medalla_a_entrenador(entrenador->socket_entrenador, mapa_dame_medalla()); break;
		case(OTORGAR_POKEMON): dar_pokemon_a_entrenador(entrenador, payload);break;
		case(AVISAR_BLOQUEO_A_ENTRENADOR): enviar_mensaje(entrenador->socket_entrenador, "bq;");  break;
		case(AVISAR_DESBLOQUEO_A_ENTRENADOR): enviar_mensaje(entrenador->socket_entrenador,"fb;"); break;
		default: ;
	}
}

char* escuchar_mensaje_entrenador(t_entrenador *entrenador, int header)
{
	switch(header)
	{
		case(SOLICITUD_DEL_ENTRENADOR): return(recibir_mensaje(entrenador->socket_entrenador,3));
		case(ENTRENADOR_OTORGA_SU_SIMBOLO): return(recibir_mensaje_especifico(entrenador->socket_entrenador)); break;
		case(ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST): return(recibir_mensaje_especifico(entrenador->socket_entrenador)); break;
		case(ENTRENADOR_QUIERE_MOVERSE): return(recibir_mensaje_especifico(entrenador->socket_entrenador)); break;
		default: return("0"); break;
	}
}

void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla)
{
	char *mensaje = armar_mensaje("mr", rutaMedalla,MAX_BYTES_TOTAL_A_ENVIAR);
	enviar_mensaje(entrenador, mensaje);
	free(mensaje);
}

void dar_pokemon_a_entrenador(t_entrenador *entrenador, char *ruta_pokemon)
{
	char *mensaje = armar_mensaje("sr",ruta_pokemon, MAX_BYTES_TOTAL_A_ENVIAR);
	enviar_mensaje(entrenador->socket_entrenador, mensaje);
	free(mensaje);
}

t_posicion* desarmar_coordenada(char *coordenada)
{
	char **por_separado = string_split(coordenada, ";");
	string_trim_left(&por_separado[0]);
	string_trim_left(&por_separado[1]);
	return (posicion_create(atoi(por_separado[0]),atoi(por_separado[1])));

}


