/*
 * comunication.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */
#include "comunication.h"

/*--------------------------------------CONEXION--------------------------------------------------------------*/
void pokedex_server_conectate()
{
	char *ip = getenv("IP");
	char *puerto_string = getenv("PUERTO");
	int puerto = atoi(puerto_string);
	servidor_pokedex = server_create(puerto, ip, MAX_CONECCTIONS);
	server_escucha(servidor_pokedex);
}

void pokedex_server_acepta_clientes()
{
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread,&attr,servidor_acepta_clientes,NULL);

	pthread_attr_destroy(&attr);
}

void servidor_acepta_clientes(void *arg)
{
	while(1)
	{
		int cliente = server_acepta_conexion_cliente(servidor_pokedex);

		//COMO ACCEPT ES BLOQUEANTE --> SI ESTÁ EN ESTE PUNTO ES QUE YA HAY UN CLIENTE ONLINE
		pthread_attr_t attr;
		pthread_t thread;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&thread,&attr,server_pokedex_atende_cliente,(void*)&cliente);

		pthread_attr_destroy(&attr);

	}
}

void server_pokedex_atende_cliente(void* socket_cliente)
{
	int *cliente = (int*) socket_cliente;
	int cliente_esta_conectado = 1;

	while(cliente_esta_conectado)
	{
		char *peticion = server_escucha_peticion(*cliente);
		if(string_equals_ignore_case(peticion, "DESCONECTADO"))
		{
			cliente_esta_conectado = 0;
		}
		else
		{
			tratar_peticion_de(*cliente, peticion);
		}
	}
	server_cerra_cliente(*cliente);
	pthread_exit(NULL);
}

char* server_escucha_peticion(int cliente)
{
	char *peticion = recibir_mensaje(cliente,HEADER);
	return peticion;
}

void tratar_peticion_de(int cliente,char *peticion)
{
	int peticion_header = atoi(peticion);

	switch (peticion_header)
	{
		case(LISTAR):
		{
			char *path = recibir_mensaje_especifico(cliente, LISTAR);
		};break;
		case(GET_ATRIBUTES):
		{
			char *path = recibir_mensaje_especifico(cliente, GET_ATRIBUTES);
		};break;
		case(CREATE_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, CREATE_FILE);
		};break;
		case(CREATE_DIRECTORY):
		{
			char *path = recibir_mensaje_especifico(cliente, CREATE_DIRECTORY);
		};break;
		case(DELETE_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, DELETE_FILE);
		};break;
		case(DELETE_DIRECTTORY):
		{
			char *path = recibir_mensaje_especifico(cliente, DELETE_DIRECTTORY);
		};break;
		case(READ_FILE):
		{
			t_to_be_read *file_to_read = recibir_mensaje_especifico(cliente, READ_FILE);
		};break;
		case(WRITE_FILE):
		{
			t_to_be_write *file_to_write = recibir_mensaje_especifico(cliente,WRITE_FILE);
		};break;
		case(RENAME_FILE):
		{
			t_to_be_rename *file_to_rename= recibir_mensaje_especifico(cliente, RENAME_FILE);
		};break;
		case(OPEN_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, OPEN_FILE);
		};break;
	}


}

/*--------------------------------------MENSAJES CON CLIENTES-----------------------------------------------------*/
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
