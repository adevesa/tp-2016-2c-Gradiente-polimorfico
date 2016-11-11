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

extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/jejox");
	pokedex_server_conectate();
	servidor_acepta_clientes();


	return EXIT_SUCCESS;
}
