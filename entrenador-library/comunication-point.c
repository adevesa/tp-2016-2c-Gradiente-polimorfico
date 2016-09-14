/*
 * comunication-point.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "comunication-point.h"

/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_servidor(int puerto, char *ip)
{
	//Configuro direccion de servidor//
	address_config_in direccionServidor = configurar_address_in(puerto, ip);

	int server = socket(AF_INET,SOCK_STREAM,0);

	//Conexión con el servidor//
	int resultConect = connect(server, (void*) &direccionServidor,sizeof(direccionServidor));
	if(resultConect !=0)
		{perror("Error de conexión");}
	return server;
}

/*-------------------------------------ENVIO DE MENSAJES-----------------------------------------------------------*/
void enviar_mensaje(int socket, char *mensaje)
{
	send(socket, mensaje, strlen(mensaje)+1,0);
	free(mensaje);
}

/*-------------------------------------RECEPCION DE MENSAJES -------------------------------------------------------*/
char* recibir_mensaje(int socket,int payloadSize)
{
	char * payload = malloc(payloadSize);
	recv(socket, payload, payloadSize,0);
	payload[payloadSize-1]= '\0';
	string_trim(&payload);
	return payload;
}

/*-------------------------------------------------------FUNCIONES SECUNDARIAS----------------------------------------------*/
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



