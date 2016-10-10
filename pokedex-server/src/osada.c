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
void* osada_get_file_called(char *file_name, t_disco_osada *disco)
{
	int archivo_encontrado = 0;
	int archivo;
	int index = 1;

	osada_file *file_1 = malloc(sizeof(osada_file));
	osada_file *file_2 = malloc(sizeof(osada_file));

	while(!archivo_encontrado && index<=1024)
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
	if(archivo_encontrado == 0)
	{
		return "NO_EXISTE";
	}
	else
	{
		t_file_osada *file = malloc(sizeof(t_file_osada));
		switch(archivo)
		{
			case(1):
			{
				free(file_2);
				file->file = file_1;
				file->block_relative = index -1;
				file->position_in_block = 0;
				return file;
			}
			case(2):
			{
				free(file_1);
				file->file = file_2;
				file->block_relative = index -1;
				file->position_in_block = 1;
				return file;
			}
		}
	}

}

int verificar_si_es_archivo_buscado(char *file_name, osada_file *file)
{
	return(string_equals_ignore_case(file_name, (char*)file->fname));
}

/*---------------------------------------------BUSUQEDA DE TABLA DE ARCHIVOS DISPONIBLE---------------------------------*/
t_osada_file_free* osada_file_table_get_space_free(t_disco_osada *disco)
{
	int index = 1;

	osada_file *file_1 = malloc(sizeof(osada_file));
	osada_file *file_2 = malloc(sizeof(osada_file));

	int file_free = 0;

	t_osada_file_free *a_file_free = malloc(sizeof(t_osada_file_free));

	while(!file_free)
	{
		void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
		memcpy(file_1,two_files, sizeof(osada_file));
		memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));

		if(verify_file_state(DELETED,file_1))
		{
				file_free = 1;
				free(two_files);
				free(file_2);
				a_file_free->file = file_1;
				a_file_free->block_relative = index;
				a_file_free->position_in_block = 0;
		}
		else
		{
			if(verify_file_state(DELETED,file_2))
			{
				file_free = 1;
				free(two_files);
				free(file_1);
				a_file_free->file = file_2;
				a_file_free->block_relative = index;
				a_file_free->position_in_block = 32;
			}
		}
		index++;
	}
	return a_file_free;
}

int verify_file_state(int state,osada_file *file)
{
	if(file->state == state)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*---------------------------------------------VERIFICACION EXISTENCIA DE UN PATH-----------------------------------------*/
int osada_check_exist(char *path)
{
	char **file_for_file = string_split(path,"/");
	int size = array_size(file_for_file);
	int i = 0;
	int verify = 1;

	int result = 0;

	while(verify && i<size)
	{
		if(i == 0)
		{
			result = verificar_existencia(file_for_file[i],-1);
			verify = revisar_resultado(result);
		}
		else
		{
			result = verificar_existencia(file_for_file[i],result);
			verify = revisar_resultado(result);
		}
		i++;
	}
	array_free_all(file_for_file);
	return verify;
}

int verificar_existencia(char *file_or_directory, uint16_t dad_block)
{
	void *result = osada_get_file_called(file_or_directory, disco);

	t_file_osada *file = (t_file_osada *) result;

	if(string_equals_ignore_case((char*) result, "NO_EXISTE") || verify_file_state(DELETED,file->file))
	{
		return -1;
	}
	else
	{

		int satisfy = -1;

		if(dad_block == 65535)
		{
			satisfy = calcular_posicion_en_tabla_de_archivos(file->block_relative,file->position_in_block);
			free(file->file);
			free(file);
			return satisfy;
		}
		else
		{
			if(file->file->parent_directory == dad_block)
			{
				satisfy = calcular_posicion_en_tabla_de_archivos(file->block_relative, file->position_in_block);
				free(file->file);
				free(file);
				return satisfy;
			}
			else
			{
				free(file->file);
				free(file);
				return -1;
			}
		}

	}
}

int revisar_resultado(int result)
{
	if(result == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int calcular_posicion_en_tabla_de_archivos(int num_block, int position)
{
	switch(num_block)
	{
		case(1):
		{
			return position;
		};break;
		case(2):
		{
			return position + 2;
		};break;
		default:
		{
			if(position == 0)
			{
				return (num_block + num_block -2);
			}
			else
			{
				return (num_block + num_block -1);
			}
		}
	}
}

/*---------------------------------------------BORRADO DE ARCHIVOS---------------------------------------------------------*/
void osada_delete_this_file(char *path)
{
	char **file_for_file = string_split(path, "/");
	int size = array_size(file_for_file);
	delete_file(file_for_file[size-1]);
	array_free_all(file_for_file);
}

void delete_file(char *archivo)
{
	t_file_osada *a_file = osada_get_file_called(archivo, disco);
	t_list *bloques_a_liberar = osada_get_blocks_nums_of_this_file(a_file->file,disco);

	osada_desocupa_n_bits(bloques_a_liberar);
	list_destroy(bloques_a_liberar);

	osada_change_file_state(a_file->file,DELETED);
	int offset = calcular_desplazamiento_tabla_de_archivos(a_file->position_in_block);
	osada_push_middle_block(TABLA_DE_ARCHIVOS,a_file->block_relative,offset,a_file->file,disco);

	free(a_file->file);
	free(a_file);
}

int calcular_desplazamiento_tabla_de_archivos(int posicion_relativa)
{
	if(posicion_relativa == 0)
	{
		return 0;
	}
	else
	{
		return 32;
	}
}

void osada_change_file_state(osada_file *file, osada_file_state new_state)
{
	switch(new_state)
	{
		case(DELETED):
		{
			file->state = DELETED;
		};break;
		case(REGULAR):
		{
			file->state = REGULAR;
		};break;
		case(DIRECTORY):
		{
			file->state = DIRECTORY;
		};break;
	}
}

/*----------------------------------------------MANIPULACION BITARRAY-------------------------------------------------*/
int osada_ocupa_bit_libre_de(t_disco_osada *disco)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int i =bloque_incial;
	while(bitarray_test_bit(disco->bitmap,i) && i<(bloque_incial + disco->header->data_blocks))
	{
		i++;
	}
	bitarray_set_bit(disco->bitmap,i);
	//impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);
	return i;
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

/*------------------------------------------DELETE DIR----------------------------------------------------------------*/



void borrar_archivo(char* path, char*nombre_archivo)
{
	char* path_nuevo = string_new();
	string_append(&path_nuevo, path);
	string_append(&path_nuevo,"/");
	string_append(&path_nuevo, nombre_archivo);
	osada_delete_this_file(path_nuevo);
	free(path_nuevo);
}


void borrar_directorio(char* path, char* nombre_directorio)
{
	char* path_nuevo = string_new();
	string_append(&path_nuevo, path);
	string_append(&path_nuevo,"/");
	string_append(&path_nuevo, nombre_directorio);
	osada_delete_this_dir(path_nuevo);
	free(path_nuevo);
}


void osada_borrar_hijos(char* path)
{
 osada_file *file_1 = malloc(sizeof(osada_file));
 osada_file *file_2 = malloc(sizeof(osada_file));
 int index = 1;

 while(index<=1024)
 	 {
	 void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
	 memcpy(file_1,two_files, sizeof(osada_file));
	 memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));

	 if(es_el_padre(file_1, path)){
	  if(file_1->state == DIRECTORY){
		  borrar_directorio(path, (char*) file_1->fname);
	  	  }
	  else{
		  borrar_archivo(path ,(char*) file_1->fname);
	  	  }
	 }

if(es_el_padre(file_2, path)){
	if(file_2->state == DIRECTORY){
		borrar_directorio(path, (char*)file_2->fname);
	}
	else{
		borrar_archivo(path ,(char*)file_2->fname);
		}
	}
}
index++;
osada_delete_dir_void(path);
free(file_1);
free(file_2);
}

void osada_delete_dir_void(char* path)
{
	int index = 1;
	osada_file *file_1 = malloc(sizeof(osada_file));
 	osada_file *file_2 = malloc(sizeof(osada_file));
 	while(index<=1024)
 		{
 			void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
 			memcpy(file_1,two_files, sizeof(osada_file));
 			memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
 			if(string_equals_ignore_case(nombre_en_el_path(path), (char*)file_1->fname)){
 				file_1->state = DELETED;
 				osada_push_middle_block(TABLA_DE_ARCHIVOS, index, 0, file_1, disco);
 				}
 			if(string_equals_ignore_case(nombre_en_el_path(path), (char*)file_2->fname)){
 				file_2->state = DELETED;
 				osada_push_middle_block(TABLA_DE_ARCHIVOS, index, 32, file_2, disco);
 				}
 		}
 	free(file_1);
 	free(file_2);
}

void nuevo_tamanio_padre(int posicion, char* path)
{
	int tamanio_del_hijo = tamanio_del_dir(path);
	char* nombre_hijo = string_new();
	nombre_hijo = nombre_en_el_path(path);
	osada_file *file_1 = malloc(sizeof(osada_file));
 	osada_file *file_2 = malloc(sizeof(osada_file));
 	void* two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS, posicion, 1, disco);
 	memcpy(file_1,two_files, sizeof(osada_file));
 	memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
 	if(es_el_hijo(file_1, path)){
	file_1->file_size =- tamanio_del_hijo;
	osada_push_middle_block(TABLA_DE_ARCHIVOS, posicion, 0, file_1, disco);
 	}
 	else{
 		file_2->file_size =- tamanio_del_hijo;
 		osada_push_middle_block(TABLA_DE_ARCHIVOS, posicion, 32, file_2, disco);
 	}
 	free(nombre_hijo);
 	free(file_1);
	free(file_2);
}



char* nombre_en_el_path(char* path)
{
	char **file_for_file = string_split(path, "/");
	int size = array_size(file_for_file);
 	return file_for_file[size-1];
}

int es_el_padre(osada_file* file_hijo,char* path_padre)
{
	uint16_t posicion = file_hijo->parent_directory;
	osada_file *file_1 = malloc(sizeof(osada_file));
	osada_file *file_2 = malloc(sizeof(osada_file));
	void* two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS, posicion, 1, disco);
	memcpy(file_1,two_files, sizeof(osada_file));
	memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
	if(string_equals_ignore_case(nombre_en_el_path(path_padre),(char*) file_1->fname) || string_equals_ignore_case(nombre_en_el_path(path_padre),(char*) file_2->fname)){
			free(file_1);
			free(file_2);
			return 1;
		}
		else{
	free(file_1);
	free(file_2);
	return 0;
		}
}

int es_el_hijo(osada_file* file_padre, char* path_hijo)
{
	char* nombre_padre = (char*) file_padre->fname;
	char* path_nuevo = string_new();
	char* nombre_hijo = string_new();
	nombre_hijo = nombre_en_el_path(path_hijo);
	string_append(&path_nuevo, nombre_padre);
	string_append(&path_nuevo, "/");
	string_append(&path_nuevo, nombre_hijo);
		if(string_ends_with(path_hijo, path_nuevo)){
			free(path_nuevo);
			free(nombre_hijo);
			return 1;
		}
		else{
			free(path_nuevo);
			free(nombre_hijo);
			return 0;
		}

}


/*
int es_el_padre(osada_file* file,char* path){
	if(string_starts_with(path, nombre_en_el_path((char*) file->fname))){
		return 0;
		}
	else{
		return 1;
	}
}
*/
void recalcular_tamanio_del_padre(char* path){
	uint16_t posicion = posicion_del_padre(path);
	nuevo_tamanio_padre(posicion, path);
	}

int tamanio_del_dir(char* path)
{
	char* nombre = string_new();
	nombre = nombre_en_el_path(path);
	int index = 1;

	osada_file *file_1 = malloc(sizeof(osada_file));
 	osada_file *file_2 = malloc(sizeof(osada_file));

 	while(index<=1024){
	 void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
	 memcpy(file_1,two_files, sizeof(osada_file));
	 memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
		if(string_equals_ignore_case(nombre_en_el_path(path), (char*) file_1->fname)){
		return file_1->file_size;
		}
	  if(string_equals_ignore_case(nombre_en_el_path(path), (char*) file_2->fname)){
		return file_2->file_size;
	  }

	  free(nombre);
	  free(file_1);
	  free(file_2);
 	}
}

uint16_t posicion_del_padre(char* path)
{
	char* nombre = string_new();
	nombre = nombre_en_el_path(path);
	int index = 1;
	osada_file *file_1 = malloc(sizeof(osada_file));
 	osada_file *file_2 = malloc(sizeof(osada_file));

 	while(index<=1024)
 	{
	  void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
	  memcpy(file_1,two_files, sizeof(osada_file));
	  memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
		if(string_equals_ignore_case(nombre_en_el_path(path), (char*)file_1->fname)){
		return file_1->parent_directory;
		}
	  	if(string_equals_ignore_case(nombre_en_el_path(path), (char*)file_2->fname)){
		return file_2->parent_directory;
	  	}
	free(nombre);
	free(file_1);
	free(file_2);
 	}
}



void osada_delete_this_dir(char* path)
{
	if(!osada_check_exist(path)){
		//logearerror();
	}
	else{
		osada_borrar_hijos(path);
		recalcular_tamanio_del_padre(path);

	//CHARLAR LO DE DESALOJO DE BITARRAY.
		}
}



