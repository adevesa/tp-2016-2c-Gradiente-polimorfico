/*
 * comunication-with-server.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "comunication-with-server.h"

/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
int cliente_pedi_atributos(const char *path, struct stat *buffer)
{
	return 1;
}

int cliente_pedi_listado(const char *path, void *buffer, fuse_fill_dir_t filler)
{
	return 2;
}

/*-------------------------------------------CREACION-----------------------------------------------------------------*/
int cliente_pedi_crear_directorio(const char *path, mode_t modo_de_creacion)
{
	return 5;
}

int cliente_pedi_crear_archivo(const char *path, mode_t modo, dev_t permisos)
{
	return 8;
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
int cliente_pedi_eliminar(int tipo,const char *path)
{
	if(tipo==ARCHIVO)
	{
		return 1;
	}
	else
	{
		return 4;
	}
}

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
int cliente_pedi_leer_archivo(const char *path, char *buf, size_t size, off_t offset)
{
	return 4;
}

int cliente_pedi_escribir_archivo(const char *path, const char *text, size_t size, off_t offset, struct fuse_file_info *f)
{
	return 1;
}

/*-------------------------------------------RENAME---------------------------------------------------------------*/
int cliente_pedi_renombra_archivo(const char *old_path, const char *new_path)
{
	return 4;
}

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
int cliente_pedi_abrir(int tipo,const char *path, struct fuse_file_info *fi)
{
	if(tipo==ARCHIVO)
	{
		return 0;
	}
	else
	{
		return 3;
	}
}
