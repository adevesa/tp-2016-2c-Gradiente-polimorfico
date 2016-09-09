/*
 * server-pthread.h
 *
 *  Created on: 2/9/2016
 *      Author: utnso
 */

#ifndef SERVER_PTHREAD_H_
#define SERVER_PTHREAD_H_

typedef struct sockaddr_in address_config_in;

typedef struct server
{
	int socket_asociado;
	int backlog;
	int numero_clientes_conectados;
	int longitud_paquetes;
} t_server_pthread;

typedef struct cliente_servidor
{
	t_server_pthread *server;
	int cliente;
} t_cliente_servidor;

typedef struct arg
{
	t_cliente_servidor *conexion;
	/*t_list *lista_nuevos_entrenadores;*/
}t_arg_pthread;

/*
 * @name: server_pthread_create(int puerto, int backlog)
 * @decryp: Dado un numero de puerto y un número máximo de clientes que
 * 			se pueden conectar (backlog), devuelve un puntero inicializado a
 * 			un struct de tipo t_server_pthread
 */
t_server_pthread* server_pthread_create(int puerto, int backlog, int longitudPaquetes, char *ip);

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

/*
 * @name:
 * @decryp:
 */
int enviar_a_cliente(int socketCliente, char* buffer, int bytesAenviar);

/*
 * @name:
 * @decryp:
 */

/*void ejecutar_hilo_socket(int puerto, int backlog, int longitudPaquete, char *ip, t_list *nuevos_entrenadores);*/

/*
 * @name:
 * @decryp:
 */
void server_pthread_cerra_cliente(t_cliente_servidor *cliente_server, int *cliente_on);

/*
 * @name:
 * @decryp:
 */
void server_pthread_mostra_resultados_por_pantalla(char *buffer);

/*
 * @name:
 * @decryp:
 */
void server_pthread_evalua_resultado_cliente(int bytes,char *buffer, t_cliente_servidor *cliente_server, int *cliente_on);

void server_pthread_saluda_cliente(int cliente);

void server_pthread_recibi_datos(t_cliente_servidor *cliente_server);

t_cliente_servidor* conexion_create(t_server_pthread *server);
/*t_arg_pthread* pthread_arg_create(t_cliente_servidor *conexion, t_list *lista_nuevos_entrenadores);*/
#endif /* SERVER_PTHREAD_H_ */
