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

void* osada_a_get_list_dir(char *path)
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

/*-------------------------------------------CREACION-------------------------------------------------------------------*/
void* osada_a_create_file(char *path)
{
	//nombre -17 caracteres
}

void* osada_a_create_dir(char *path)
{

}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
void* osada_a_delete_file(char *path)
{
	if(osada_check_exist(path))
	{
		osada_delete_this_file(path);
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
		char *contenido = osada_get_data_of_this_file(file_a_leer->file,disco);

		char *contenido_a_enviar = malloc(to_read->size);
		memcpy(contenido_a_enviar,contenido + to_read->offset,to_read->size);

		free(contenido);
		t_file_osada_destroy(file_a_leer);
		return contenido_a_enviar;
	}
	else
	{
		return NO_EXISTE;
	}
}

void* osada_a_write_file(t_to_be_write *to_write)
{
	//verificar espacio suficiente

}

/*-------------------------------------------RENAME---------------------------------------------------------------------*/
void* osada_a_rename_file(t_to_be_rename *to_rename)
{

}
/*-------------------------------------------OPENS & CLOSER-------------------------------------------------------------*/
void* osada_a_open_file(char *path)
{

}
