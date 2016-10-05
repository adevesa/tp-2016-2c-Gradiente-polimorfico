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
	//int tamanio_asignaciones = calcular_tamanio_tabla_de_asignaciones(disco->header);
	//printf("El tamaño de la tabla de asignaciones es de %d bloques \n", tamanio_asignaciones);
	//printf("El tamaño de datos es de %d bloques", disco->header->data_blocks);
		//off_t i;
		//int ocupados = 0;
		//int libres = 0;
		//for(i=0; i<disco->header->fs_blocks ; i++)
		{
			//int result = bitarray_test_bit(disco->bitmap, i);
			//if(result == 1)
			{
				//ocupados++;
			}
			//else
			{
				//libres++;
			}
		}
		//printf("El tamaño del disco es %d bytes\n", disco->size);
		//rintf("En total el disco tiene %d bloques\n", (disco->size)/64);
		//printf("El tamaño del header es de 1 bloque\n");
		//printf("El tamaño de la tabla de archivos es de 1024 bloques\n");
		//printf("El tamaño del bitmap es de %d bloques\n", disco->header->bitmap_blocks);
		//printf("El tamaño de la tabla de asignaciones es de %d bloques \n", tamanio_asignaciones);
		//printf("El tamaño de datos es de %d bloques\n", disco->header->data_blocks);
		//printf("Hay %d bloques ocupados y %d bloques libres \n", ocupados,libres);
		//int bits = bitarray_get_max_bit(disco->bitmap);
		//printf("Hay %d bits  y %d bytes en bitmap\n", bits, disco->bitmap->size);

		osada_file *file = osada_get_file_called("large.txt", disco);
		printf("%s\n",file->fname);
		printf("El primer bloque del archivo es %d\n", file->first_block);
		printf("%d\n", file->file_size);

		char *texto = osada_get_data_of_this_file(file, disco);
		printf("%s\n", texto);
	return EXIT_SUCCESS;
}
