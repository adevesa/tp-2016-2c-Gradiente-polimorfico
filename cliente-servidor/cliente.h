/*
 * cliente.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef CLIENTE_H_
#define CLIENTE_H_
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/config.h>
#include "string.h"

#define TRUE 1
#define FALSE 0

/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_mapa(char *mapa, char *rutaPokedex);
int conectar_a_servidor(int puerto, char *ip);

/*--------------------------------------MENSAJES QUE RECIBE EL ENTRENADOR----------------------------------------*/
int esperar_turno(int server); //<-- si contesta 1, entonces turno concedido.
char* esperar_coordenadas_pokenest(int server);
char* esperar_por_pokemon(int server);
char* esperar_por_medalla(int server);
int esperar_por_desbloqueo(int server);
char* esperar_respuesta_captura_pokemon(int server);

/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR----------------------------------------*/
void solicitar_ubicacion_pokenest(int server,char *pokemonBuscado);
void solicitar_moverse(int server,char *coordenadaDestino);
void solicitar_captura_pokemon(int server, char *pokemonAcapturar);
void notificar_fin_objetivos(int server);

/*-------------------------------------------------------FUNCIONES SECUNDARIAS-------------------------------*/
typedef struct sockaddr_in address_config_in;
address_config_in configurar_address_in(int puerto, char *ip);
void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket);
char* armar_mensaje(char *header, char *payload);
char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo);

#endif /* CLIENTE_H_ */
