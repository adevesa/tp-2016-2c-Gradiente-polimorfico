/*
 * sockets-manipuling.c
 *
 *  Created on: 30/8/2016
 *      Author: utnso
 */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket-manipuling.h"
#include "string.h"


int leer_socket(int socketCliente, char* buffer, int bytesALeer)
{
	int bytesRecibidos = recv(socketCliente, (void*) buffer, bytesALeer,0);
	return bytesRecibidos;
}

/*
 * LAS QUE DEBEN USAR LOS CHICOS
 */
void recibir_mensaje(int socket, char* buffer, int bytesALeer)
{
	int bytesRecibidos;
	do
	{
		bytesRecibidos = recv(socket, buffer, bytesALeer,0);
	}while (bytesRecibidos < bytesALeer && bytesRecibidos != -1);
}

void enviar_mensaje(int socket, char* buffer, int bytesAEnviar)
{
	int bytesEnviados;
	do
	{
		bytesEnviados = send(socket,buffer, bytesAEnviar,0);
	}while (bytesEnviados < bytesAEnviar);
}

/*
 *
 */

int enviar_a_socket(int socketCliente, char* buffer, int bytesAenviar)
{
	int result = send(socketCliente, buffer, bytesAenviar,0);
	return result;
}

//SOBRE ARRYAS NECESARIOS PARA EL SELECT//
void agregar_elemento_a_array(int *array, int posicion, int elemento)
{
	array[posicion -1] = elemento;
}

void eliminar_elemento_del_array(int *array, int cantidadDeElementos, int elemento)
{
	int i;
	for(i=0; i<cantidadDeElementos; i++)
	{
		if(array[i]==elemento)
		{
			array[i]=0;
		}
	}
}

int mayor_del_array(int *array, int tamanio)
{
	int i;
	int mayor = 0;
	for (i=0; i<(tamanio -1); i++)
		{
		    if (array[i]> mayor) mayor=array[i];
		}
	return mayor;
}

int arrayInt_maximo_entre(int *array, int tamanioArray, int numero)
{
	int mayorDelArray = mayor_del_array(array,tamanioArray);
	if(mayorDelArray>numero)
		return mayorDelArray;
	else	return numero;
}

void inicializarArray(int *array, int tamanio)
{
	int posicion;
	for(posicion=0; posicion<tamanio; posicion++)
	{
		array[posicion]=0;
	}
}

int* arrayInt_create(int tamanio)
{
	int *array = malloc(sizeof(int) * tamanio);
	inicializarArray(array, tamanio);
	return array;
}

