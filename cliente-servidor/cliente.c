/*
 * cliente.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */
#include "cliente.h"

/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_mapa(char *mapa, char *rutaPokedex)
{
	char *ruta_final = obtener_ruta_especifica(rutaPokedex, "Mapas", mapa);
	ruta_final = obtener_ruta_especifica(ruta_final,"metadata", NULL);
	t_config *config = config_create(ruta_final);
	int puerto = config_get_int_value(config, "Puerto");
	char *ip = config_get_string_value(config, "IP");
	int server = conectar_a_servidor(puerto, ip);
	config_destroy(config);
	return server;
}

int conectar_a_servidor(int puerto, char *ip)
{
	//Configuro direccion de servidor//
	address_config_in direccionServidor = configurar_address_in(puerto, ip);

	int server = socket(AF_INET,SOCK_STREAM,0);

	//Conexión con el servidor//
	int resultConect = connect(server, (void*) &direccionServidor,sizeof(direccionServidor));
	if(resultConect !=0)
		{perror("Error de conexión");}
	return server;
}

/*--------------------------------------MENSAJES QUE RECIBE EL ENTRENADOR----------------------------------------*/

int esperar_turno(int server)
{
	char *header = malloc(3);
	recv(server, header, 2,0);
	header[2]= '\0';
	if(strcmp(header,"tr"))
	{
		return TRUE;
		free(header);
	}
	return FALSE;
	free(header);
}

char* esperar_coordenadas_pokenest(int server)
{
	char *header = malloc(3);
	recv(server, header, 2,0);
	header[2]= '\0';

	int payloadsize;
	if(strcmp(header,"ur")) payloadsize = 10;

	char *payload = malloc(payloadsize);
	recv(server, payload, payloadsize,0);

	return payload;
	free(header);
	//DESPUES DE DECOFIFICAR LA COORDENADA, DEBE LIBERARSE EL PUNTERO
}

char* esperar_por_pokemon(int server)
{
	char *header = malloc(3);
	recv(server, header, 2,0);
	header[2]= '\0';

	int payloadsize;
	if(strcmp(header,"sr")) payloadsize = 5;

	char *payload = malloc(payloadsize);
	recv(server, payload, payloadsize,0);

	return payload;
	free(header);
	//DESPUES DE DECOFIFICAR Y CREAR EL POKEMON DEBE LIBERARSE EL PUNTERO
}

char* esperar_por_medalla(int server)
{
	char *header = malloc(3);
		recv(server, header, 2,0);
		header[2]= '\0';

		int payloadsize;
		if(strcmp(header,"sr")) payloadsize = 180;

		char *payload = malloc(payloadsize);
		recv(server, payload, payloadsize,0);

		return payload;
		free(header);
		//DESPUES DE DECOFIFICAR LA RUTA DEBE LIBERARSE EL PUNTERO
}

char* esperar_respuesta_captura_pokemon(int server)
{
	char *header = malloc(3);
	recv(server, header, 2,0);
	header[2]= '\0';
	if(strcmp(header,"bq"))
	{
		return "bloqueado";
	}
	return "no bloqueado";
}

int esperar_por_desbloqueo(int server)
{
	char *header = malloc(3);
	recv(server, header, 2,0);
	header[2]= '\0';

	int payloadsize;
	if(strcmp(header,"fb")) payloadsize = 20;

	char *payload = malloc(payloadsize);
	recv(server, payload, payloadsize,0);
	string_trim_right(&payload);
	int retardo_bloqeado =  atoi(payload);
	return retardo_bloqeado;
	free(payload);
	free(header);
}

/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR AL MAPA----------------------------------------*/
void solicitar_ubicacion_pokenest(int server,char *pokemonBuscado)
{
	char *mensaje = armar_mensaje("up", pokemonBuscado);
	send(server, mensaje, strlen(mensaje)+1,0);
	free(mensaje);
}

void solicitar_moverse(int server,char *coordenadaDestino)
{
	char *mensaje = armar_mensaje("mp", coordenadaDestino);
	send(server, mensaje, strlen(mensaje)+1,0);
	free(mensaje);
}

void solicitar_captura_pokemon(int server, char *pokemonAcapturar)
{
	char *mensaje = armar_mensaje("cp", pokemonAcapturar);
	send(server, mensaje, strlen(mensaje)+1,0);
	free(mensaje);
}

void notificar_fin_objetivos(int server)
{
	send(server, "fp", 2,0);
	close(server);
}



/*-------------------------------------------------------FUNCIONES SECUNDARIAS----------------------------------------------*/
address_config_in configurar_address_in(int puerto, char *ip)
{
	address_config_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr=inet_addr(ip);
	direccionServidor.sin_port=htons(puerto);
	memset(&(direccionServidor.sin_zero), '\0', 8);
	return direccionServidor;

}

void activar_reutilizacion_de_direcciones(int valorVerdaderoso, int socket)
{
	setsockopt(socket,SOL_SOCKET,SO_REUSEADDR, &valorVerdaderoso, sizeof(valorVerdaderoso));
}
char* armar_mensaje(char *header, char *payload)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	string_append(&mensaje,payload);
	return mensaje;
}

char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo)
{
	char* ruta = string_new();
	string_append(&ruta,ruta_inicial);
	string_append(&ruta, "/");
	string_append(&ruta, directorio_o_nombre_archivo);
	if(sub_directorio_o_nombre_archivo != NULL)
	{	string_append(&ruta, "/");
		string_append(&ruta,sub_directorio_o_nombre_archivo);
		return ruta;
		free(ruta);
	}
	else return ruta;
	free(ruta);
}

