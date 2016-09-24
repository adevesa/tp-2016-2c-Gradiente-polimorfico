/*
 * send&recv.h
 *
 *  Created on: 24/9/2016
 *      Author: utnso
 */

#ifndef SENDYRECV_H_
#define SENDYRECV_H_

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <commons/string.h>
#include "string.h"

void enviar_mensaje(int socket, char *mensaje);
char* recibir_mensaje(int socket,int payloadSize);
char* recibir_mensaje_especifico(int socket);
char* armar_mensaje(char *header, char *payload, int MAX_BYTES_TOTAL_A_ENVIAR);

#endif /* SENDYRECV_H_ */
