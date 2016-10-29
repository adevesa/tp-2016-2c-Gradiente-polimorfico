/*
 * comunication-with-server.c
 *
 *  Created on: 5/10/2016
 *      Author: utnso
 */
#include "comunication-with-server.h"
extern t_cliente_osada* cliente_osada;

pthread_mutex_t mutex_operaciones=PTHREAD_MUTEX_INITIALIZER;

void iniciar_log()
{
	log = log_create("Cliente pokedex", "Cliente",0,LOG_LEVEL_DEBUG);
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
}

void cliente_osada_conectate()
{
	cliente_osada->socket_pokedex_servidor = cliente_create(cliente_osada->puerto, cliente_osada->ip);
}

/*-------------------------------------------ATRIBUTOS-----------------------------------------------------------------*/
int cliente_pedi_atributos(const char *path, struct stat *buffer)
{
	char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"ATRIBUTOS: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(GET_ATRIBUTES,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int resultado = escuchar_y_modelar_atributos(buffer);
	pthread_mutex_unlock(&mutex_operaciones);
	return resultado;
}

int cliente_pedi_listado(const char *path, void *buffer, fuse_fill_dir_t filler)
{
	char *mensaje_A_log = string_new();
	string_append(&mensaje_A_log,"LISTAR: ");
	string_append(&mensaje_A_log,path);
	log_info(log,mensaje_A_log);
	free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(LISTAR,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int resultado = escuchar_listado(buffer,filler);
	pthread_mutex_unlock(&mutex_operaciones);
	return resultado;
}

/*-------------------------------------------CREACION-----------------------------------------------------------------*/
int cliente_pedi_crear_directorio(const char *path, mode_t modo_de_creacion)
{
	char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"CREAR DIRECTORIO: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(CREATE_DIRECTORY,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	pthread_mutex_unlock(&mutex_operaciones);
	return respuesta;
}

int cliente_pedi_crear_archivo(const char *path, mode_t modo, dev_t permisos)
{
	char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"CREAR ARCHIVO: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(CREATE_FILE,path,NULL,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	pthread_mutex_unlock(&mutex_operaciones);
	return respuesta;
}

/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
int cliente_pedi_eliminar(int tipo,const char *path)
{
	switch(tipo)
	{
		case(DIRECTORIO):
		{
			char *mensaje_A_log = string_new();
				string_append(&mensaje_A_log,"ELIMINAR DIRECTORIO: ");
				string_append(&mensaje_A_log,path);
				log_info(log,mensaje_A_log);
				free(mensaje_A_log);

			pthread_mutex_lock(&mutex_operaciones);
			char *msg = build_msg(DELETE_DIRECTTORY,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			int respuesta = escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
		case(ARCHIVO):
		{
			char *mensaje_A_log = string_new();
				string_append(&mensaje_A_log,"ELIMINAR ARCHIVO: ");
				string_append(&mensaje_A_log,path);
				log_info(log,mensaje_A_log);
				free(mensaje_A_log);

			pthread_mutex_lock(&mutex_operaciones);
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
	char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"LEER: ");
		string_append(&mensaje_A_log,path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(READ_FILE,path,NULL,size,offset);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	//char *datos = string_new();
	char *primer_byte = recibir_mensaje(cliente_osada->socket_pokedex_servidor,1);
	if(string_equals_ignore_case(primer_byte,"F"))
	{
		return 0;
	}
	else
	{
		char *lectura = recibir_mensaje(cliente_osada->socket_pokedex_servidor,9);
		string_append(&primer_byte,lectura);
		int tamanio = atoi(primer_byte);
		free(lectura);
		free(primer_byte);
		//char *lectura_final = recibir_mensaje(cliente_osada->socket_pokedex_servidor,tamanio);
		void *lectura_final = recibir_mensaje_tipo_indistinto(cliente_osada->socket_pokedex_servidor,tamanio);

				//string_append(&datos,lectura_final);
				//int leido=string_length(datos);

				/*char* contenido = string_new();
				contenido=(char*) lectura_final;
				contenido[tamanio-1] = '\0';*/
		//memcpy(buf,lectura_final,string_length(lectura_final)); //OJO ACA
		memcpy(buf,lectura_final,tamanio+1);
		free(lectura_final);
		pthread_mutex_unlock(&mutex_operaciones);
		return tamanio;
	}
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
	char *mensaje_A_log = string_new();
		string_append(&mensaje_A_log,"RENOMBRAR: ");
		string_append(&mensaje_A_log,old_path);
		string_append(&mensaje_A_log, new_path);
		log_info(log,mensaje_A_log);
		free(mensaje_A_log);

	pthread_mutex_lock(&mutex_operaciones);
	char *msg = build_msg(RENAME_FILE,(char*)old_path,(char*)new_path,NULL,NULL);
	enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
	int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
	pthread_mutex_unlock(&mutex_operaciones);
	return respuesta;
}

/*-------------------------------------------OPENS & CLOSER--------------------------------------------------------*/
int cliente_pedi_abrir(int tipo,const char *path, struct fuse_file_info *fi)
{
	switch(tipo)
	{
		case(ARCHIVO):
		{
			char *mensaje_A_log = string_new();
				string_append(&mensaje_A_log,"ABRIR ARCHIVO: ");
				string_append(&mensaje_A_log,path);
				log_info(log,mensaje_A_log);
				free(mensaje_A_log);

			pthread_mutex_lock(&mutex_operaciones);
			char *msg = build_msg(OPEN_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			fi->flags = FUSE_BUF_FORCE_SPLICE;
			int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
		case(DIRECTORIO):
		{
			char *mensaje_A_log = string_new();
				string_append(&mensaje_A_log,"ABRIR DIRECTORIO: ");
				string_append(&mensaje_A_log,path);
				log_info(log,mensaje_A_log);
				free(mensaje_A_log);

			pthread_mutex_lock(&mutex_operaciones);
			char *msg = build_msg(OPEN_FILE,path,NULL,NULL,NULL);
			enviar_mensaje(cliente_osada->socket_pokedex_servidor,msg);
			//fi->flags = FUSE_BUF_FORCE_SPLICE;
			int respuesta=escuchar_respuesta_comun(cliente_osada->socket_pokedex_servidor);
			pthread_mutex_unlock(&mutex_operaciones);
			return respuesta;
		};break;
	}
}






/*-------------------------------------------RESPUESTAS DEL SERVER------------------------------------------------*/

int lectura_sin_errores(char* data)
{
	char *msj = recibir_mensaje(cliente_osada->socket_pokedex_servidor, MAX_BYTES_TO_ADVISES);
	int respuesta = atoi(msj);
	if(respuesta == ARGUMENTO_INVALIDOO)
	{
		return NO_EXISTE;
	}
	else
	{
		if(respuesta == NO_EXISTEE)
		{
			return NO_EXISTE;
		}
		else
		{
			string_append(&data,msj);
			free(msj);
			return 1;
		}
	}
}


int escuchar_respuesta_comun(int socket_server)
{
	char* msj = recibir_mensaje(socket_server, MAX_BYTES_TO_ADVISES);
	int respuesta = atoi(msj);
	switch(respuesta)
	{
		case(EXITO): return OPERACION_EXITOSA; break;
		case(NO_EXISTEE): return NO_EXISTE; break;
		case(EXISTEE): return EXISTE; break;
		case(NO_HAY_ESPACIOO): return NO_HAY_ESPACIO;break;
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
