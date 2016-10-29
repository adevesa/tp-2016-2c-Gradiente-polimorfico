/*
 * serializacion_pokedex_cliente_servidor.c
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */
#include "serializacion_pokedex_cliente_servidor.h"

/*-----------------------------------------PETICIONES--------------------------------------------*/
char* build_msg(int header, char *path_original, char *path_new_or_text, int size, int offset)
{
	switch(header)
	{
		case(LISTAR):
			{
				char *msg = armar_header(LISTAR);
				char *bytes_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg,bytes_path );
				string_append(&msg, path_original);
				free(bytes_path);
				return msg;
			};break;
		case(GET_ATRIBUTES):
			{
				char *msg = armar_header(GET_ATRIBUTES);
				char *num_bytes_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg, num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
		case(CREATE_FILE):
			{
				char *msg = armar_header(CREATE_FILE);
				char *num_bytes_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg, num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
		case(CREATE_DIRECTORY):
			{
				char *msg = armar_header(CREATE_DIRECTORY);
				char *num_bytes_path =armar_numero_de_bytes(string_length(path_original));
				string_append(&msg,num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
		case(DELETE_FILE):
			{
				char *msg = armar_header(DELETE_FILE);
				char *num_bytes_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg,num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
		case(DELETE_DIRECTTORY):
			{
				char *msg = armar_header(DELETE_DIRECTTORY);
				char *num_bytes_path =  armar_numero_de_bytes(string_length(path_original));
				string_append(&msg,num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
		case(READ_FILE):
			{
				char *msg = armar_header(READ_FILE);
				char *resto = armar_lectura_o_escritura(READ_FILE,path_original,NULL,size,offset);
				string_append(&msg,resto);
				free(resto);
				return msg;
			};break;
		case(WRITE_FILE):
			{
				char *msg = armar_header(WRITE_FILE);
				char *resto = armar_lectura_o_escritura(READ_FILE,path_original,path_new_or_text,size,offset);
				string_append(&msg,resto);
				free(resto);
				return msg;
			};break;
		case(RENAME_FILE):
			{
				char *msg = armar_header(RENAME_FILE);
				char *num_bytes_first_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg, num_bytes_first_path);
				string_append(&msg, path_original);
				char *num_bytes_second_path = armar_numero_de_bytes(string_length(path_new_or_text));
				string_append(&msg,num_bytes_second_path);
				string_append(&msg, path_new_or_text);
				free(num_bytes_first_path);
				free(num_bytes_second_path);
				return msg;
			};break;
		case(OPEN_FILE):
			{
				char *msg = armar_header(OPEN_FILE);
				char *num_bytes_path = armar_numero_de_bytes(string_length(path_original));
				string_append(&msg, num_bytes_path);
				string_append(&msg, path_original);
				free(num_bytes_path);
				return msg;
			};break;
	}
}

char* armar_numero_de_bytes(int size_payload)
{
	char *size_String = string_itoa(size_payload);
	int length = string_length(size_String);

	char *result = string_repeat(' ',MAX_BYTES_TO_SEND-length);
	string_append(&result,size_String);
	free(size_String);
	return result;
	/*if(size_payload < MAX_BYTES_SIZE)
	{
		if((size_payload < 10))
		{
			char *size=string_repeat(' ',4);
			char *size_string = string_itoa(size_payload);
			string_append(&size,size_string);
			free(size_string);
			return size;
		}
		else
		{
			if(size_payload < 100)
			{
				char *size=string_repeat(' ',3);
				char *size_string = string_itoa(size_payload);
				string_append(&size,size_string);
				free(size_string);
				return size;
			}
			else
			{
				if(size_payload < 1000)
				{
					char *size=string_repeat(' ',2);
					char *size_string = string_itoa(size_payload);
					string_append(&size,size_string);
					free(size_string);
					return size;
				}
				else
				{
					char *size=string_repeat(' ',1);
					char *size_string = string_itoa(size_payload);
					string_append(&size,size_string);
					free(size_string);
					return size;
				}
			}
		}
	}
	else
	{
		char *size =string_itoa(size_payload);
		return size;
	}*/
}

char* armar_header(int header)
{
	if(header < 10)
	{
		char *heaader = string_repeat(' ',1);
		char *itoa = string_itoa(header);
		string_append(&heaader,itoa);
		free(itoa);
		return heaader;
	}
	else
	{
		return string_itoa(header);
	}
}

char* armar_lectura_o_escritura(int tipo,char *path, char *text, int size, int offset)
{
	switch(tipo)
	{
		case(READ_FILE):
		{
			char *msg = armar_numero_de_bytes(string_length(path));
			string_append(&msg,path);
			char *size_a_leer = armar_numero_de_bytes(size);
			string_append(&msg, size_a_leer);
			char *a_partir_de = armar_numero_de_bytes(offset);
			string_append(&msg,a_partir_de);
			free(size_a_leer);
			free(a_partir_de);
			return msg;
		};break;
		case(WRITE_FILE):
		{
			char *msg = armar_numero_de_bytes(string_length(path));
			string_append(&msg,path);


			char *size_a_escribir = armar_numero_de_bytes(size);
			string_append(&msg, size_a_escribir);

			char *a_partir_de = armar_numero_de_bytes(offset);
			string_append(&msg,a_partir_de);

			string_append(&msg, text);
			free(size_a_escribir);
			free(a_partir_de);
			return msg;
		};break;
	}
}

/*-----------------------------------------RESPUESTAS-------------------------------------------------*/
