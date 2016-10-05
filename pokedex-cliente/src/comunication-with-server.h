/*
 * comunication-with-server.h
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */

#ifndef COMUNICATION_WITH_SERVER_H_
#define COMUNICATION_WITH_SERVER_H_

#include "fuse-basic.h"
enum
{
	ARCHIVO,
	DIRECTORIO
};
/*-------------------------------------------CREACION-----------------------------------------------------------------*/
int cliente_pedi_crear_directorio(const char *path, mode_t modo_de_creacion);
int cliente_pedi_crear_archivo(const char *path, mode_t modo, dev_t permisos);
/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
int cliente_pedi_atributos(const char *path, struct stat *buffer);
int cliente_pedi_listado(const char *path, void *buffer, fuse_fill_dir_t filler);
/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
int cliente_pedi_eliminar(int tipo,const char *path);
/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
int cliente_pedi_leer_archivo(const char *path, char *buf, size_t size, off_t offset);
int cliente_pedi_escribir_archivo(const char *path, const char *text, size_t size, off_t offset, struct fuse_file_info *f);
/*-------------------------------------------RENAME---------------------------------------------------------------*/
int cliente_pedi_renombra_archivo(const char *old_path, const char *new_path);
/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
int cliente_pedi_abrir(int tipo,const char *path, struct fuse_file_info *fi);

#endif /* COMUNICATION_WITH_SERVER_H_ */
