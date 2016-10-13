/*
 * borrados.c
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */
#include "borrados.h"
/*---------------------------------------------DELETE FILE-----------------------------------------------------------------*/
void osada_delete_this_file(char *path)
{
	t_file_osada *a_file = osada_get_file_called(path, disco);
	t_list *bloques_a_liberar = osada_get_blocks_nums_of_this_file(a_file->file,disco);

	osada_desocupa_n_bits(bloques_a_liberar);
	list_destroy(bloques_a_liberar);

	osada_change_file_state(a_file->file,DELETED);
	int offset = calcular_desplazamiento_tabla_de_archivos(a_file->position_in_block);
	osada_push_middle_block(TABLA_DE_ARCHIVOS,a_file->block_relative,offset,a_file->file,disco);

	t_file_osada_destroy(a_file);
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

void osada_recalculate_size_of(char* path, int tamanio_a_restar)
{
	t_file_osada* file = osada_get_file_called(path, disco);
	file->file->file_size = file->file->file_size - tamanio_a_restar;
	int offset = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
	osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative, offset,file->file,disco);
	free(file->file);
	free(file);
}

char* recuperar_path_padre(char* path)
{
	char** file_for_file = string_split(path,"/");
	int size = array_size(file_for_file);
	int i;
	char* path_padre = string_new();
	string_append(&path_padre,"/");
	for(i=0;i<(size-1);i++)
	{
		string_append(&path_padre,file_for_file[i]);
		string_append(&path_padre,"/");
	}
	return path_padre;
}

/*------------------------------------------DELETE DIR---------------------------------------------------------------------*/
void osada_delete_this_dir(char* path)
{
		if(es_directorio_vacio(path))
		{
			osada_delete_dir_void(path);
		}
		else
		{
			osada_borrar_hijos(path);
		}
}

int es_directorio_vacio(char* path)
{
	t_list* lista = osada_b_listar_hijos(path);
	if(list_is_empty(lista)){
		list_destroy(lista);
		return 1;
	}
	else{
		list_destroy_and_destroy_elements(lista,file_listado_eliminate);
		return 0;
	}
}

void osada_delete_dir_void(char* path)
{
	t_osada_file_free* directorio = osada_get_file_called(path, disco);
	osada_change_file_state(directorio->file, DELETED);
	osada_push_middle_block(TABLA_DE_ARCHIVOS, directorio->block_relative, calcular_desplazamiento_tabla_de_archivos(directorio->position_in_block),directorio->file, disco);
	t_file_osada_destroy(directorio);
}


void osada_borrar_hijos(char* path)
{
	t_list* lista_hijos = osada_b_listar_hijos(path);
	int size_lista = list_size(lista_hijos);
	int index = 0;
	while(index < size_lista)
	{
		t_file_listado* hijo = list_get(lista_hijos, index);
		switch(hijo->tipo)
		{
			case(DIRECTORY):
				osada_delete_this_dir(hijo->path);
				break;
			case(REGULAR):
				osada_delete_this_file(hijo->path);
				break;
		}
		index++;
	}
	osada_delete_dir_void(path);
	list_destroy_and_destroy_elements(lista_hijos,file_listado_eliminate);
}
