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


extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/basic.bin");

	//osada_delete_this_file("archivo.txt");

	int verifica = osada_check_exist("archivo.txt");
	printf("%d\n", verifica);
	//pokedex_server_conectate();
	//pokedex_server_acepta_clientes();

	off_t i;
	int ocupados = 0;
	int libres = 0;
	for(i=0; i<disco->header->fs_blocks ; i++)
	{
		int result = bitarray_test_bit(disco->bitmap, i);
		if(result == 1)
		{
			ocupados++;
		}
		else
		{
			libres++;
		}
	}
	printf("El tamaño del disco es %d bytes\n", disco->size);
	printf("En total el disco tiene %d bloques\n", (disco->size)/64);
	printf("El tamaño del header es de 1 bloque\n");
	printf("El tamaño de la tabla de archivos es de 1024 bloques\n");
	printf("El tamaño del bitmap es de %d bloques\n", disco->header->bitmap_blocks);
	//printf("El tamaño de la tabla de asignaciones es de %d bloques \n", tamanio_asignaciones);
	printf("El tamaño de datos es de %d bloques\n", disco->header->data_blocks);
	printf("Hay %d bloques ocupados y %d bloques libres \n", ocupados,libres);
	int bits = bitarray_get_max_bit(disco->bitmap);
	printf("Hay %d bits  y %d bytes en bitmap\n", bits, disco->bitmap->size);
	return EXIT_SUCCESS;
}
