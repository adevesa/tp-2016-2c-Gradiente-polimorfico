/*
 * sockets-utils.h
 *
 *  Created on: 25/9/2016
 *      Author: utnso
 */

#ifndef SOCKETS_UTILS_H_
#define SOCKETS_UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "string.h"

typedef struct sockaddr_in address_config_in;

address_config_in configurar_address_in(int puerto, char *ip);
void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket);

void enviar_mensaje(int socket, char *mensaje);
char* recibir_mensaje(int socket,int payloadSize);

#endif /* SOCKETS_UTILS_H_ */
