/*
 * global-vars.h
 *
 *  Created on: 6/10/2016
 *      Author: utnso
 */

#ifndef GLOBAL_VARS_H_
#define GLOBAL_VARS_H_
/*-------------------------------------------STRUCT CLIENTE-----------------------------------------------------------*/
typedef struct
{
	int puerto;
	char* ip;
	int socket_pokedex_servidor;
}t_cliente_osada;

t_cliente_osada* cliente_osada;



#endif /* GLOBAL_VARS_H_ */
