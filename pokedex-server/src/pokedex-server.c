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
#include "osada_generales.h"
#include "borrados.h"

extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/basic.bin");

	/*t_list *listado = osada_listar_hijos("/directorio/subdirectorio");
		int size = list_size(listado);
		int i;
		for(i=0; i<size; i++)
		{
			t_file_listado *file = list_get(listado,i);
			printf("%s\n", file->path);
		}*/

	int size = osada_b_calculate_size_of_directory("/directorio");
	printf("%d",size);


	/*t_list *listado2 = osada_listar_hijos("/directorio/subdirectorio");
	int sizee = list_size(listado2);
	int ii;
	for(ii=0; ii<sizee; ii++)
	{
		t_file_listado *file = list_get(listado2,ii);
		printf("%s\n", file->path);
	}*/

	//int verifica = osada_check_exist("/pokedex");
	//printf("%d \n",verifica);

	//t_file_osada *file = osada_get_file_called("/directorio/archivo.txt",disco);
	//char * text = osada_get_data_of_this_file(file->file,disco);
	//printf("%s",text);
	/*int libres = 0;
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
	*/
	return EXIT_SUCCESS;
}
