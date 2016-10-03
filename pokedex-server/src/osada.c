/*
 * osada.c
 *
 *  Created on: 2/10/2016
 *      Author: utnso
 */
#include "osada.h"

t_disco_osada* osada_disco_abrite(char *ruta)
{
	t_disco_osada *disco_new = malloc(sizeof(t_disco_osada));
	disco_new->ruta = ruta;
	disco_new->size = disco_dame_tu_tamanio(ruta);
	disco_new->file_descriptor = disco_dame_tu_descriptor(ruta);
	disco_new->map = disco_dame_mapping(disco_new->size, disco_new->file_descriptor);
	disco_new->header = osada_header_create(disco_new->map);
	disco_new->bitmap = osada_bitmap_create(disco_new);
	return disco_new;
}

/*-------------------------------------------------------CREATES Y RECUPEROS-----------------------------------------------*/
int disco_dame_tu_descriptor(char *ruta)
{
	int file_descriptor = open(ruta,O_RDWR);
	return file_descriptor;
}

void* disco_dame_mapping(int size, int file_descriptor)
{
	void *block_pointer = mmap((caddr_t)0,(size_t)size,PROT_READ | PROT_WRITE, MAP_SHARED,file_descriptor,0);
	return block_pointer;
}

int disco_dame_tu_tamanio(char *ruta)
{
	FILE *disco = fopen(ruta, "r");
	fseek(disco, 0L, SEEK_END); // <-- Pongo el puntero del archivo en el ultimo byte
	int tamanio_archivo = ftell(disco);
	fseek(disco, 0L, SEEK_SET); // <-- Pongo el puntero del archivo en el inicio nuevamente
	fclose(disco);
	return tamanio_archivo;
}

osada_header* osada_header_create(void *map)
{
	osada_header *osada_header_new = malloc(sizeof(osada_header));
	memcpy(osada_header_new,map,OSADA_BLOCK_SIZE);
	return osada_header_new;
}

t_bitarray* osada_bitmap_create(t_disco_osada *disco)
{
	char *estructura_bitmap = malloc(disco->header->bitmap_blocks *OSADA_BLOCK_SIZE);
	int i;
	for(i=1; i<=disco->header->bitmap_blocks; i++)
	{
		estructura_bitmap = (char*) osada_get_block_relative(BITMAP,i,disco);
	}
	t_bitarray *bitArray_new = bitarray_create(estructura_bitmap, disco->header->bitmap_blocks*OSADA_BLOCK_SIZE);
	return bitArray_new;
}

/*----------------------------------------------IMPACTAR CAMBIOS EN UN BLOQUE-------------------------------------------------*/
void osada_push_block(int campo, int numero_block_relative, void *bloque,t_disco_osada *disco)
{
	int byte_inicial = calcular_byte_inicial_relative(campo,numero_block_relative,disco->header);
	impactar_en_disco(byte_inicial,bloque,disco->map);
}

void impactar_en_disco(int byte_inicial,void *bloque, void *map)
{
	int i;
	int byte = 0;
	int byte_final = byte_inicial + OSADA_BLOCK_SIZE;
	char *block = (char*) bloque;
	char *mapping = (char*) map;
	for(i=byte_inicial; i<=byte_final;i++)
	{
		mapping[i] = block[byte];
		byte++;
	}
}
/*----------------------------------------------OBTENCION DE BLOQUES-------------------------------------------------*/

void* osada_get_block_relative(int campo, int num_block, t_disco_osada *disco)
{
	switch(campo)
	{
		case(HEADER):
			{
				int byte_inicial = calcular_byte_inicial_relative(HEADER, num_block,disco->header);
				return osada_get_block_start_in(byte_inicial,disco->map);
			};break;
		case(BITMAP):
			{
				int byte_inicial = calcular_byte_inicial_relative(BITMAP, num_block,disco->header);
				return osada_get_block_start_in(byte_inicial,disco->map);
			};break;
		case(TABLA_DE_ARCHIVOS):
			{
				int byte_inicial = calcular_byte_inicial_relative(TABLA_DE_ARCHIVOS, num_block,disco->header);
				return osada_get_block_start_in(byte_inicial,disco->map);
			};break;
		case(TABLA_DE_ASIGNACIONES):
			{
				int byte_inicial = calcular_byte_inicial_relative(TABLA_DE_ASIGNACIONES, num_block,disco->header);
				return osada_get_block_start_in(byte_inicial,disco->map);
			};break;
		case(BLOQUE_DE_DATOS):
			{
				int byte_inicial = calcular_byte_inicial_relative(BLOQUE_DE_DATOS, num_block,disco->header);
				return osada_get_block_start_in(byte_inicial,disco->map);
			};break;
	}
}

void* osada_get_block_start_in(int byte_inicial, void *map)
{
	char *mapping = (char*) map;
	char *bloc = malloc(OSADA_BLOCK_SIZE);
	int i;
	for(i=0; i<OSADA_BLOCK_SIZE; i++)
	{
		bloc[i] = mapping[byte_inicial];
	}
	return bloc;
}


osada_block_pointer calcular_byte_inicial_relative(int campo, int numero_bloque,osada_header *header)
{

	switch(campo)
	{
		case(BITMAP):
			{
				return (OSADA_BLOCK_SIZE*numero_bloque + (numero_bloque -1));
			};break;
		case(TABLA_DE_ARCHIVOS):
			{
				int primer_byte_bloque = (header->bitmap_blocks + 1)*OSADA_BLOCK_SIZE + OSADA_BLOCK_SIZE*(numero_bloque-1) + (numero_bloque+1);
				return(primer_byte_bloque);
			};break;
		case(TABLA_DE_ASIGNACIONES):
			{
				if(numero_bloque == 1)
				{
					int primer_byte_bloque = ((header->allocations_table_offset)-1)*OSADA_BLOCK_SIZE + header->allocations_table_offset -2;
					return primer_byte_bloque;
				}
				else
				{
					int primer_byte_bloque = ((header->allocations_table_offset + numero_bloque -1)-1)*OSADA_BLOCK_SIZE + (header->allocations_table_offset +numero_bloque -1) -2;
					return primer_byte_bloque;
				}

			};break;
		case(BLOQUE_DE_DATOS):
			{
				int tamanio_tabla_asignaciones = calcular_tamanio_tabla_de_asignaciones(header);
				int byte_inicial_del_ultimo_bloque_tabla_de_asignaciones = calcular_byte_inicial_relative(TABLA_DE_ASIGNACIONES,tamanio_tabla_asignaciones,header);

				int primer_byte_del_bloque_pedido = (byte_inicial_del_ultimo_bloque_tabla_de_asignaciones + OSADA_BLOCK_SIZE +1) +65*(numero_bloque-1);
				return primer_byte_del_bloque_pedido;
			};break;
	}

}

osada_block_pointer calcular_byte_inicial_absolut(int numero_bloque_absoluto)
{
	if(numero_bloque_absoluto == 1)
	{
		return 0;
	}
	else
	{
		int byte_inicial = (numero_bloque_absoluto -1)*OSADA_BLOCK_SIZE + (numero_bloque_absoluto-2);
		return (byte_inicial);
	}
}

int calcular_tamanio_tabla_de_asignaciones(osada_header *header)
{
	int tamanio_tabla_de_asignaciones = header->fs_blocks - HEADER - TABLA_DE_ARCHIVOS - header->bitmap_blocks - header->data_blocks;
	return tamanio_tabla_de_asignaciones;
}
