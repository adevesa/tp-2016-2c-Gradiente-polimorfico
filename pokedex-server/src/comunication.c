/*
 * comunication.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */
#include "comunication.h"

void* recibir_mensaje_especifico(int socket, int header)
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
					return escuchar_mensaje_read(socket);
				};break;
			case(WRITE_FILE):
				{
					return escuchar_mensaje_write(socket);
				};break;
			case(RENAME_FILE):
				{
					return escuchar_mensaje_rename(socket);
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

t_to_be_read* escuchar_mensaje_read(int socket)
{
	t_to_be_read *to_read = malloc(sizeof(t_to_be_read));

	char *bytes_of_path = recibir_mensaje(socket,BYTES_TO_RCV);
	int bytes_path = atoi(bytes_of_path);
	free(bytes_of_path);
	char *path = recibir_mensaje(socket, bytes_path);
	to_read->path = path;

	char *size_to_be_read = recibir_mensaje(socket, BYTES_TO_RCV);
	int size_to_read = atoi(size_to_be_read);
	free(size_to_be_read);
	to_read->size = size_to_read;

	char *offset_string = recibir_mensaje(socket, BYTES_TO_RCV);
	int offset = atoi(offset_string);
	free(offset_string);
	to_read->offset = offset;

	return to_read;
}

t_to_be_write* escuchar_mensaje_write(int socket)
{
	t_to_be_write *to_write = malloc(sizeof(t_to_be_write));

	char *bytes_of_path = recibir_mensaje(socket,BYTES_TO_RCV);
	int bytes_path = atoi(bytes_of_path);
	free(bytes_of_path);
	char *path = recibir_mensaje(socket, bytes_path);
	to_write->path = path;

	char *size_to_be_write = recibir_mensaje(socket, BYTES_TO_RCV);
	int size_to_write= atoi(size_to_be_write);
	free(size_to_be_write);
	to_write->size = size_to_write;

	char *offset_string = recibir_mensaje(socket, BYTES_TO_RCV);
	int offset = atoi(offset_string);
	free(offset_string);
	to_write->offset = offset;

	char *text = recibir_mensaje(socket, size_to_write);
	to_write->text = text;

	return to_write;

}

t_to_be_rename* escuchar_mensaje_rename(int socket)
{
	t_to_be_rename *to_rename = malloc(sizeof(t_to_be_rename));

	char *bytes_of_path = recibir_mensaje(socket,BYTES_TO_RCV);
	int bytes_path = atoi(bytes_of_path);
	free(bytes_of_path);
	char *old_path = recibir_mensaje(socket, bytes_path);
	to_rename->old_path =old_path;

	char *bytes_of_path_2= recibir_mensaje(socket,BYTES_TO_RCV);
	int bytes_path_2 = atoi(bytes_of_path_2);
	free(bytes_of_path_2);
	char *new_path = recibir_mensaje(socket, bytes_path);
	to_rename->new_path =new_path;

	return to_rename;
}
