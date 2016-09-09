/*
 * socket-manipuling.h
 *
 *  Created on: 30/8/2016
 *      Author: utnso
 */

#ifndef SOCKET_MANIPULING_H_
#define SOCKET_MANIPULING_H_

typedef struct sockaddr_in address_config_in;
int leer_socket(int socketCliente, char* buffer, int bytesALeer);
int enviar_a_socket(int socketCliente, char* buffer, int bytesAenviar);

//PROTOTIPOS PARA ARRAYS NECESARIOS PARA EL SELECT//
void inicializarArray(int *array, int tamanio);
void eliminar_elemento_del_array(int *array, int cantidadDeElementos, int elemento);
int mayor_del_array(int *array, int tamanio);
int arrayInt_maximo_entre(int *array, int tamanioArray, int numero);
void agregar_elemento_a_array(int *array, int posicion, int elemento);
int* arrayInt_create(int tamanio);

#endif /* SOCKET_MANIPULING_H_ */
