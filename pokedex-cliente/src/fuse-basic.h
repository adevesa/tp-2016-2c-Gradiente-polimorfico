/*
 * fuse-basic.h
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */

#ifndef FUSE_BASIC_H_
#define FUSE_BASIC_H_
#include <stdio.h>
#include <fuse.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <fcntl.h>

#include "so-commons/string.h"
#include "comunication-with-server.h"
#include "vars.h"


extern t_log *log;


/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
static int osada_get_atributes(const char *path, struct stat *buffer)
{
	memset(buffer, 0, sizeof(struct stat));
	int resultado = cliente_pedi_atributos(path, buffer);
	return resultado;
}

static int osada_listame(const char *path, void *buffer, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
	int resultado = cliente_pedi_listado(path, buffer, filler);
	return resultado;
}
/*-------------------------------------------CREACION-----------------------------------------------------------------*/
static int osada_crea_directorio(const char *path, mode_t modo_de_creacion)
{
	int resultado = cliente_pedi_crear_directorio(path, modo_de_creacion);
	return resultado;
}

static int osada_crea_archivo(const char *path, mode_t modo, dev_t permisos)
{
	int resultado = cliente_pedi_crear_archivo(path, modo, permisos);
	return resultado;
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
static int osada_borra_archivo(const char *path)
{
	int resultado = cliente_pedi_eliminar(ARCHIVO,path);
	return resultado;
}

static int osada_borra_directorio(const char *path)
{
	int resultado = cliente_pedi_eliminar(DIRECTORIO,path);
	return resultado;
}

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
static int osada_lee_archivo_desde(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *f)
{
	int resultado = cliente_pedi_leer_archivo(path, buf, size, offset);
	return resultado;
}

static int osada_escribi_archivo(const char *path, const char *text, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int resultado = cliente_pedi_escribir_archivo((char*)path, text, size, offset, fi);
	return resultado;
}

/*-------------------------------------------RENAME---------------------------------------------------------------*/
static int osada_renombra_archivo(const char *old_path, const char *new_path)
{
	int resultado = cliente_pedi_renombra_archivo(old_path,new_path);
	return resultado;
}

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
static int osada_abri_archivo(const char *path, struct fuse_file_info *fi)
{
	int resultado = cliente_pedi_abrir(ARCHIVO,path, fi);
	return resultado;
}

static int osada_abri_directorio(const char *path, struct fuse_file_info *fi)
{
	int resultado = cliente_pedi_abrir(DIRECTORIO,path, fi);
	return resultado;
}

static int osada_trucate_file(const char* path, off_t size)
{
	char *mensaje_to_log = string_new();
	string_append(&mensaje_to_log, "TRUNCATE: ");
	string_append(&mensaje_to_log, path);
	string_append(&mensaje_to_log, " SIZE: ");
	char *size_string = string_itoa(size);
	string_append(&mensaje_to_log, size_string);
	log_info(log,mensaje_to_log);
	free(mensaje_to_log);
	free(size_string);
	return OPERACION_EXITOSA;
};

static int osada_acces(const char* path, int mask)
{
	int resultado = cliente_pedi_abrir(DIRECTORIO,path, NULL);
	return resultado;
}
/*-------------------------------------------STRUCTS-----------------------------------------------------------------*/
static struct fuse_operations osada_operations =
{
		.getattr = osada_get_atributes,
		.readdir = osada_listame,
		.mkdir = osada_crea_directorio,
		.mknod = osada_crea_archivo,
		.unlink = osada_borra_archivo,
		.rmdir = osada_borra_directorio,
		.read = osada_lee_archivo_desde,
		.write = osada_escribi_archivo,
		.rename = osada_renombra_archivo,
		.open = osada_abri_archivo,
		.truncate = osada_trucate_file,
		.access= osada_acces
};


#endif /* FUSE_BASIC_H_ */
