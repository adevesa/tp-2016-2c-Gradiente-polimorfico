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
		if(file_a_leer->file->file_size == 0)
		{
			return ARGUMENTO_INVALIDO;
		}
		else
		{
			char *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);

			char *contenido_a_enviar = malloc(to_read->size);
			memcpy(contenido_a_enviar,contenido + to_read->offset,to_read->size);

			free(contenido);
			t_file_osada_destroy(file_a_leer);
			return contenido_a_enviar;
		}
	}
	else
	{
		return NO_EXISTE;
	}
}

void* osada_a_write_file(t_to_be_write *to_write)
{


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
	if(osada_check_exist(path))
		{
			return EXITO;
		}
	else
	{
		return NO_EXISTE;
	}
}
