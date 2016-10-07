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
#define HEADER 2

typedef struct
{
	int size;
	int offset;
	char* path;
}t_to_be_read;

typedef struct
{
	int size;
	int offset;
	char* path;
	char* text;
}t_to_be_write;

typedef struct
{
	char* old_path;
	char* new_path;
}t_to_be_rename;


void* recibir_mensaje_especifico(int socket, int header);
char* escuchar_mensaje_operaciones_basicas(int socket);
t_to_be_read* escuchar_mensaje_read(int socket);
t_to_be_write* escuchar_mensaje_write(int socket);
t_to_be_rename* escuchar_mensaje_rename(int socket);


#endif /* COMUNICATION_H_ */
