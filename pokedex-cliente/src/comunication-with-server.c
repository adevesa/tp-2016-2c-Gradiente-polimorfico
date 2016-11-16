/*
 * comunication-with-server.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "comunication-with-server.h"
extern t_cliente_osada* cliente_osada;

pthread_mutex_t mutex_operaciones;

#define LOG_ACTIVADO 1

void iniciar_log()
{
	if(LOG_ACTIVADO)
	{
		log = log_create("Cliente pokedex", "Cliente",0,LOG_LEVEL_DEBUG);
	}

}

/*-------------------------------------------CREACION Y CONEXION CON SERVER----------------------------------------------*/
void cliente_osada_create()
{
	cliente_osada =malloc(sizeof(t_cliente_osada));
	//cliente_osada->ip = getenv("IP");
	cliente_osada->ip = string_new();
	string_append(&cliente_osada->ip, "127.0.0.1");

	//char *puerto = getenv("PUERTO");
	//cliente_osada->puerto = atoi(puerto);

	//free(puerto);

	cliente_osada->puerto = 5001;
	pthread_mutex_init(&mutex_operaciones,NULL);
}

void cliente_osada_conectate()
{
	cliente_osada->socket_pokedex_servidor = cliente_create(cliente_osada->puerto, cliente_osada->ip);
}

/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
int cliente_pedi_atributos(const char *path, struct stat *buffer)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"ATRIBUTOS: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);
	}

	char *msg = build_msg(GET_ATRIBUTES,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int resultado = escuchar_y_modelar_atributos(buffer);
	pthread_mutex_unlock(&mutex_operaciones);
	return resultado;
}

int cliente_pedi_listado(const char *path, void *buffer, fuse_fill_dir_t filler)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"LISTAR: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);
	}

	char *msg = build_msg(LISTAR,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int resultado = escuchar_listado(buffer,filler);
	pthread_mutex_unlock(&mutex_operaciones);
	return resultado;
}

/*-------------------------------------------CREACION-----------------------------------------------------------------*/
int cliente_pedi_crear_directorio(const char *path, mode_t modo_de_creacion)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
			string_append(&mensaje_A_log,"CREAR DIRECTORIO: ");
			string_append(&mensaje_A_log,path);
			log_info(log,mensaje_A_log);
			free(mensaje_A_log);
	}


	char *msg = build_msg(CREATE_DIRECTORY,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	if(respuesta==ARGUMENTO_INVALIDO)
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return -ENAMETOOLONG;
	}
	else
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return respuesta;
	}
}

int cliente_pedi_crear_archivo(const char *path, mode_t modo, dev_t permisos)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"CREAR ARCHIVO: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);
	}

	char *msg = build_msg(CREATE_FILE,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	if(respuesta==ARGUMENTO_INVALIDO)
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return -ENAMETOOLONG;
	}
	else
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return respuesta;
	}
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
int cliente_pedi_eliminar(int tipo,const char *path)
{
	switch(tipo)
	{
		case(DIRECTORIO):
		{
			pthread_mutex_lock(&mutex_operaciones);
			if(LOG_ACTIVADO)
			{
							char *mensaje_A_log = string_new();
							string_append(&mensaje_A_log,"ELIMINAR DIRECTORIO: ");
							string_append(&mensaje_A_log,path);
							log_info(log,mensaje_A_log);
							free(mensaje_A_log);
			}


			char *msg = build_msg(DELETE_DIRECTTORY,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
		case(ARCHIVO):
		{
			pthread_mutex_lock(&mutex_operaciones);
			if(LOG_ACTIVADO)
			{
				char *mensaje_A_log = string_new();
							string_append(&mensaje_A_log,"ELIMINAR ARCHIVO: ");
							string_append(&mensaje_A_log,path);
							log_info(log,mensaje_A_log);
							free(mensaje_A_log);
			}


			char *msg = build_msg(DELETE_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
	}
}

/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
int cliente_pedi_leer_archivo(const char *path, char *buf, size_t size, off_t offset)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
			string_append(&mensaje_A_log,"LEER ");
			char *size_string = string_itoa(size);
			char *offset_string = string_itoa(offset);
			string_append(&mensaje_A_log, size_string);
			string_append(&mensaje_A_log, " BYTES, OFFSET: ");
			string_append(&mensaje_A_log, offset_string);
			string_append(&mensaje_A_log, " DEL PATH: ");
			string_append(&mensaje_A_log,path);
			log_info(log,mensaje_A_log);
			free(mensaje_A_log);
			free(size_string);
			free(offset_string);
	}

	char *msg = build_msg(READ_FILE,path,NULL,size,offset);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	char *primer_byte = recibir_mensaje(cliente_osada->socket_pokedex_servidor,10);
	if(string_equals_ignore_case(primer_byte,"FFFFFFFFFF"))
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return 0;
	}
	else
	{
		int tamanio = atoi(primer_byte);
		log_info(log,"AHORA VOY A ESCUCHAR EL MENSAJE!");
		//void *lectura_final = reciveall(cliente_osada->socket_pokedex_servidor,tamanio);
		void *lectura_final = recibir_mensaje_tipo_indistinto(cliente_osada->socket_pokedex_servidor,tamanio);
		memcpy(buf,lectura_final,tamanio);
		free(lectura_final);

		if(LOG_ACTIVADO)
		{
			char *mensaje_A_log_2 = string_new();
				string_append(&mensaje_A_log_2,"LECTURA COMPLETADA CON: ");
					string_append(&mensaje_A_log_2,primer_byte);
					string_append(&mensaje_A_log_2, " LEIDOS");
					log_info(log,mensaje_A_log_2);
					free(mensaje_A_log_2);
					free(primer_byte);

		}

		pthread_mutex_unlock(&mutex_operaciones);
		return tamanio;
	}
}

int cliente_pedi_escribir_archivo(const char *path, const char *text, size_t size, off_t offset, struct fuse_file_info *f)
{
	pthread_mutex_lock(&mutex_operaciones);

	if(LOG_ACTIVADO)
	{	char *mensaje_A_log_2 = string_new();
		string_append(&mensaje_A_log_2,"ESCRIBIR: ");
			string_append(&mensaje_A_log_2,path);
			string_append(&mensaje_A_log_2, " CON ");
			string_append(&mensaje_A_log_2,string_itoa(size));
			string_append(&mensaje_A_log_2, " OFFSET ");
			string_append(&mensaje_A_log_2,string_itoa(offset));
			log_info(log,mensaje_A_log_2);
			free(mensaje_A_log_2);
	}


	char *msg = build_msg(WRITE_FILE,(char*)path,text,size,offset);

	//int tamanio_a_enviar = size+30+2+string_length(path);
	int tamanio_a_enviar = 32 +string_length(path);
	sendall(cliente_osada->socket_pokedex_servidor,msg,&tamanio_a_enviar);
	//enviar_mensaje_cantidad_especifica(cliente_osada->socket_pokedex_servidor,msg, tamanio_a_enviar);
	free(msg);

	int tamanio_mensaje = size;
	sendall(cliente_osada->socket_pokedex_servidor,text,&tamanio_mensaje);

	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log_2 = string_new();
		string_append(&mensaje_A_log_2,"SE ENVIARON: ");
		string_append(&mensaje_A_log_2,string_itoa(tamanio_mensaje));
		string_append(&mensaje_A_log_2," BYTES ");
		log_info(log,mensaje_A_log_2);
		free(mensaje_A_log_2);
	}

	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);

	if(LOG_ACTIVADO)
	{
		char *respuesta_String = string_itoa(respuesta);
			string_append(&respuesta_String, " FUE EL RESULTADO DE LA ESCRITURA");
			log_info(log,respuesta_String);
			free(respuesta_String);
	}

	switch(respuesta)
	{
		case(OPERACION_EXITOSA):
		{
			pthread_mutex_unlock(&mutex_operaciones);
			return size;
		};break;
		case(NO_EXISTE):
		{
			pthread_mutex_unlock(&mutex_operaciones);
			return 0;
		}break;
		case(NO_HAY_ESPACIO):
		{
			pthread_mutex_unlock(&mutex_operaciones);
			return NO_HAY_ESPACIO;
		};break;
	}

}

/*-------------------------------------------RENAME---------------------------------------------------------------*/
int cliente_pedi_renombra_archivo(const char *old_path, const char *new_path)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
	{
		char *mensaje_A_log = string_new();
			string_append(&mensaje_A_log,"RENOMBRAR: ");
			string_append(&mensaje_A_log,old_path);
			string_append(&mensaje_A_log, new_path);
			log_info(log,mensaje_A_log);
			free(mensaje_A_log);

	}
	char *msg = build_msg(RENAME_FILE,(char*)old_path,(char*)new_path,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	if(respuesta==ARGUMENTO_INVALIDO)
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return ENAMETOOLONG;
	}
	else
	{
		pthread_mutex_unlock(&mutex_operaciones);
		return respuesta;
	}
}

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
int cliente_pedi_abrir(int tipo,const char *path, struct fuse_file_info *fi)
{
	switch(tipo)
	{
		case(ARCHIVO):
		{
			pthread_mutex_lock(&mutex_operaciones);
			if(LOG_ACTIVADO)
			{
				char *mensaje_A_log = string_new();
				string_append(&mensaje_A_log,"ABRIR ARCHIVO: ");
				string_append(&mensaje_A_log,path);
				log_info(log,mensaje_A_log);
				free(mensaje_A_log);
			}


			char *msg = build_msg(OPEN_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			fi->flags = FUSE_BUF_FORCE_SPLICE;
			int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);

			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
		case(DIRECTORIO):
		{
			pthread_mutex_lock(&mutex_operaciones);
			if(LOG_ACTIVADO)
			{
				char *mensaje_A_log = string_new();
							string_append(&mensaje_A_log,"ABRIR DIRECTORIO: ");
							string_append(&mensaje_A_log,path);
							log_info(log,mensaje_A_log);
							free(mensaje_A_log);

			}

			char *msg = build_msg(OPEN_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
	}
}

/*-------------------------------------------TRUNCATE----------------------------------------------------------------*/
int cliente_pedi_truncar(const char* path, off_t size)
{
	pthread_mutex_lock(&mutex_operaciones);
	if(LOG_ACTIVADO)
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
	}
	char* msg = build_msg(11,path,NULL,size,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	pthread_mutex_unlock(&mutex_operaciones);
	return respuesta;
}

/*-------------------------------------------TIMES---------------------------------------------------------------*/
int cliente_pedi_times(const char* path, const struct timespec ts[2])
{

}

/*-------------------------------------------RESPUESTAS DEL SERVER------------------------------------------------*/
int escuchar_respuesta_comun(int socket_server)
{
	char *msj = recibir_mensaje(socket_server, MAX_BYTES_TO_ADVISES);
	int respuesta = atoi(msj);
	switch(respuesta)
	{
		case(EXITO): return OPERACION_EXITOSA; break;
		case(NO_EXISTEE): return NO_EXISTE; break;
		case(EXISTEE): return EXISTE; break;
		case(NO_HAY_ESPACIOO): return -NO_HAY_ESPACIO;break;
		case(ARGUMENTO_INVALIDOO): return ARGUMENTO_INVALIDO; break;
	}
	free(msj);
}

int escuchar_y_modelar_atributos(struct stat *buffer)
{
	char *verify = recibir_mensaje(cliente_osada->socket_pokedex_servidor,1);
	if(string_equals_ignore_case(verify,"F"))
	{
		free(verify);
		return -NO_EXISTE;
	}
	else
	{
		char* size_string = recibir_mensaje(cliente_osada->socket_pokedex_servidor,9);
		string_append(&verify, size_string);
		free(size_string);
		int size = atoi(verify);
		free(verify);

		char* tipo_string = recibir_mensaje(cliente_osada->socket_pokedex_servidor,1);
		int tipo = atoi(tipo_string);
		free(tipo_string);

		modelar_stat_buff(buffer,tipo,size);
		return OPERACION_EXITOSA;
	}

}

void modelar_stat_buff(struct stat *buffer, int tipo, int size)
{
	switch(tipo)
	{
		case(DIRECTORIO):
			{
				buffer->st_mode = S_IFDIR | 0755;
				buffer->st_nlink = 2;
				buffer->st_size = size;
			}break;
		case(ARCHIVO):
			{
				buffer->st_mode = S_IFREG | 0444;
				buffer->st_nlink = 1;
				buffer->st_size = size;
				//buffer->st_mtim
			}break;
	}

}

int escuchar_listado (void *buffer, fuse_fill_dir_t filler)
{
	char *cantidad_de_elementos_string = recibir_mensaje(cliente_osada->socket_pokedex_servidor,4);

	if(string_equals_ignore_case(cantidad_de_elementos_string, "FFFF"))
	{
		return -NO_EXISTE;
	}
	else
	{
		if(string_equals_ignore_case(cantidad_de_elementos_string, "0000"))
		{
			return OPERACION_EXITOSA;
		}
		else
		{
			int cantidad_de_elementos = atoi(cantidad_de_elementos_string);
			int i=0;
			while(i<cantidad_de_elementos)
				{
					modelar_listado(buffer,filler);
					i++;
				}
			return OPERACION_EXITOSA;
		}
	}
}

void modelar_listado(void *buffer, fuse_fill_dir_t filler)
{
	char *tamanio_del_nombre_string = recibir_mensaje(cliente_osada->socket_pokedex_servidor,2);
	int tamanio_del_nombre = atoi(tamanio_del_nombre_string);
	free(tamanio_del_nombre_string);

	char *nombre = recibir_mensaje(cliente_osada->socket_pokedex_servidor,tamanio_del_nombre);
	filler(buffer, nombre,NULL,0);
	//free(nombre);
}
