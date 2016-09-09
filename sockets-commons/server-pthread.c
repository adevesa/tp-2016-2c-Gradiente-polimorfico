/*
 * server-pthread.c
 *
 *  Created on: 2/9/2016
 *      Author: utnso
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include "server-pthread.h"
#include <commons/collections/list.h>
#include "string.h"
#include <pthread.h>
#include <commons/process.h>


/*--------------------------------------------PINCIPALES-----------------------------------------------------*/

t_server_pthread* server_pthread_create(int puerto, int backlog, int longitudPaquetes, char *ip)
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
		new_server->backlog = backlog;
		new_server->longitud_paquetes=longitudPaquetes;
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

	/*server_pthread_agrega_proceso_a_lista(arg_pthread->lista_nuevos_entrenadores);*/

	server_pthread_saluda_cliente(arg_pthread->conexion->cliente); //<-- No necesita estar si o si

	server_pthread_recibi_datos(arg_pthread->conexion);

	pthread_exit(NULL);
}

void ejecutar_hilo_socket(int puerto, int backlog, int longitudPaquete, char *ip, t_list *nuevos_entrenadores)
{
	//CREACION E INICIALIZACION DEL SERVIDOR//
	t_server_pthread *server_pthread = server_pthread_create(puerto,backlog, longitudPaquete, ip);

	//SERVER SE PONE A ESCUCHAR//
	server_pthread_escucha(server_pthread);

	int se_puede_ejecutar = 1;
	while(se_puede_ejecutar)
	{
		t_cliente_servidor *conexion = conexion_create(server_pthread);

		printf("Conexión de %d\n", conexion->cliente); fflush(stdout);//<--No necesita estar

		//CREACION DE UN HILO POR CADA CLIENTE CONECTADO//

		pthread_t thread;
		int result =pthread_create(&thread,NULL,server_pthread_atender_cliente,(void*)&conexion);
		if( result <0)
			{perror("could not create thread");}
		else
		{
			printf("Cliente aceptado y conversando con él\n");//<--No necesita estar
			fflush(stdout);
		}
		printf("Sigo esperando clientes\n"); fflush(stdout); //<--No necesita estar

	}

}

void pthread_conexion_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores)
{
	t_arg_pthread *arg = pthread_arg_create(conexion,lista_nuevos_entrenadores);

	pthread_t thread;
	int result =pthread_create(&thread,NULL,server_pthread_atender_cliente,(void*)arg);
			if( result <0)
				{perror("could not create thread");}
}

/*t_arg_pthread* pthread_arg_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores)
{
	t_arg_pthread *new_arg = malloc(sizeof(t_arg_pthread));
	new_arg->conexion = conexion;
	new_arg->lista_nuevos_entrenadores = lista_nuevos_entrenadores;
	return new_arg;
}*/

/*--------------------------------------------SECUNDARIOS-----------------------------------------------------*/



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

	//Verificamos que no haya errores al asociar puerto//
	if(resultBind !=0)
	{	perror("Falló el bind");
		return 1;
	}
	return resultBind;
}

void server_pthread_escucha(t_server_pthread *server)
{
	printf("Escuchando...\n");
	fflush(stdout);
	listen(server->socket_asociado,server->backlog);
}

void server_pthread_evalua_resultado_cliente(int bytes,char *buffer, t_cliente_servidor *cliente_server, int *cliente_on)
{
	switch (bytes)
	{
		case 0 : {
					server_pthread_cerra_cliente(cliente_server, cliente_on);
					break;
				}
		case -1 : {break;}
		default: server_pthread_mostra_resultados_por_pantalla(buffer); //<--ACA DEBE IR EL ALGORITMO QUE TRATE LA RESPUESTA

	}
}

void server_pthread_mostra_resultados_por_pantalla(char *buffer)
{
	printf("%s", buffer);
}

void server_pthread_cerra_cliente(t_cliente_servidor *cliente_server, int *cliente_on)
{
	close(cliente_server->cliente);
	cliente_server->server->numero_clientes_conectados--;
	*cliente_on = 0;
}

int enviar_a_cliente(int socketCliente, char* buffer, int bytesAenviar)
{
	int result = send(socketCliente, buffer, bytesAenviar,0);
	return result;
}

void server_pthread_saluda_cliente(int cliente)
{
	char* msg = "Bienvenido\n";
	enviar_a_cliente(cliente, msg, strlen(msg));
	//enviar_a_cliente(cliente, ":)",3 );
}

void server_pthread_recibi_datos(t_cliente_servidor *cliente_server)
{
	int bytes;
	int longitudPaquetes = cliente_server->server->longitud_paquetes;
	char* buffer = malloc(longitudPaquetes);
	int cliente_esta_ON = 1;
	while(cliente_esta_ON)
	{
		bytes= recv(cliente_server->cliente, buffer, longitudPaquetes ,0);
		server_pthread_evalua_resultado_cliente(bytes, buffer, cliente_server, &cliente_esta_ON);
	}
	free(buffer);
}

void server_pthread_agrega_proceso_a_lista(t_list *lista_procesos)
{
	//DEBE USARSE SEMAFORO
	list_add(lista_procesos,(int*)process_get_thread_id());
}

