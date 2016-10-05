/*
 * fuse-basic.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "fuse-basic.h"

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
	int resultado = cliente_pedi_abrir_archivo(path, buf, size, offset);
	return resultado;
}

static int osada_escribi_archivo(const *path, const char *text, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int resultado = cliente_pedi_escribir_archivo(path, text, size, offset, fi);
	return resultado;
}

/*-------------------------------------------RENAME---------------------------------------------------------------*/
static int osada_renombra_archivo(const char *old_path, const char *new_path)
{
	int resultado = cliente_pedi_renombra_archivo(old_path,new_path);
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


