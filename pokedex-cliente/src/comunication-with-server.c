/*
 * comunication-with-server.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "comunication-with-server.h"
extern t_cliente_osada* cliente_osada;

/*-------------------------------------------CREACION Y CONEXION CON SERVER----------------------------------------------*/
void cliente_osada_create()
{
	cliente_osada =malloc(sizeof(t_cliente_osada));
	cliente_osada->ip = getenv("IP");

	char *puerto = getenv("PUERTO");
	cliente_osada->puerto = atoi(puerto);

	free(puerto);

}

void cliente_osada_conectate()
{
	cliente_osada->socket_pokedex_servidor = cliente_create(cliente_osada->puerto, cliente_osada->ip);
}

/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
int cliente_pedi_atributos(const char *path, struct stat *buffer)
{
	char *msg = build_msg(GET_ATRIBUTES,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 1;
}

int cliente_pedi_listado(const char *path, void *buffer, fuse_fill_dir_t filler)
{
	char *msg = build_msg(LISTAR,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 2;
}

/*-------------------------------------------CREACION-----------------------------------------------------------------*/
int cliente_pedi_crear_directorio(const char *path, mode_t modo_de_creacion)
{
	char *msg = build_msg(CREATE_DIRECTORY,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 5;
}

int cliente_pedi_crear_archivo(const char *path, mode_t modo, dev_t permisos)
{
	char *msg = build_msg(CREATE_FILE,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 8;
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
int cliente_pedi_eliminar(int tipo,const char *path)
{
	switch(tipo)
	{
		case(DIRECTORIO):
		{
			char *msg = build_msg(DELETE_DIRECTTORY,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
		};break;
		case(ARCHIVO):
		{
			char *msg = build_msg(DELETE_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
		};break;
	}
}

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
int cliente_pedi_leer_archivo(const char *path, char *buf, size_t size, off_t offset)
{
	char *msg = build_msg(READ_FILE,path,NULL,size,offset);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 4;
}

int cliente_pedi_escribir_archivo(const char *path, const char *text, size_t size, off_t offset, struct fuse_file_info *f)
{
	char *msg = build_msg(WRITE_FILE,path,text,size,offset);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 1;
}

/*-------------------------------------------RENAME---------------------------------------------------------------*/
int cliente_pedi_renombra_archivo(const char *old_path, const char *new_path)
{
	char *msg = build_msg(RENAME_FILE,old_path,new_path,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	return 4;
}

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
int cliente_pedi_abrir(int tipo,const char *path, struct fuse_file_info *fi)
{
	switch(tipo)
	{
		case(ARCHIVO):
		{
			char *msg = build_msg(OPEN_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
		};break;
		case(DIRECTORIO):
		{

		};break;
	}
}
