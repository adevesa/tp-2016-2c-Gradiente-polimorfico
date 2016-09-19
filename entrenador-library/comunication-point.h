/*
 * comunication-point.h
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#ifndef COMUNICATION_POINT_H_
#define COMUNICATION_POINT_H_
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "basic-structs.h"


/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_servidor(int puerto, char *ip);

/*-------------------------------------ENVIO DE MENSAJES--------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje);
/*-------------------------------------RECEPCION DE MENSAJES -------------------------------------------------------*/
char* recibir_mensaje(int socket,int payloadSize);
char* recibir_mensaje_especifico(int socket);
/*-------------------------------------------------------FUNCIONES SECUNDARIAS-------------------------------*/
typedef struct sockaddr_in address_config_in;
address_config_in configurar_address_in(int puerto, char *ip);
void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket);

#endif /* COMUNICATION_POINT_H_ */
