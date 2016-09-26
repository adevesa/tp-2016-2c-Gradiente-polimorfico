/*
 * serializacion_mapa_entrenador.c
 *
 *  Created on: 25/9/2016
 *      Author: utnso
 */
#include "serializacion_mapa_entrenador.h"

char* recibir_mensaje_especifico(int socket)
{
	char * payload = malloc(5);
	int bytes_recibidos = 0;
	while(bytes_recibidos != 4)
	{
		bytes_recibidos=recv(socket, payload, 4,0);
	}

	payload[4]= '\0';
	char **solo_tamanio = string_split(payload, ";");

	string_trim_left(&solo_tamanio[0]);
	int tamanio_del_mensaje = atoi(solo_tamanio[0]);
	char *mensaje_final = recibir_mensaje(socket, tamanio_del_mensaje);
	return mensaje_final;

}

char* armar_mensaje(char *header, char *payload, int max_bytes)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	int tamanio_payload = strlen(payload);
	if(tamanio_payload < max_bytes)
	{
		if(tamanio_payload < 10)
		{
			char *tamanio_payload_a_enviar = string_repeat(' ',2);
			string_append(&tamanio_payload_a_enviar, string_itoa(tamanio_payload));
			string_append(&mensaje, tamanio_payload_a_enviar);
			string_append(&mensaje,";");
			string_append(&mensaje,payload);
			return mensaje;
		}
		else
		{
			char *tamanio_payload_a_enviar = string_repeat(' ',1);
			string_append(&tamanio_payload_a_enviar, string_itoa(tamanio_payload));
			string_append(&mensaje, tamanio_payload_a_enviar);
			string_append(&mensaje,";");
			string_append(&mensaje,payload);
			return mensaje;
		}

	}
	else
	{
		string_append(&mensaje, string_itoa(max_bytes));
		string_append(&mensaje,";");
		string_append(&mensaje,payload);
		return mensaje;
	}
}

char* armar_coordenada(int x, int y, int max_bytes)
{
	char *coordenada_x = string_itoa(x);
	char *coordenada_y = string_itoa(y);
	int longitud_eje_x = strlen(coordenada_x);
	int longitud_eje_y = strlen(coordenada_y);
	if(longitud_eje_x < (max_bytes))
	{
		char *nueva_coordenada_x = string_repeat(' ', max_bytes - longitud_eje_x);
		string_append(&nueva_coordenada_x, coordenada_x);
		if(longitud_eje_y < (max_bytes))
		{
			char *nueva_coordenada_y = string_repeat(' ', (max_bytes - longitud_eje_y));
			string_append(&nueva_coordenada_y, coordenada_y);
			char *coordenada_final = string_new();
			string_append(&coordenada_final, nueva_coordenada_x);
			string_append(&coordenada_final, ";");
			string_append(&coordenada_final, nueva_coordenada_y);
			return coordenada_final;
		}
		else
		{
			string_append(&coordenada_y, coordenada_y);
			char *coordenada_final = string_new();
			string_append(&coordenada_final, nueva_coordenada_x);
			string_append(&coordenada_final, ";");
			string_append(&coordenada_final,coordenada_y);
			return coordenada_final;
		}

	}
	else
	{
		string_append(&coordenada_x, coordenada_x);
				if(longitud_eje_y < (max_bytes))
				{
					char *nueva_coordenada_y = string_repeat(' ', (max_bytes - longitud_eje_y));
					string_append(&nueva_coordenada_y, coordenada_y);
					char *coordenada_final = string_new();
					string_append(&coordenada_final,coordenada_x);
					string_append(&coordenada_final, ";");
					string_append(&coordenada_final, nueva_coordenada_y);
					return coordenada_final;
				}
				else
				{
					string_append(&coordenada_y, coordenada_y);
					char *coordenada_final = string_new();
					string_append(&coordenada_final,coordenada_x);
					string_append(&coordenada_final, ";");
					string_append(&coordenada_final,coordenada_y);
					return coordenada_final;
				}
	}

}

/*-------------------------------------------MORE STRINGS FUNCTIONS------------------------------------------------------*/

void free_string_array(char **path)
{
	int i =0;
	while(path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
}

int string_contains(char *palabra, char *conteiner)
{
	int i;
	int esta_en_palabra =0;
	int cantidad_letras = string_length(palabra);
	for(i=0; i<cantidad_letras;i++)
	{
		if(palabra[i] == conteiner[0])
		{
			esta_en_palabra = 1;
		}
	}

	return esta_en_palabra;
}

void string_replace(char *palabra, char *este_caracter,char *por_este)
{
	char **por_separado=string_split(palabra, este_caracter);
	char *palabra_new= string_new();
	int i = 0;
	int cantidad_palabras_contenidas = 0;
	while(por_separado[i] != NULL)
	{
		cantidad_palabras_contenidas++;
		i++;
	}

	i = 0;
	int numero_palabra = 0;
	while(por_separado[i] !=NULL)
	{

		if(numero_palabra == (cantidad_palabras_contenidas-1) )
		{
			string_append(&palabra_new, por_separado[i]);
		}
		else
		{
			string_append(&palabra_new, por_separado[i]);
			string_append(&palabra_new,por_este);
		}
		numero_palabra++;
		i++;
	}
	strcpy(palabra, palabra_new);
	free_string_array(por_separado);
	free(palabra_new);
}

void string_path_replace_spaces(char *path, char *este_caracter, char *por_este)
{

		char **por_separado = string_split(path,"/");
		int i = 0;
		char *aux = string_new();
		int cantidad_elementos = 0;
		while(por_separado[i] != NULL)
		{
			cantidad_elementos++;
			i++;
		}
		i=0;
		while(por_separado[i] != NULL)
		{
			if( i == (cantidad_elementos -1))
			{
				if(string_ends_with(path, "/"))
				{
					if(string_contains(por_separado[i],este_caracter))
					{
						string_replace(por_separado[i],este_caracter,por_este);
						string_append(&aux, por_separado[i]);
						string_append(&aux,"/");
					}
					else
					{
						string_append(&aux, por_separado[i]);
					}
				}
				else
				{
					if(string_contains(por_separado[i],este_caracter))
					{
						string_replace(por_separado[i],este_caracter,por_este);
						string_append(&aux, por_separado[i]);
					}
					else
					{
						string_append(&aux, por_separado[i]);
					}
				}
			}
			else
			{
				if(string_contains(por_separado[i],este_caracter))
				{
					string_replace(por_separado[i],este_caracter,por_este);
				}
				string_append(&aux, por_separado[i]);
				string_append(&aux,"/");

			}
			i++;
		}
		strcpy(path, aux);
		free(aux);
		free_string_array(por_separado);

}

