/*
 * server.h
 *
 *  Created on: 30/8/2016
 *      Author: utnso
 */

#ifndef SERVER_SELECT_H_
#define SERVER_SELECT_H_

/*
 * TODAS LAS FUNCIONES AQUI DESARROLLADAS
 * SE PUEDE IMPLEMENTAR CON UN SERVER
 * QUE ESCUCHARÁ A SUS CLIENTES CON UN SELECT
 */

//ESTRUCTURA PARA UN SERVIDOR QUE SE IMPLEMENTARÁ CON SELECT//
typedef struct server
{
	int socket_asociado;
	int backlog;
	fd_set *readset;
	int numero_clientes_conectados;
	int *sockets_clientes_conectados;
	int maximo_descriptor;
} t_socket_server;

typedef struct sockaddr_in address_config_in;

						//FUNCIONES PRINCIPALES//
/*
 * SON LAS FUNCIONES QUE SE DEBEN IMPLEMENTAR
 * EN EL MAIN SERVIDOR
 */
t_socket_server* server_stream_create(int puerto, int backlog);
int server_stream_asociate_a_puerto(int server, address_config_in *address);
void server_stream_escucha(t_socket_server *server);
void server_stream_destruite(t_socket_server *server);
void server_stream_atende_clientes(t_socket_server *server,fd_set *descriptorTemporal,int longitudPaquete);
void server_stream_nuevo_cliente (t_socket_server *server);


						//FUNCIONES SECUNDARIAS//
/*
 * SON LAS FUNCIONES A LAS QUE DELEGAN
 * LAS FUNCIONES PRINCIPALES
 */

/* MANIPULACION DE DIRECCIONES */
address_config_in* configurar_address_in(int puerto);
void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket);

/*MANIPULACION DE READSET */
fd_set* readset_create();
void server_agrega_cliente_a_readset(t_socket_server *server, int cliente);
void server_quita_cliente_del_readset(t_socket_server *server, int cliente);
void server_actualiza_maximo_descriptor(t_socket_server *server);
void server_cambia_tu_maximo_descriptor(t_socket_server *server);

/* MANIPULACION DE CLIENTES CONECTADOS */
void server_aumenta_numero_de_conectados(t_socket_server *server, int cliente);
void server_modela_nuevo_cliente(t_socket_server *server);
int server_acepta_conexion_cliente(int socketServer);
void server_atende_cliente(t_socket_server *server, int cliente, int longitudPaquete, char *buffer);
void server_cerra_cliente(t_socket_server *server, int cliente);
int server_permitis_nuevo_cliente(int nClientesConectados, int backlog);

/* MANIPULACION DE RESULTADOS Y TRANSFERENCIA CON EL CLIENTE*/
void server_informa_conexion_exitosa(int cliente);
void server_saluda_al_cliente(int socketCliente);

#endif /* SERVER_SELECT_H_ */
