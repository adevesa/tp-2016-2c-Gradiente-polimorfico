/*
 * servidor.h
 *
 *  Created on: 6/9/2016
 *      Author: utnso
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include "string.h"
#include <pthread.h>
#include <commons/process.h>


/* ----------------------------------------STRUCTS---------------------------------------------*/
typedef struct sockaddr_in address_config_in;

typedef struct server
{
	int socket_asociado;
	int backlog;
	int numero_clientes_conectados;
} t_server_pthread;

typedef struct cliente_servidor
{
	t_server_pthread *server;
	int cliente;
} t_cliente_servidor;

typedef struct arg
{
	t_cliente_servidor *conexion;
	t_list *lista_nuevos_entrenadores;
}t_arg_pthread;

/*-----------------------------------------------EXECUTE-----------------------------------------------------------*/

void ejecutar_hilo_socket(int puerto,char *ip, t_list *nuevos_entrenadores);

/*------------------------------------------------CREATES----------------------------------------------------------*/

/*
 * @name: server_pthread_create(int puerto, int backlog)
 * @decryp: Dado un numero de puerto y un número máximo de clientes que
 * 			se pueden conectar (backlog), devuelve un puntero inicializado a
 * 			un struct de tipo t_server_pthread
 */
t_server_pthread* server_pthread_create(int puerto,char *ip);

t_cliente_servidor* conexion_create(t_server_pthread *server);

t_arg_pthread* pthread_arg_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores);

void pthread_conexion_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores);

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
void otorgar_turno_a_entrenador(int entrenador);
void otorgar_posicion_pokenest_a_entrenador(int entrenador, int x, int y);
void otorgar_pokemon_a_entrenador(int entrenador, int nivelPokemon);
void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla);


/*--------------------------------------RECEPCION DE MENSAJES -------------------------------------------------*/
char* escuchar_al_entrenador(int entrenador);
char* escuchar_que_pokemon_busca(int entrenador);
void esuchar_a_que_direccion_se_mueve(int entrenador, int *x, int *y);

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

void server_pthread_cerra_cliente(t_cliente_servidor *cliente_server, int *cliente_on);

void server_pthread_agrega_proceso_a_lista(t_list *lista_procesos);

char* armar_mensajee(char *header, char *payload);

#endif /* SERVIDOR_H_ */
