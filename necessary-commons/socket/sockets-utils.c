/*
 * sockets-utils.c
 *
 *  Created on: 25/9/2016
 *      Author: utnso
 */

#include "sockets-utils.h"

address_config_in configurar_address_in(int puerto, char *ip)
{
	address_config_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr=inet_addr(ip);
	direccionServidor.sin_port=htons(puerto);
	memset(&(direccionServidor.sin_zero), '\0', 8);
	return direccionServidor;
}

void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket)
{
	setsockopt(socket,SOL_SOCKET,SO_REUSEADDR, &valorVerdaderoso, sizeof(valorVerdaderoso));
}

void enviar_mensaje_cantidad_especifica(int socket, void *buffer, int size)
{
	int cantidad_total_enviada = 0;
	cantidad_total_enviada = send(socket,buffer,size,0);
	if(cantidad_total_enviada < size)
	{
		while(cantidad_total_enviada < size)
		{
			void *data = malloc(size-cantidad_total_enviada);
			memcpy(data,buffer+cantidad_total_enviada,size-cantidad_total_enviada);
			cantidad_total_enviada = seguir_enviado(socket, data,size-cantidad_total_enviada) + cantidad_total_enviada;
			free(data);
		}
	}

	/*int cantidad_enviada = 0;
	void *data = malloc(size);
	memcpy(data,buffer,size);
	while(cantidad_enviada != size)
	{
		cantidad_enviada=send(socket,data,size,0) + cantidad_enviada;
		if(cantidad_enviada < size)
		{
			memcpy(data,buffer+cantidad_enviada,(size-cantidad_enviada));
		}
	}
	free(data);*/

}

int seguir_enviado(int socket, void *buffer, int size)
{
	int cantidad_enviada;
	cantidad_enviada=send(socket,buffer,size,0);
	return cantidad_enviada;
}

void enviar_mensaje(int socket, char *msg)
{
	send(socket, msg, strlen(msg),0);
}

char* recibir_mensaje(int socket,int payloadSize)
{
	char *payload = malloc(payloadSize+1);
	int bytes_recibidos;
	bytes_recibidos=recv(socket, payload, payloadSize,0);
	if(bytes_recibidos == 0)
	{
		char *desconectado = string_new();
		string_append(&desconectado, "DESCONECTADO");
		return desconectado;
	}
	else
	{
		payload[payloadSize]= '\0';
		return payload;
	}

}

void* recibir_mensaje_tipo_indistinto(int socket,int payloadsize)
{

	void *payload = malloc(payloadsize);
	int bytes_recibidos = 0;
	bytes_recibidos = recv(socket,payload,payloadsize,0);
	if(bytes_recibidos<payloadsize)
	{
		while(bytes_recibidos<payloadsize)
		{
			int tamanio = payloadsize-bytes_recibidos;
			int offset_anterior = bytes_recibidos;
			void *pay_aux = malloc(tamanio);
			bytes_recibidos = recv(socket,pay_aux,tamanio,0) + bytes_recibidos;

			int nuevo_tamanio = bytes_recibidos - offset_anterior;
			memcpy(payload + offset_anterior,pay_aux,nuevo_tamanio);
			free(pay_aux);
		}
	}
	return payload;

	/*void *payload = malloc(payloadsize);//+1
	int bytes_recibidos = 0;
	while(bytes_recibidos<=payloadsize)
	{
		bytes_recibidos = recv(socket,payload,payloadsize,0) + bytes_recibidos;
	}
	if(bytes_recibidos == 0)
		{
			char *desconectado = string_new();
			string_append(&desconectado, "DESCONECTADO");
			return desconectado;
		}
		else
		{
			return payload;
		}*/
}
