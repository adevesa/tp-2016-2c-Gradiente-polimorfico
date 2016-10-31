/*
 * osada_generales.c
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */

#include "osada_generales.h"

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
	char *estructura_bitmap = (char*) osada_get_blocks_relative_since(BITMAP,1,disco->header->bitmap_blocks,disco);
	t_bitarray *bitArray_new = bitarray_create_with_mode(estructura_bitmap,disco->header->bitmap_blocks*OSADA_BLOCK_SIZE,LSB_FIRST);
	return bitArray_new;
}

/*----------------------------------------------IMPACTAR CAMBIOS EN DISCO-------------------------------------------------*/
void osada_push_middle_block(int campo, int numero_block_relative, int offset, void *bloque, t_disco_osada *disco)
{
	int byte_inicial = calcular_byte_inicial_relative(campo,numero_block_relative,disco->header);
	impactar_en_disco_medio_bloque(byte_inicial + offset,bloque,disco->map);
}

void osada_push_block(int campo, int numero_block_relative, void *bloque,t_disco_osada *disco)
{
	int byte_inicial = calcular_byte_inicial_relative(campo,numero_block_relative,disco->header);
	impactar_en_disco_bloque_completo(byte_inicial,bloque,disco->map);
}

void impactar_en_disco_bloque_completo(int byte_inicial,void *bloque, void *map)
{
	int i;
	int byte = 0;
	int byte_final = byte_inicial + OSADA_BLOCK_SIZE-1;
	char *block = (char*) bloque;
	char *mapping = (char*) map;
	for(i=byte_inicial; i<=byte_final;i++)
	{
		mapping[i] = block[byte];
		byte++;
	}
}

void impactar_en_disco_medio_bloque(int byte_inicial,void *bytes, void *map)
{
	int i;
	int byte = 0;
	char *block = (char*) bytes;
	int byte_final = byte_inicial + 31;
	char *mapping = (char*) map;
	for(i=byte_inicial; i<=byte_final;i++)
	{
		mapping[i] = block[byte];
		byte++;
	}
}

void impactar_en_disco_n_bloques(int byte_inicial, int cantidad_bloques,void *bloques, void *map)
{
	int i;
	int byte = 0;
	int byte_final = byte_inicial + (OSADA_BLOCK_SIZE)*cantidad_bloques -1;
	char *block = (char*) bloques;
	char *mapping = (char*) map;
	for(i=byte_inicial; i<=byte_final;i++)
	{
			mapping[i] = block[byte];
			byte++;
	}
}

/*----------------------------------------------OBTENCION DE BLOQUES-------------------------------------------------*/
void* osada_get_blocks_relative_since(int campo, int num_block_init, int num_blocks,t_disco_osada *disco)
{
	switch(campo)
	{
		case(HEADER):
			{
				int byte_inicial = calcular_byte_inicial_relative(HEADER, num_block_init,disco->header);
				return osada_get_block_start_in(byte_inicial,num_blocks,disco->map);
			};break;
		case(BITMAP):
			{
				int byte_inicial = calcular_byte_inicial_relative(BITMAP, num_block_init,disco->header);
				void* valor_Retorno = osada_get_block_start_in(byte_inicial,num_blocks,disco->map);
				return valor_Retorno;
			};break;
		case(TABLA_DE_ARCHIVOS):
			{
				int byte_inicial = calcular_byte_inicial_relative(TABLA_DE_ARCHIVOS, num_block_init,disco->header);
				return osada_get_block_start_in(byte_inicial,num_blocks,disco->map);
			};break;
		case(TABLA_DE_ASIGNACIONES):
			{
				int byte_inicial = calcular_byte_inicial_relative(TABLA_DE_ASIGNACIONES, num_block_init,disco->header);
				return osada_get_block_start_in(byte_inicial,num_blocks,disco->map);
			};break;
		case(BLOQUE_DE_DATOS):
			{
				int byte_inicial = calcular_byte_inicial_relative(BLOQUE_DE_DATOS, num_block_init,disco->header);
				return osada_get_block_start_in(byte_inicial,num_blocks,disco->map);
			};break;
	}
}

void* osada_get_block_start_in(int byte_inicial, int num_blocks, void *map)
{
	void *bloc = malloc(OSADA_BLOCK_SIZE * num_blocks);
	memcpy(bloc,map + byte_inicial ,OSADA_BLOCK_SIZE * num_blocks);
	return bloc;
}

void* osada_get_bytes_start_in(int byte_inicial, int num_bytes_total, void *map)
{
	void *bloc = malloc(num_bytes_total);
	memcpy(bloc,map + byte_inicial ,num_bytes_total);
	return bloc;
}

osada_block_pointer calcular_byte_inicial_relative(int campo, int numero_bloque,osada_header *header)
{

	switch(campo)
	{
		case(BITMAP):
			{
					return (OSADA_BLOCK_SIZE*numero_bloque);


			};break;
		case(TABLA_DE_ARCHIVOS):
			{
				int ultimo_byte_bitmap = OSADA_BLOCK_SIZE * header->bitmap_blocks;
				int primer_byte = ultimo_byte_bitmap + OSADA_BLOCK_SIZE*numero_bloque;
				return(primer_byte);
			};break;
		case(TABLA_DE_ASIGNACIONES):
			{
				if(numero_bloque==1)
				{
					return calcular_byte_inicial_absolut(header->allocations_table_offset);
				}
				else
				{
					int primer_byte = calcular_byte_inicial_absolut(header->allocations_table_offset) + OSADA_BLOCK_SIZE*numero_bloque;
					return primer_byte;
				}
			};break;
		case(BLOQUE_DE_DATOS):
			{
				int tamanio_tabla_asignaciones = calcular_tamanio_tabla_de_asignaciones(header);
				int byte_inicial_bd = calcular_byte_inicial_relative(TABLA_DE_ASIGNACIONES,tamanio_tabla_asignaciones,header);

				int primer_byte_del_bloque_pedido = (byte_inicial_bd) +(OSADA_BLOCK_SIZE*numero_bloque);
				return primer_byte_del_bloque_pedido;
			};break;
	}

}

osada_block_pointer calcular_byte_inicial_absolut(int numero_bloque_absoluto)
{
	if(numero_bloque_absoluto == 0)
	{
		return 0;
	}
	else
	{
		int byte_inicial = numero_bloque_absoluto*64;
		return (byte_inicial);
	}
}

int calcular_tamanio_tabla_de_asignaciones(osada_header *header)
{
	int tamanio_tabla_de_asignaciones = header->fs_blocks - HEADER - TABLA_DE_ARCHIVOS - header->bitmap_blocks - header->data_blocks;
	return tamanio_tabla_de_asignaciones;
}

osada_block_pointer osada_get_start_block_absolut_of(int campo, t_disco_osada *disco)
{
	switch(campo)
	{
			case(HEADER):
				{
					return 0;
				};break;
			case(BITMAP):
				{
					return 1;
				};break;
			case(TABLA_DE_ARCHIVOS):
				{
					return (0 + disco->header->bitmap_blocks +1);
				};break;
			case(TABLA_DE_ASIGNACIONES):
				{
					return disco->header->allocations_table_offset;
				};break;
			case(BLOQUE_DE_DATOS):
				{
					return (disco->header->allocations_table_offset + calcular_tamanio_tabla_de_asignaciones(disco->header));
				};break;
		}
}

/*----------------------------------------------MANIPULACION BITARRAY------------------------------------------------------*/
int osada_b_check_is_bitarray_have_n_blocks_free(int n, t_disco_osada *disco)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int i =bloque_incial;
	int contador = 0;
	int hay_n_bit_libre = 1;
	while( contador!=n && i<(bloque_incial + disco->header->data_blocks))
	{
		if(!bitarray_test_bit(disco->bitmap,i))
		{
			contador++;
		}
		i++;
	}
	if(contador < n)
	{
		return 0;
	}
	else
	{
		return hay_n_bit_libre;
	}
}

int osada_b_check_is_bitarray_full(t_disco_osada *disco)
{

	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int i =bloque_incial;
	int hay_un_bit_libre = 1;
	while( hay_un_bit_libre && i<(bloque_incial + disco->header->data_blocks))
	{
		hay_un_bit_libre=bitarray_test_bit(disco->bitmap,i);
		i++;
	}

	return hay_un_bit_libre;
}

int osada_ocupa_bit_libre_de(t_disco_osada *disco)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int i =bloque_incial;
	while(bitarray_test_bit(disco->bitmap,i) && i<(bloque_incial + disco->header->data_blocks))
	{
		i++;
	}
	bitarray_set_bit(disco->bitmap,i);
	impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);
	return i;
}

int calcular_posicion_relativa_en_bloque_de_datos(int posicion_absoluta)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	return (posicion_absoluta- bloque_incial);
}

void osada_desocupa_bit(t_disco_osada *disco, int num_block)
{
	bitarray_clean_bit(disco->bitmap,num_block);
}

void osada_desocupa_n_bits(t_list *bloques_a_liberar)
{
	int size = list_size(bloques_a_liberar);
	int i;
	for(i=0; i<size; i++)
	{
		int *bloque=list_get(bloques_a_liberar,i);
		int bit_a_desocupar = calcular_cantidad_bloques_admin() + *bloque;
		osada_desocupa_bit(disco,bit_a_desocupar);
	}
	impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);
}

int calcular_cantidad_bloques_admin()
{
	return disco->header->bitmap_blocks + 1 +1024 + calcular_tamanio_tabla_de_asignaciones(disco->header);
}


/*---------------------------------------------AUXILIARES----------------------------------------------------------------*/
 int array_size(char **array)
{
	int cantidad_de_elementos = 0;
	int i = 0;
	while(array[i] !=NULL)
	{
		cantidad_de_elementos++;
		i++;
	}
	return cantidad_de_elementos;
}

void array_free_all(char **array)
{
	int i =0;
	while(array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
}

char* array_last_element(char* path)
{
	char **file_for_file = string_split(path, "/");
	int size = array_size(file_for_file);
	char *nombre = string_new();
	string_append(&nombre,file_for_file[size - 1]);
	array_free_all(file_for_file);
 	return nombre;
}

char* crear_ruta(char* hijo, char* path_padre)
{
	char* path_final = string_new();
	string_append(&path_final,path_padre);
	string_append(&path_final,"/");
	string_append(&path_final,hijo);
	return path_final;
}

/*---------------------------------------------DESTROYERS----------------------------------------------------------------*/
void t_file_osada_destroy(t_file_osada *file)
{
	free(file->file);
	//free(file->path);
	free(file);
}

void file_listado_eliminate(t_file_listado* file)
{
	free(file->file->file);
	free(file->file);
	free(file);
}



