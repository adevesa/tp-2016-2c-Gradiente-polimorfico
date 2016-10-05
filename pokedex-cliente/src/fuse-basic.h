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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <fcntl.h>
#include "so-commons/string.h"
#include "comunication-with-server.h"
/*
 * REDEFINIMOS FLAGS DE RETORNO
 */
#define NO_EXISTE ENOENT
#define EXISTE EEXIST
#define NO_HAY_ESPACIO ENOSPC
#define ARGUMENTO_INVALIDO EINVAL
#define OPERACION_EXITOSA 0



/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
static int osada_get_atributes(const char *path, struct stat *buffer);
static int osada_listame(const char *path, void *buffer, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi);

/*-------------------------------------------CREACION-----------------------------------------------------------------*/
static int osada_crea_directorio(const char *path, mode_t modo_de_creacion);
static int osada_crea_archivo(const char *path, mode_t modo, dev_t permisos);

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
static int osada_borra_archivo(const char *path);
static int osada_borra_directorio(const char *path);

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
static int osada_lee_archivo_desde(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *f);
static int osada_escribi_archivo(const *path, const char *text, size_t size, off_t offset, struct fuse_file_info *fi);

/*-------------------------------------------RENAME---------------------------------------------------------------*/
static int osada_renombra_archivo(const char *old_path, const char *new_path);

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
static int osada_abri_archivo(const char *path, struct fuse_file_info *fi);
static int osada_abri_directorio(const char *path, struct fuse_file_info *fi);

/*-------------------------------------------STRUCTS-----------------------------------------------------------------*/
static struct fuse_operations osada_operations =
{
		.getattr = osada_get_atributes,
		//.readdir = osada_listame,
		.mkdir = osada_crea_directorio,
		.mknod = osada_crea_archivo,
		.unlink = osada_borra_archivo,
		.rmdir = osada_borra_directorio,
		.read = osada_lee_archivo_desde,
		.write = osada_escribi_archivo,
		.rename = osada_renombra_archivo,
		.open = osada_abri_archivo
};


#endif /* FUSE_BASIC_H_ */
