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

extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/basic.bin");

		t_list *listado = osada_b_listar_hijos("/directorio");
		int size = list_size(listado);
		int i;
		for(i=0; i<size; i++)
		{
			t_file_listado *file = list_get(listado,i);
			printf("%s\n", file->path);
		}

		//osada_delete_this_dir("/directorio/subdirectorio");

		int exist = osada_check_exist("/directorio/subdirectorio/large.txt");
		printf("%d", exist);


	return EXIT_SUCCESS;
}
