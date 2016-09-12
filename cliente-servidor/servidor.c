/*
 * servidor.c
 *
 *  Created on: 6/9/2016
 *      Author: utnso
 */

#include "servidor.h"

/*--------------------------------------------EXECUTE-----------------------------------------------------*/

void ejecutar_hilo_socket(int puerto, char *ip, t_list *nuevos_entrenadores)
{

	t_server_pthread *server_pthread = server_pthread_create(puerto,ip);
	server_pthread_escucha(server_pthread);

	int se_puede_ejecutar = 1;
	while(se_puede_ejecutar)
	{
		t_cliente_servidor *conexion = conexion_create(server_pthread);
		pthread_arg_create(conexion, nuevos_entrenadores);
	}
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
		new_server->numero_clientes_conectados = 0;
		new_server->backlog = 1500;
		return new_server;
}

t_cliente_servidor* conexion_create(t_server_pthread *server)
{
	t_cliente_servidor *new_conexion = malloc(sizeof(t_cliente_servidor));
	int cliente = server_pthread_acepta_conexion_cliente(server);
	new_conexion->cliente = cliente;
	new_conexion->server = server;
	return new_conexion;
}

void pthread_conexion_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores)
{
	t_arg_pthread *arg = pthread_arg_create(conexion,lista_nuevos_entrenadores);

	pthread_t thread;
	int result =pthread_create(&thread,NULL,server_pthread_atender_cliente,(void*)arg);
			if( result <0)
				{perror("could not create thread");}
}

t_arg_pthread* pthread_arg_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores)
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
	server->numero_clientes_conectados++;
	return cliente;
}

void* server_pthread_atender_cliente(void* argumento)
{
	t_arg_pthread *arg_pthread = (t_arg_pthread*) argumento;

	server_pthread_agrega_proceso_a_lista(arg_pthread->lista_nuevos_entrenadores, arg_pthread->conexion->cliente);


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



void server_pthread_cerra_cliente(t_cliente_servidor *cliente_server, int *cliente_on)
{
	close(cliente_server->cliente);
	cliente_server->server->numero_clientes_conectados--;
	*cliente_on = 0;
}



void server_pthread_agrega_proceso_a_lista(t_list *lista_procesos, int socket_cliente)
{
	//DEBE USARSE SEMAFORO
	t_entrenador_nuevo *entrenador = malloc(sizeof(t_entrenador_nuevo));
	entrenador->id_proceso = (int)process_get_thread_id();
	entrenador->socket_entrenador = socket_cliente;
	list_add(lista_procesos,entrenador);
}

