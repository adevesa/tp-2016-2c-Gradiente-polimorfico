/*
 * comunication.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "comunication.h"

void decodificar_coordenadas(char *payload, int *x, int*y)
{
	string_trim(&payload);
	char **coordenadas= string_split(payload, ",");
	*x = atoi(coordenadas[0]);
	*y= atoi(coordenadas[1]);
}
