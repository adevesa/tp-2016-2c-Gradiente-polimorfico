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

int main(int argc, char* argv[])
{
	t_disco_osada *disco = osada_disco_abrite("/home/utnso/workspace/basic.bin");

	pokedex_server_conectate();
	pokedex_server_acepta_clientes();

	osada_file *file = osada_get_file_called("large.txt", disco);
	printf("%s\n",file->fname);
	printf("El primer bloque del archivo es %d\n", file->first_block);
	printf("%d\n", file->file_size);

	char *texto = osada_get_data_of_this_file(file, disco);
	printf("%s\n", texto);
	return EXIT_SUCCESS;
}
