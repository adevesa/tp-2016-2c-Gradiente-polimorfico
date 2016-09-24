/*
 * send&recv.c
 *
 *  Created on: 24/9/2016
 *      Author: utnso
 */

#include "sendYrecv.h"
/*-------------------------------------ENVIO DE MENSAJES-----------------------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje)
{
	send(socket, mensaje, strlen(mensaje),0);
}

char* armar_mensaje(char *header, char *payload, int MAX_BYTES_TOTAL_A_ENVIAR)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	int tamanio_payload = strlen(payload);
	if(tamanio_payload < MAX_BYTES_TOTAL_A_ENVIAR)
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
		string_append(&mensaje, string_itoa(MAX_BYTES_TOTAL_A_ENVIAR));
		string_append(&mensaje,";");
		string_append(&mensaje,payload);
		return mensaje;
	}
}
/*-------------------------------------RECEPCION DE MENSAJES -------------------------------------------------------*/
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

