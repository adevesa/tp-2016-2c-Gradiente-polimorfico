/*
 * operaciones.c
 *
 *  Created on: 2/10/2016
 *      Author: utnso
 */
#include "operaciones.h"
/*-------------------------------------------ATRIBUTOS------------------------------------------------------------------*/
void* osada_a_get_attributes(char *path)
{
	if(string_equals_ignore_case(path,"/"))
	{
		t_attributes_file *atributos = malloc(sizeof(t_attributes_file));
		atributos->tipo=2;
		atributos->size = osada_b_calculate_size_of_directory(path);
		return atributos;
	}
	else
	{
		if(osada_check_exist(path))
		{
			t_attributes_file *atributos = osada_b_get_attributes_of_this_file(path);
			return atributos;
		}
		else
		{
			return NO_EXISTE;
		}
	}
}

void* osada_a_get_list_dir(char *path)
{
	if(string_equals_ignore_case(path,"/"))
	{
		t_list *listado = osada_b_listar_hijos(path);
		return listado;
	}
	else
	{
		if(osada_check_exist(path))
			{
				t_list *listado = osada_b_listar_hijos(path);
				return listado;
			}
		else
		{
			return NO_EXISTE;
		}
	}

}

/*-------------------------------------------CREACION-------------------------------------------------------------------*/
void* osada_a_create_file(char *path)
{
	if(osada_b_check_name(path))
	{
		if(!osada_b_check_repeat_name(REGULAR,path))
		{
			if(!osada_b_check_is_bitarray_full(disco))
			{
				t_osada_file_free *new_file=osada_b_file_create(REGULAR,path);
				int offset = calcular_desplazamiento_tabla_de_archivos(new_file->position_in_block);
				osada_push_middle_block(TABLA_DE_ARCHIVOS,new_file->block_relative,offset,new_file->file,disco);
				t_file_osada_destroy((t_file_osada*) new_file);
				return EXITO;
			}
			else
			{
				return NO_HAY_ESPACIO;
			}
		}
		else
		{
			return EXISTE;
		}
	}
	else
	{
		return ARGUMENTO_INVALIDO;
	}
}

void* osada_a_create_dir(char *path)
{
	if(osada_b_check_name(path))
	{
		if(!osada_b_check_repeat_name(DIRECTORY,path))
		{
			t_osada_file_free *new_file=osada_b_file_create(DIRECTORY,path);
			int offset = calcular_desplazamiento_tabla_de_archivos(new_file->position_in_block);
			osada_push_middle_block(TABLA_DE_ARCHIVOS,new_file->block_relative,offset,new_file->file,disco);
			t_file_osada_destroy((t_file_osada*) new_file);
			return EXITO;
		}
		else
		{
			return EXISTE;
		}
	}
	else
	{
		return ARGUMENTO_INVALIDO;
	}
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
void* osada_a_delete_file(char *path)
{
	if(osada_check_exist(path))
	{
		osada_delete_this_file(path);
		return EXITO;
	}
	else
	{
		return NO_EXISTE;
	}
}

void* osada_a_delete_dir(char *path)
{
	if(osada_check_exist(path))
	{
		osada_delete_this_dir(path);
		return EXITO;
	}
	else
	{
		return NO_EXISTE;
	}
}

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
void* osada_a_read_file(t_to_be_read *to_read)
{
	if(osada_check_exist(to_read->path))
	{
		t_file_osada *file_a_leer = osada_get_file_called(to_read->path,disco);
		int size = file_a_leer->file->file_size;
		if(size == 0)
		{
			t_file_osada_destroy(file_a_leer);
			return ARGUMENTO_INVALIDO;
		}
		else
		{
			read_content *read = malloc(sizeof(read_content));
			if(size< to_read->size)
			{
				void *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);
				void *contenido_a_enviar = malloc(size); //size +1
				memcpy(contenido_a_enviar,contenido + to_read->offset,size);
				free(contenido);
				t_file_osada_destroy(file_a_leer);
				read->contenido = contenido_a_enviar;
				read->tamanio = size;
				return read;
			}
			else
			{
				/*void *contenido = osada_b_read_file(file_a_leer->file,disco,to_read);
				t_file_osada_destroy(file_a_leer);
				read->contenido = contenido;
				read->tamanio = to_read->size;
				return read;*/

				void *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);
				void *contenido_a_enviar = malloc(to_read->size);
				memcpy(contenido_a_enviar,contenido + to_read->offset,to_read->size);
				//void *contenido_a_enviar = malloc(size+1);
				//memcpy(contenido_a_enviar,contenido + to_read->offset,size);
				free(contenido);
				t_file_osada_destroy(file_a_leer);
				read->contenido = contenido_a_enviar;
				read->tamanio = to_read->size;
				return read;
			}

		}
	}
	else
	{
		return NO_EXISTE;
	}
}

void* osada_a_write_file(t_to_be_write *to_write)
{
	if(osada_check_exist(to_write->path))
	{
		if(osada_check_space_to_write(to_write))
		{
			t_file_osada *file = osada_get_file_called(to_write->path,disco);
			t_to_be_truncate *truncate = malloc(sizeof(t_to_be_truncate));
			truncate->file = file;
			truncate->new_size =to_write->size;
			osada_b_truncate_file(truncate);
			to_write->file = file;
			osada_write_file(to_write);
			t_file_osada_destroy(to_write->file);
			return EXITO;
		}
		else
		{
			return NO_HAY_ESPACIO;
		}
	}
	else
	{
		return NO_EXISTE;
	}

}

/*-------------------------------------------RENAME---------------------------------------------------------------------*/

void* osada_a_rename(t_to_be_rename *to_rename)
{
	if(osada_check_exist(to_rename->old_path))
		{

			if(osada_b_check_name(to_rename->new_path))
			{
				t_file_osada *file = osada_get_file_called(to_rename->old_path,disco);
				if(!osada_b_check_repeat_name(file->file->state, to_rename->new_path))
				{
					osada_b_rename(file, to_rename->new_path);
					t_file_osada_destroy(file);
					return EXITO;
				}
				else
				{
					t_file_osada_destroy(file);
					return ARGUMENTO_INVALIDO;
				}
				}
				else
				{
					return ARGUMENTO_INVALIDO;
				}
			}
			else
			{
				return NO_EXISTE;
			}
}

/*-------------------------------------------OPENS & CLOSER-------------------------------------------------------------*/
void* osada_a_open_file(char *path)
{
	if(string_equals_ignore_case(path,"/"))
	{
		return EXITO;
	}
	else
	{
		if(osada_check_exist(path))
				{
					return EXITO;
				}
			else
			{
				return NO_EXISTE;
			}
	}

}
