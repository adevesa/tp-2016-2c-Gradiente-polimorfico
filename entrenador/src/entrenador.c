/*
 ============================================================================
 Name        : entrenador.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "entrenador-commons.h"

int main(int argc, char *argv[])
{
	ejecutar_entrenador("ash","/home/utnso/Pokedex");
	/*char *prueba= string_new();
	string_append(&prueba,"/home/utnso/Pokedex/Entrenadores/ash/Dir de Bill");
	char *aux=string_path_replace_spaces(&prueba," ","\\ ");
	printf("%s",aux);*/
	return EXIT_SUCCESS;
}
