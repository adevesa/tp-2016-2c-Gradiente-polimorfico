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
	char *estructura_bitmap = (char*) osada_get_blocks_relative_since(BITMAP,1,disco->header->bitmap_blocks,disco);
	t_bitarray *bitArray_new = bitarray_create_with_mode(estructura_bitmap,disco->header->bitmap_blocks*OSADA_BLOCK_SIZE,LSB_FIRST);
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
	int byte_final = byte_inicial + OSADA_BLOCK_SIZE-1;
	char *block = (char*) bloque;
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

/*----------------------------------------------OBTENCION DE NUM BLOQUES ARCHIVO-----------------------------------------*/
t_list* osada_get_blocks_nums_of_this_file(osada_file *file, t_disco_osada *disco)
{
	t_list *list_blocks = list_create();
	osada_block_pointer before_block = file->first_block;
	osada_block_pointer byte_inicial_tabla_asignaciones = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);

	list_add(list_blocks,&file->first_block);

	int hay_mas_para_leer = 1;
	while(hay_mas_para_leer)
	{
		int *block = osada_get_bytes_start_in(byte_inicial_tabla_asignaciones + 4*before_block,sizeof(int),disco->map);
		if(*block == FEOF)
		{
			hay_mas_para_leer=0;
		}
		else
		{
			list_add(list_blocks, block);
			before_block = *block;
		}
	}
	return list_blocks;
}

/*----------------------------------------------OBTENCION DE DATOS DE UN ARCHIVO---------------------------------------*/
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco)
{
	t_list *bloques_por_recuperar = osada_get_blocks_nums_of_this_file(file, disco);
	int size = list_size(bloques_por_recuperar);

	int last_block = size -1;
	char *data = string_new();
	int i;
	for(i=0; i<(size); i++)
	{
		int *block_num = list_get(bloques_por_recuperar,i);
		void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
		if(i == last_block)
		{
			int ultimo_byte_a_recibir = calcular_byte_final_a_recuperar_de_file(file->file_size);
			void *data_last = osada_get_bytes_start_in(0,ultimo_byte_a_recibir,data_recv);
			string_append(&data, (char*) data_last);
			free(data_last);
			//list_remove_and_destroy_element(bloques_por_recuperar,i,nada);
		}
		else
		{
			string_append(&data, (char*) data_recv);
			free(data_recv);
			//list_remove_and_destroy_element(bloques_por_recuperar,i,nada);
		}
	}
	list_destroy(bloques_por_recuperar);
	return( (void*) data);
}

void nada (void *nada)
{

}

int calcular_byte_final_a_recuperar_de_file(int file_size)
{
	if(file_size < OSADA_BLOCK_SIZE)
	{
		return file_size -1;
	}
	else
	{
		int bloques_completos = (int) floor(file_size / OSADA_BLOCK_SIZE);
		int byte_final = file_size -  OSADA_BLOCK_SIZE * bloques_completos;
		return byte_final;
	}
}

/*----------------------------------------------OBTENCION DE UN ARCHIVO ESPECIFICO---------------------------------------*/
osada_file* osada_get_file_called(char *file_name, t_disco_osada *disco)
{
	int archivo_encontrado = 0;
	int archivo;
	int index = 1;

	osada_file *file_1 = malloc(sizeof(osada_file));
	osada_file *file_2 = malloc(sizeof(osada_file));


	while(!archivo_encontrado)
	{
		void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
		memcpy(file_1,two_files, sizeof(osada_file));
		memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
		if(verificar_si_es_archivo_buscado(file_name, file_1))
		{
			archivo_encontrado = 1;
			archivo = 1;
			free(two_files);
		}
		else
		{
			if(verificar_si_es_archivo_buscado(file_name,file_2))
			{
				archivo_encontrado = 1;
				archivo = 2;
				free(two_files);
			}
		}
		index++;
	}
	if(archivo == 1)
	{
		free(file_2);
		return file_1;
	}
	else
	{
		free(file_1);
		return file_2;
	}
}


int verificar_si_es_archivo_buscado(char *file_name, osada_file *file)
{
	return(string_equals_ignore_case(file->fname, file_name));
}

/*----------------------------------------------MANIPULACION BITARRAY-------------------------------------------------*/
int osada_ocupa_bit_libre(t_disco_osada *disco)
{
	int i =0;
	while(bitarray_test_bit(disco->bitmap,i) && i<disco->header->fs_blocks)
	{
		i++;
	}
	bitarray_set_bit(disco->bitmap,i);
	return i;
}
