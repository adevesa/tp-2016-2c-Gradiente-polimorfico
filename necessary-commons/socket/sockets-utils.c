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

void enviar_mensaje(int socket, char *mensaje)
{
	send(socket, mensaje, strlen(mensaje),0);
}

char* recibir_mensaje(int socket,int payloadSize)
{
	char * payload = malloc(payloadSize+1);
	int bytes_recibidos = 0;
	while(bytes_recibidos !=payloadSize)
	{
		bytes_recibidos=recv(socket, payload, payloadSize,0);
	}
	payload[payloadSize]= '\0';
	return payload;
}