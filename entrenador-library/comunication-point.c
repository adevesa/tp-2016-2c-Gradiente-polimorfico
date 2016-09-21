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
	send(socket, mensaje, strlen(mensaje),0);
	//free(mensaje);
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



