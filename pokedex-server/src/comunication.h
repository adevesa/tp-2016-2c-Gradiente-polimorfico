/*
 * comunication.h
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#include "socket/serializacion_pokedex_cliente_servidor.h"
#define BYTES_TO_RCV 5


char* recibir_mensaje_especifico(int socket, int header);
char* escuchar_mensaje_operaciones_basicas(int socket);
#endif /* COMUNICATION_H_ */
