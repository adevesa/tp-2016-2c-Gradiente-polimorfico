/*
 * comunication-point.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "comunication-point.h"

/*--------------------------------------------EXECUTE-----------------------------------------------------*/
void* ejecutar_servidor(void *argumento)
{
	t_info_socket *info_sock = (t_info_socket*) argumento;
	ejecutar_hilo_socket(info_sock->puerto, info_sock->ip);
}


void ejecutar_hilo_socket(int puerto, char *ip)
{

	t_server_pthread *server_pthread = server_pthread_create(puerto,ip);
	server_pthread_escucha(server_pthread);

	int se_puede_ejecutar = 1;
	while(se_puede_ejecutar)
	{
		int conexion = conexion_create(server_pthread);
		pthread_conexion_create(&conexion);
	}
}

int se_termino_la_conexion = 0;

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


int conexion_create(t_server_pthread *server)
{
	int cliente = server_pthread_acepta_conexion_cliente(server);
	return cliente;
}


void pthread_conexion_create(int *conexion)
{
	pthread_t thread;
	pthread_create(&thread,NULL,server_pthread_atender_cliente,(void*)conexion);

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
		perror("Error al intentar aceptar cliente");
		close(cliente);
		return 0;
	}
	return cliente;
}


void* server_pthread_atender_cliente(void* argumento)
{
	int *conexion = (int*) argumento;
	server_pthread_agrega_proceso_a_lista(conexion);
	while(!se_termino_la_conexion)
	{
	}
	pthread_exit(NULL);
}

/*-------------------------------------ENVIO DE MENSAJES A ENTRENADORES--------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje)
{
	send(socket, mensaje, strlen(mensaje)+1,0);
	free(mensaje);
}

/*-------------------------------------RECEPCION DE MENSAJES DE ENTRENADORES--------------------------------------------*/
char* recibir_mensaje(int socket,int payloadSize)
{
	char * payload = malloc(payloadSize);
	recv(socket, payload, payloadSize,0);
	payload[payloadSize-1]= '\0';
	string_trim(&payload);
	return payload;
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


void server_pthread_agrega_proceso_a_lista(int *socket_cliente)
{
	extern t_mapa *mapa;
	//DEBE USARSE SEMAFORO
	t_entrenador_nuevo *entrenador = malloc(sizeof(t_entrenador_nuevo));
	entrenador->id_proceso = (int)process_get_thread_id();
	entrenador->socket_entrenador = *socket_cliente;
	list_add(mapa->entrenadores->lista_entrenadores_a_planificar,entrenador);
}


