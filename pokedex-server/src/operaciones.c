/*
 * operaciones.c
 *
 *  Created on: 2/10/2016
 *      Author: utnso
 */
#include "operaciones.h"
extern pthread_mutex_t mutex_operaciones;
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
			pthread_mutex_lock(&mutex_operaciones);
			int hay_lugar = osada_hay_espacio_para_archivos();
			pthread_mutex_unlock(&mutex_operaciones);
			if(hay_lugar)
			{
				t_osada_file_free *new_file=osada_b_file_create(REGULAR,path);
				int offset = calcular_desplazamiento_tabla_de_archivos(new_file->position_in_block);
				pthread_mutex_lock(&mutex_operaciones);
				osada_push_middle_block(TABLA_DE_ARCHIVOS,new_file->block_relative,offset,new_file->file,disco);
				pthread_mutex_unlock(&mutex_operaciones);
				osada_disminui_cantidad_de_archivos();
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

			pthread_mutex_lock(&mutex_operaciones);
			int hay_lugar = osada_hay_espacio_para_archivos();
			pthread_mutex_unlock(&mutex_operaciones);
			if(hay_lugar)
			{
				t_osada_file_free *new_file=osada_b_file_create(DIRECTORY,path);
				pthread_mutex_lock(&mutex_operaciones);
				int offset = calcular_desplazamiento_tabla_de_archivos(new_file->position_in_block);
				osada_push_middle_block(TABLA_DE_ARCHIVOS,new_file->block_relative,offset,new_file->file,disco);
				pthread_mutex_unlock(&mutex_operaciones);
				osada_disminui_cantidad_de_archivos();
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

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
void* osada_a_delete_file(char *path)
{
	if(osada_check_exist(path))
	{
		osada_delete_this_file(path);
		osada_aumenta_cantidad_de_archivos();
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
		osada_aumenta_cantidad_de_archivos();
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
			int tamanio_final = to_read->offset + to_read->size;

			if(size< tamanio_final)
			{
				if(to_read->offset == 0)
				{
					pthread_mutex_lock(&mutex_operaciones);
					void *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);
					pthread_mutex_unlock(&mutex_operaciones);
					void *contenido_a_enviar = malloc(size); //size +1
					memcpy(contenido_a_enviar,contenido,size);
					free(contenido);
					t_file_osada_destroy(file_a_leer);
					read->contenido = contenido_a_enviar;
					read->tamanio = size;
					return read;
				}
				else
				{
					pthread_mutex_lock(&mutex_operaciones);
					void *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);
					pthread_mutex_unlock(&mutex_operaciones);
					void *contenido_a_enviar = malloc(size); //size +1
					memcpy(contenido_a_enviar,contenido + to_read->offset,(tamanio_final-size));
					free(contenido);
					t_file_osada_destroy(file_a_leer);
					read->contenido = contenido_a_enviar;
					read->tamanio = tamanio_final - size;
					return read;
				}
			}
			else
			{
				pthread_mutex_lock(&mutex_operaciones);
				void *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);
				pthread_mutex_unlock(&mutex_operaciones);
				void *contenido_a_enviar = malloc(to_read->size+1);
				memcpy(contenido_a_enviar,contenido + to_read->offset,to_read->size);

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
		t_file_osada *file = osada_get_file_called(to_write->path,disco);
		//if(osada_check_space_to_write(to_write))
		int new_size_to_truncate = to_write->size + file->file->file_size;
		if(osada_check_space_to_truncate(NULL,file,new_size_to_truncate))
		{
			t_to_be_truncate *truncate = malloc(sizeof(t_to_be_truncate));
			to_write->size_inmediatamente_anterior = file->file->file_size;
			if(to_write->offset == 0 && file->file->file_size>=0)
			{
				truncate->new_size = to_write->size;
			}
			else
			{
				truncate->new_size = to_write->size + file->file->file_size;
			}
			truncate->file = file;
			osada_b_truncate_file(truncate);
			to_write->file = file;
			if(to_write->size_inmediatamente_anterior !=0)
			{
				if(to_write->size == PAGE_SIZE_MAX && es_multiplo_de(to_write->offset,to_write->size_inmediatamente_anterior) )
				{
					osada_write_aux_file(to_write);
				}
				else
				{
					osada_write_file(to_write);
				}
			}
			else
			{
				osada_write_file(to_write);
			}
			osada_b_actualiza_time(to_write->file);
			t_file_osada_destroy(to_write->file);
			free(truncate);
			return EXITO;
		}
		else
		{
			t_file_osada_destroy(file);
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

/*-------------------------------------------TRUNCATE-------------------------------------------------------------------*/
void* osada_a_truncate_file(char* path, int new_size)
{
	if(osada_check_exist(path))
	{
		t_file_osada *file = osada_get_file_called(path,disco);
		if(new_size == 0 && file->file->file_size == 0)
		{
			t_file_osada_destroy(file);
			return EXITO;
		}
		else
		{
			if(osada_check_space_to_truncate(NULL,file,new_size))
			{
				t_to_be_truncate* trunct = malloc(sizeof(t_to_be_truncate));
				trunct->path=path;
				trunct->new_size = new_size;
				trunct->file=file;
				osada_b_truncate_file(trunct);
				osada_b_actualiza_time(file);
				t_file_osada_destroy(file);
				free(trunct);
				return EXITO;
			}
			else
			{
				t_file_osada_destroy(file);
				return NO_HAY_ESPACIO;
			}
		}
		}
	else
	{
		return NO_EXISTE;
	}

}




