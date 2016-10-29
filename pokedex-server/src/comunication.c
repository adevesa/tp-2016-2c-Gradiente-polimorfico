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
	//char *ip = getenv("IP");
	//char *puerto_string = getenv("PUERTO");
	//int puerto = atoi(puerto_string)

	char *ip = string_new();
	string_append(&ip,"127.0.0.1");
	servidor_pokedex = server_create(5001, ip, 1500);
	server_escucha(servidor_pokedex);
}

void servidor_acepta_clientes()
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

void* server_pokedex_atende_cliente(void* socket_cliente)
{
	int *cliente = (int*) socket_cliente;
	int cliente_esta_conectado = 1;

	while(cliente_esta_conectado)
	{
		printf("Espero peticion de cliente\n");
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
			printf("LISTAR: %s\n",path);
			void* resultado = osada_a_get_list_dir(path);
			if((int) resultado == NO_EXISTE)
			{
				enviar_mensaje(cliente,"FFFF");
			}
			else
			{
				char *listado_string = armar_listado((t_list*) resultado);
				enviar_mensaje(cliente,listado_string);
				free(listado_string);
			}
			free(path);
			list_destroy_and_destroy_elements(resultado,file_listado_eliminate);

		};break;
		case(GET_ATRIBUTES):
		{
			char *path = recibir_mensaje_especifico(cliente, GET_ATRIBUTES);
			printf("ATRIBUTOS: %s\n",path);
			void* respuesta = osada_a_get_attributes(path);
			if((int) respuesta == NO_EXISTE)
			{
				enviar_mensaje(cliente,"F");
				free(path);
			}
			else
			{
				t_attributes_file *file = (t_attributes_file*) respuesta;
				char *mensj = armar_attributes(file);
				enviar_mensaje(cliente, mensj);
				free(file);
				free(path);
			}
		};break;
		case(CREATE_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, CREATE_FILE);
			printf("CREAR: %s\n",path);
			int resultado_operacion = osada_a_create_file(path);
			responder_solo_resultado(cliente,resultado_operacion);
			free(path);
		};break;
		case(CREATE_DIRECTORY):
		{
			char *path = recibir_mensaje_especifico(cliente, CREATE_DIRECTORY);
			printf("CREAR: %s\n",path);
			int resultado_operacion = osada_a_create_dir(path);
			responder_solo_resultado(cliente,resultado_operacion);
			free(path);
		};break;
		case(DELETE_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, DELETE_FILE);
			printf("DELETE: %s\n",path);
			int resultado_operacion = osada_a_delete_file(path);
			responder_solo_resultado(cliente,resultado_operacion);
			free(path);
		};break;
		case(DELETE_DIRECTTORY):
		{
			char *path = recibir_mensaje_especifico(cliente, DELETE_DIRECTTORY);
			printf("DELETE: %s\n",path);
			int resultado_operacion = osada_a_delete_dir(path);
			responder_solo_resultado(cliente,resultado_operacion);
			free(path);
		};break;
		case(READ_FILE):
		{
			t_to_be_read *file_to_read = recibir_mensaje_especifico(cliente, READ_FILE);
			printf("LEER: %d BYTES OFFSET: %d DE: %s\n", file_to_read->size,file_to_read->offset,file_to_read->path);
			void *result = osada_a_read_file(file_to_read);
			if((int)result == NO_EXISTE || (int) result == ARGUMENTO_INVALIDO)
			{
				enviar_mensaje(cliente,"F");
				//responder_solo_resultado(cliente, (int)resultado);

			}
			else
			{
				read_content *resultado = (read_content*) result;
				printf("LECTURA COMPLETADA: %d BYTES DE: %s\n",resultado->tamanio ,file_to_read->path);
				void *buffer = malloc(resultado->tamanio + 10);
				char *tam = string_itoa(resultado->tamanio);
				int tamanio_del_archivo = string_length(tam);
				//printf("BYTES OCUPADOS POR TAMANIO: %d\n", tamanio_del_archivo);
				char *mensaje = string_repeat(' ', 10 -tamanio_del_archivo);
				string_append(&mensaje,tam);
				free(tam);
				memcpy(buffer,mensaje,10);
				memcpy(buffer+10,resultado->contenido,resultado->tamanio);
				enviar_mensaje_cantidad_especifica(cliente,buffer,resultado->tamanio +10);
				free(mensaje);
				free(resultado->contenido);
				free(resultado);
			}
			free(file_to_read->path);
			free(file_to_read);
		};break;
		case(WRITE_FILE):
		{
			t_to_be_write *file_to_write = recibir_mensaje_especifico(cliente,WRITE_FILE);
		};break;
		case(RENAME_FILE):
		{
			t_to_be_rename *file_to_rename= recibir_mensaje_especifico(cliente, RENAME_FILE);
			int resultado_operacion = osada_a_rename(file_to_rename);
			responder_solo_resultado(cliente,resultado_operacion);
			free(file_to_rename->new_path);
			free(file_to_rename->old_path);
			free(file_to_rename);
		};break;
		case(OPEN_FILE):
		{
			char *path = recibir_mensaje_especifico(cliente, OPEN_FILE);
			printf("ABRIR: %s\n",path);
			int resultado_operacion = osada_a_open_file(path);
			responder_solo_resultado(cliente,resultado_operacion);
			free(path);
		};break;
	}


}

/*--------------------------------------PETICIONES DEL CLIENTE-----------------------------------------------------*/
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
	char *new_path = recibir_mensaje(socket, bytes_path_2);
	to_rename->new_path =new_path;

	return to_rename;
}

/*--------------------------------------RESPUESTAS AL CLIENTE----------------------------------------------------*/
void responder_solo_resultado(int cliente, int resultado)
{
	char *resultado_a_enviar = string_itoa(resultado);
	enviar_mensaje(cliente, resultado_a_enviar);
	free(resultado_a_enviar);
}

char* armar_attributes(t_attributes_file *attributes)
{
	char *size = string_itoa(attributes->size);
	char *tipo = string_itoa(attributes->tipo);
	int tamanio = string_length(size);

	char *msg = string_repeat(' ', 10-tamanio);
	string_append(&msg,size);
	string_append(&msg,tipo);

	free(size);
	free(tipo);
	return msg;
}

char* armar_listado(t_list *listado)
{
	char* listado_string = string_new();
	if(list_is_empty(listado))
	{
		string_append(&listado_string, "0000");
		return listado_string;
	}
	else
	{
		int size = list_size(listado);
		modelar_cantidad_elementos_listado(listado_string,size);
		char *list=modelar_elementos_en_listado(listado,size);
		string_append(&listado_string,list);
		free(list);
		return listado_string;
	}

}

void modelar_cantidad_elementos_listado(char* buffer, int size)
{
	char *size_string = string_itoa(size);
	if(size < 10)
	{
		char* repeat = string_repeat(' ',3);
		string_append(&buffer,repeat);
		string_append(&buffer,size_string);
		free(repeat);
		free(size_string);
	}
	else
	{
		if(size<100)
		{
			char* repeat = string_repeat(' ',2);
			string_append(&buffer,repeat);
			string_append(&buffer,size_string);
			free(repeat);
			free(size_string);
		}
		else
		{
			if(size<1000)
			{
				char* repeat = string_repeat(' ',1);
				string_append(&buffer,repeat);
				string_append(&buffer,size_string);
				free(repeat);
				free(size_string);
			}
			else
			{
				string_append(&buffer,size_string);
				free(size_string);
			}
		}
	}
}

char* modelar_elementos_en_listado(t_list *listado, int size)
{
	char *resultado = string_new();
	int i=0;
	while(i<size)
	{
		t_file_listado *file = list_get(listado,i);
		int tamanio_nombre = string_length((char*)file->file->file->fname);
		char *size_name=modelar_tamanio_nombre(tamanio_nombre);
		string_append(&resultado,size_name);
		free(size_name);
		string_append(&resultado, (char*) file->file->file->fname);
		i++;
	}
	return resultado;
}

char* modelar_tamanio_nombre(int size)
{

	if(size < 10)
	{
		char *repeat=string_repeat(' ',1);
		char *size_string = string_itoa(size);
		string_append(&repeat, size_string);
		free(size_string);
		return repeat;
	}
	else
	{
		char *size_string = string_itoa(size);
		return size_string;
	}
}

void agregar_barra_si_es_necesario(char *path)
{
	if(!string_starts_with(path,"/"))
	{
		char *aux = string_new();
		string_append(&aux,"/");
		string_append(&aux, path);
		path=realloc(path,string_length(path) + 2);
		path = aux;
		free(aux);
	}
}
