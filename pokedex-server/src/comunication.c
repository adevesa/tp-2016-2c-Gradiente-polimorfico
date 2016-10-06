/*
 * comunication.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */
#include "comunication.h"

char* recibir_mensaje_especifico(int socket, int header)
{
	switch(header)
		{
			case(LISTAR):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(GET_ATRIBUTES):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(CREATE_FILE):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(CREATE_DIRECTORY):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(DELETE_FILE):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(DELETE_DIRECTTORY):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
			case(READ_FILE):
				{

				};break;
			case(WRITE_FILE):
				{

				};break;
			case(RENAME_FILE):
				{

				};break;
			case(OPEN_FILE):
				{
					return escuchar_mensaje_operaciones_basicas(socket);
				};break;
		}
}

char* escuchar_mensaje_operaciones_basicas(int socket)
{
	char *bytes_of_path = recibir_mensaje(socket,BYTES_TO_RCV);
	int bytes_path = atoi(bytes_of_path);
	free(bytes_of_path);
	char *path = recibir_mensaje(socket, bytes_path);
	return path;
}
