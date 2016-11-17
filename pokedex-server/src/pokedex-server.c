/*
 ============================================================================
 Name        : pokedex-server.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "osada.h"
#include "file_manipuling.h"
#include "ls_and_atributes.h"
#include "osada_generales.h"
#include "borrados.h"
#include "comunication.h"
#include "semaphore.h"

sem_t semaforo_terminacion;

extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/little");
	disco->cantidad_archivos_libres=disco_recupera_cantidad_Archivos();
	sem_init(&semaforo_terminacion,0,0);
	iniciar_semaforos();
	ejecutar_servidor();
	sem_wait(&semaforo_terminacion);

	return EXIT_SUCCESS;
}
