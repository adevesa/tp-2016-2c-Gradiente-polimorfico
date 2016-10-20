/*
 * ls_and_atributes.c
 *
 *  Created on: 13/10/2016
 *      Author: utnso
 */
#include "ls_and_atributes.h"
/*---------------------------------------------TAMAÑO DE UN DIRECTORIO---------------------------------------------------*/
int osada_b_calculate_size_of_directory(char *path_directory)
{
	if(es_directorio_vacio(path_directory))
	{
		return 0;
	}
	else
	{
		return realizar_sumatoria_size_hijos(path_directory);
	}
}

int realizar_sumatoria_size_hijos(char *path)
{
	t_list *lista_hijos = osada_b_listar_hijos(path);
	int size = list_size(lista_hijos);
	int i;
	int sumatoria = 0;

	for(i=0; i<size;i++)
	{
		t_file_listado *file = list_get(lista_hijos,i);
		if(verify_file_state(REGULAR,file->file->file))
		{
			sumatoria = sumatoria + file->file->file->file_size;
		}
		else
		{
			sumatoria = sumatoria + osada_b_calculate_size_of_directory(file->path);
		}
	}
	list_destroy_and_destroy_elements(lista_hijos, file_listado_eliminate);
	return sumatoria;
}

/*------------------------------------------LISTAR HIJOS  ----------------------------------------------------------------*/
t_list* osada_b_listar_hijos(char* path)
{
	t_list* lista = list_create();
	int index = 1;

	if(es_el_directorio_raiz(path))
	{
		listar_directorio_raiz(lista);
	}
	else
	{
		listar_directorio_comun(lista, path);
	}
	return lista;
}

void listar_directorio_raiz(t_list *lista)
{

	osada_file *file_1 = malloc(sizeof(osada_file));
	osada_file *file_2 = malloc(sizeof(osada_file));
	int index = 0;

	while(index<=1024)
	{

		void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
		memcpy(file_1,two_files, sizeof(osada_file));
		memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
		agregar_a_lista_si_es_hijo_de_raiz(RAIZ,file_1,lista);
		agregar_a_lista_si_es_hijo_de_raiz(RAIZ,file_2,lista);

		index++;
	}
	free(file_1);
	free(file_2);


}

void listar_directorio_comun(t_list *lista, char *path)
{
	t_file_osada *file_path = osada_get_file_called(path,disco);
		if(file_path->file->state == DIRECTORY)
		{
			osada_file *file_1 = malloc(sizeof(osada_file));
			osada_file *file_2 = malloc(sizeof(osada_file));
			int index =0;
				while(index<=1024)
				{

					void *two_files = osada_get_blocks_relative_since(TABLA_DE_ARCHIVOS,index,1,disco);
					memcpy(file_1,two_files, sizeof(osada_file));
					memcpy(file_2,two_files + sizeof(osada_file), sizeof(osada_file));
				 	agregar_a_lista_si_es_hijo(file_path,file_1,lista);
				 	agregar_a_lista_si_es_hijo(file_path,file_2,lista);

				 	index++;
				}
				free(file_1);
				free(file_2);
		}
}

int es_el_directorio_raiz(char *path)
{
	return string_equals_ignore_case(path,"/");
}

void agregar_a_lista_si_es_hijo(t_file_osada *path_padre, osada_file* hijo, t_list* lista)
{
	if(!verificar_si_son_mismo_files(path_padre->file,hijo))
	{
		if(es_el_padre(hijo, path_padre) && !verify_file_state(DELETED, hijo))
		{
			t_file_listado* dato_en_lista = malloc(sizeof(t_file_listado));
			char* path_hijo = crear_ruta((char*) hijo->fname,(char*)path_padre->path);
			dato_en_lista->file = osada_get_file_called(path_hijo,disco);
			dato_en_lista->path = path_hijo;
			dato_en_lista->tipo = dato_en_lista->file->file->state;
			list_add(lista, dato_en_lista);
		}
	}

}

void agregar_a_lista_si_es_hijo_de_raiz(int num_raiz, osada_file *file, t_list *lista)
{
	if(num_raiz == file->parent_directory)
	{
		t_file_listado* dato_en_lista = malloc(sizeof(t_file_listado));
		char *path_hijo = string_new();

		string_append(&path_hijo, "/");
		string_append(&path_hijo, (char*)file->fname);

		dato_en_lista->file = osada_get_file_called(path_hijo,disco);
		dato_en_lista->path = path_hijo;
		dato_en_lista->tipo = dato_en_lista->file->file->state;
		list_add(lista, dato_en_lista);
	}
}

int es_el_padre(osada_file* file_hijo,t_file_osada *path_padre)
{
	int posicion_en_tabla_de_archivos = calcular_posicion_en_tabla_de_archivos(path_padre->block_relative,path_padre->position_in_block);
	return (posicion_en_tabla_de_archivos==file_hijo->parent_directory);
}

int verificar_si_son_mismo_files(osada_file *file_actual, osada_file *file_expected)
{
	int nombres_coinciden = string_equals_ignore_case((char*) file_actual->fname,(char*)file_expected->fname);
	return ((file_actual->parent_directory == file_expected->parent_directory) && nombres_coinciden);
}

/*------------------------------------------ATRIBUTOS----------------------------------------------------------------------*/
t_attributes_file* osada_b_get_attributes_of_this_file(char *path_file)
{
	t_file_osada *file_find = osada_get_file_called(path_file, disco);
	t_attributes_file *atributos = malloc(sizeof(t_attributes_file));

	atributos->size = osada_b_calculate_size_of_directory(file_find->path);
	atributos->tipo = file_find->file->state;

	t_file_osada_destroy(file_find);
	return atributos;
}
