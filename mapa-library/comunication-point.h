/*
 * comunication-point.h
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#ifndef MAPA_MAPA_COMMONS_COMUNICATION_POINT_H_
#define MAPA_MAPA_COMMONS_COMUNICATION_POINT_H_
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <commons/process.h>
#include "map-commons.h"


/* ----------------------------------------STRUCTS---------------------------------------------*/
typedef struct sockaddr_in address_config_in;

typedef struct server
{
	int socket_asociado;
	int backlog;
} t_server_pthread;

typedef struct cliente_servidor
{
	t_server_pthread *server;
	int cliente;
} t_cliente_servidor;

typedef struct arg
{
	int *conexion;
	t_list *lista_nuevos_entrenadores;
}t_arg_pthread;


/*-----------------------------------------------EXECUTE-----------------------------------------------------------*/
void* ejecutar_servidor(void *argumento);
void ejecutar_hilo_socket(int puerto,char *ip);

/*------------------------------------------------CREATES----------------------------------------------------------*/

/*
 * @name: server_pthread_create(int puerto, int backlog)
 * @decryp: Dado un numero de puerto y un número máximo de clientes que
 * 			se pueden conectar (backlog), devuelve un puntero inicializado a
 * 			un struct de tipo t_server_pthread
 */
t_server_pthread* server_pthread_create(int puerto,char *ip);
int conexion_create(t_server_pthread *server);

t_arg_pthread* pthread_arg_create(int *conexion, t_list *lista_nuevos_entrenadores);

void pthread_conexion_create(int *conexion);

/*--------------------------------------------PINCIPALES-----------------------------------------------------------*/
/*
 * @name: int server_pthread_acepta_conexion_cliente(int socketServer)
 * @decryp: El socket recibido por parámetro acepta conexiones de clientes que intentan conectarse
 */
int server_pthread_acepta_conexion_cliente(t_server_pthread *server);

/*
 * @name: void *server_pthread_atender_cliente(void *argumento)
 * @decryp: funcón con la que comienza la ejecución del thread (HILO).
 * 			Contiene toda la lógica para dialogar con un cliente.
 */
void* server_pthread_atender_cliente(void* argumento);

/*-------------------------------------ENVIO DE MENSAJES A ENTRENADORES----------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje);


/*--------------------------------------RECEPCION DE MENSAJES -------------------------------------------------*/
char* recibir_mensaje(int socket,int payloadSize);
char* recibir_mensaje_especifico(int socket);
/*--------------------------------------------SECUNDARIOS-----------------------------------------------------------*/
/*
 * @name: configurar_address(int puerto)
 * @decryp: Dado un numero de puerto, crea un struct sockaddr_in con la siguiente info:
 * 			-sin_family = AF_INET;
			-sin_addr.s_addr=INADDR_ANY;
			-sin_port=htons(puerto);
 */
address_config_in* configurar_address(int puerto, char*ip);

/*
 * @name: reutilizar_direccion(int valorVerdaderoso, int socket)
 * @decryp: Activa la reutilización de la dirección del socket
 * 			para que una vez finalizado el proceso, otro pueda
 * 			tomar el mismo ID de socket.
 */
void reutilizar_direccion(int valorVerdaderoso, int socket);

/*
 * @name: server_pthread_asociate_a_puerto(int server, address_config_in *address)
 * @decryo: Asocia un socket al puerto especificado en addres_config_in
 */
int server_pthread_asociate_a_puerto(int server, address_config_in *address);

/*
 * @name: server_pthread_escucha(t_server_pthread *server)
 * @decryp: Se pone en modo listening al socket asociado al t_server_pthread
 */
void server_pthread_escucha(t_server_pthread *server);

void server_pthread_cerra_cliente(int cliente);

void server_pthread_agrega_proceso_a_lista(int *socket_cliente);



#endif /* MAPA_MAPA_COMMONS_COMUNICATION_POINT_H_ */
