/*
 * comunication-point.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "comunication-point.h"
extern t_mapa *mapa;
extern t_mapa *mapa;
extern sem_t semaforo_hay_algun_entrenador_listo;
extern pthread_mutex_t mutex_manipular_cola_nuevos;
int se_termino_la_conexion = 0;
extern t_log *informe_cola_nuevos;

/*--------------------------------------------EXECUTE-----------------------------------------------------*/
void* ejecutar_servidor(void *argumento)
{
	t_info_socket *info_sock = (t_info_socket*) argumento;
	//ejecutar_hilo_socket(info_sock->puerto, info_sock->ip);

	t_server_pthread *server_pthread = server_pthread_create(info_sock->puerto,info_sock->ip);
	server_pthread_escucha(server_pthread);
	int se_puede_ejecutar = 1;

	while(se_puede_ejecutar > 0)
	{
		int conexion = server_pthread_acepta_conexion_cliente(server_pthread);
		if(conexion == SERVER_DESCONECTADO)
		{
			se_puede_ejecutar = 0;
		}
		else {pthread_conexion_create(&conexion);}
	}

	pthread_exit(NULL);
}

/*--------------------------------------------CREATES-----------------------------------------------------*/

t_server_pthread* server_pthread_create(int puerto, char *ip)
{
		t_server_pthread *new_server = malloc(sizeof(t_server_pthread));

		//Configuro direccion de servidor//
		address_config_in *direccionServidor =configurar_address(puerto, ip);

		//Creo el socket para el server//
		int server_socket = socket(AF_INET,SOCK_STREAM,0);

		//Activo reutilización de direcciones//
		reutilizar_direccion(1,server_socket);

		//Asocio socket al puerto por donde escuchará//
		server_pthread_asociate_a_puerto(server_socket, direccionServidor);

		//Modelo al nuevo servidor//
		new_server->socket_asociado = server_socket;
		new_server->backlog = 1500;
		return new_server;
}

void pthread_conexion_create(int *conexion)
{
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread,&attr,server_pthread_atender_cliente,(void*)conexion);

	pthread_attr_destroy(&attr);

}

t_arg_pthread* pthread_arg_create(int *conexion, t_list *lista_nuevos_entrenadores)
{
	t_arg_pthread *new_arg = malloc(sizeof(t_arg_pthread));
	new_arg->conexion = conexion;
	new_arg->lista_nuevos_entrenadores = lista_nuevos_entrenadores;
	return new_arg;
}

/*--------------------------------------------PINCIPALES-----------------------------------------------------*/

int server_pthread_acepta_conexion_cliente(t_server_pthread *server)
{
	address_config_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(address_config_in);
	int cliente = accept(server->socket_asociado,(void*) &direccionCliente, &tamanioDireccion);
	if(cliente <0)
	{
		return 0;
	}
	return cliente;
}


void* server_pthread_atender_cliente(void* argumento)
{
	int *conexion = (int*) argumento;
	sem_t semaforo_termina_proceso;
	sem_init(&semaforo_termina_proceso,0,0);
	server_pthread_agrega_proceso_a_lista(conexion,&semaforo_termina_proceso);
	sem_wait(&semaforo_termina_proceso);
	pthread_exit(NULL);
}


void server_pthread_agrega_proceso_a_lista(int *socket_cliente, sem_t *semaforo_finalizacion)
{
	t_entrenador_nuevo *entrenador = malloc(sizeof(t_entrenador_nuevo));
	entrenador->id_proceso = (int)process_get_thread_id();
	entrenador->socket_entrenador = *socket_cliente;
	entrenador->simbolo_identificador = recibir_mensaje(*socket_cliente,1);
	entrenador->semaforo_finalizacion = semaforo_finalizacion;

	pthread_mutex_lock(&mutex_manipular_cola_nuevos);
	list_add(mapa->entrenadores->lista_entrenadores_a_planificar,entrenador);
	pthread_mutex_unlock(&mutex_manipular_cola_nuevos);

	sem_post(&semaforo_hay_algun_entrenador_listo);

	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Nuevo entrenador identificador con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	string_append(&mensaje_A_loggear, " y por el socket ");
	string_append(&mensaje_A_loggear,string_itoa(entrenador->socket_entrenador));
	log_info(informe_cola_nuevos, mensaje_A_loggear);
	free(mensaje_A_loggear);
}







/*-------------------------------------ENVIO DE MENSAJES A ENTRENADORES--------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje)
{
	send(socket, mensaje, strlen(mensaje),0);
}

/*-------------------------------------RECEPCION DE MENSAJES DE ENTRENADORES--------------------------------------------*/
char* recibir_mensaje(int socket,int payloadSize)
{
	char * payload = malloc(payloadSize+1);
	int bytes_recibidos = 0;
	while(bytes_recibidos !=payloadSize)
	{
		bytes_recibidos=recv(socket, payload, payloadSize,0);
	}
	payload[payloadSize]= '\0';
	return payload;
}

char* recibir_mensaje_especifico(int socket)
{
	char * payload = malloc(5);
	int bytes_recibidos = 0;
	while(bytes_recibidos != 4)
	{
		bytes_recibidos=recv(socket, payload, 4,0);
	}

	payload[4]= '\0';
	char **solo_tamanio = string_split(payload, ";");

	string_trim_left(&solo_tamanio[0]);
	int tamanio_del_mensaje = atoi(solo_tamanio[0]);
	char *mensaje_final = recibir_mensaje(socket, tamanio_del_mensaje);
	return mensaje_final;

}

/*-------------------------------------------------------FUNCIONES SECUNDARIAS----------------------------------------------*/

address_config_in* configurar_address(int puerto, char *ip)
{
	address_config_in *direccionServidor = malloc(sizeof(address_config_in));
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port=htons(puerto);
	memset(&(direccionServidor->sin_zero), '\0', 8);
	return direccionServidor;

}


void reutilizar_direccion(int valorVerdaderoso, int socket)
{
	setsockopt(socket,SOL_SOCKET,SO_REUSEADDR, &valorVerdaderoso, sizeof(valorVerdaderoso));
}


int server_pthread_asociate_a_puerto(int server, address_config_in *address)
{
	int resultBind;
	resultBind = bind(server,(void*) address,sizeof(*address));

	if(resultBind !=0)
	{	perror("Falló el bind");
		return 1;
	}
	return resultBind;
}


void server_pthread_escucha(t_server_pthread *server)
{
	listen(server->socket_asociado,server->backlog);
}


void server_pthread_cerra_cliente(int cliente)
{
	close(cliente);
	se_termino_la_conexion = 1;
}




