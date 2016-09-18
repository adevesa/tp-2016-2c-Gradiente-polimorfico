/*
 * comunication.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "comunication.h"
/*--------------------------------------CONEXION CON EL SERVIDOR--------------------------------------------------*/
int conectar_a_mapa(t_mapa *mapa)
{
	mapa->server= conectar_a_servidor(atoi(mapa->puerto),mapa->ip);

	return mapa->server;
}

void entrenador_dale_a_mapa_tu_simbolo()
{
	extern t_entrenador *entrenador;
	enviar_mensaje(entrenador->mapa_actual->server, entrenador->simbolo);
}
/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
void tratar_respuesta(char* respuesta_del_entrenador,t_entrenador *entrenador)
{
	if(string_equals_ignore_case(respuesta_del_entrenador, "tr"))
	{
		mapa_me_da_turno(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador,"ur"))
	{
		mapa_me_da_coordendas_pokenest(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "sr"))
	{
			mapa_me_da_pokemon(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "mr"))
	{
		mapa_me_da_medalla(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "bq"))
	{
		mapa_me_bloqueo(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "fb"))
	{
		mapa_me_desbloqueo(entrenador);
	}
}
/*--------------------------------------ESPERAR RESPUESTA DEL MAPA----------------------------------------*/
char* esperar_respuesta(t_mapa *mapa)
{
	char *resp = recibir_mensaje(mapa->server, 3);
	return resp;
}

/*--------------------------------------MAPA NOS DA TURNO----------------------------------------*/

void mapa_me_da_turno(t_entrenador *entrenador)
{
	//SOLO CONTINUA EJECUTANDO
}

/*--------------------------------------MAPA ME DA COORDENADA POKENEST----------------------------------------*/

void mapa_me_da_coordendas_pokenest(t_entrenador *entrenador)
{
	char * coordendas_array = recibir_mensaje(entrenador->mapa_actual->server, 10);
	decodificar_coordenadas(coordendas_array, &(entrenador->pokenest->x),&(entrenador->pokenest->y));

}

void decodificar_coordenadas(char *payload, int *x, int*y)
{
	string_trim(&payload);
	char **coordenadas= string_split(payload, ",");
	*x = atoi(coordenadas[0]);
	*y= atoi(coordenadas[1]);
}


/*--------------------------------------MAPA ME DA POKEMON----------------------------------------*/

void mapa_me_da_pokemon(t_entrenador *entrenador)
{
	char *ruta_para_copiar_pokemon = recibir_mensaje(entrenador->mapa_actual->server, 80);
	copiar(entrenador->directorio_de_bill, ruta_para_copiar_pokemon);
}


/*--------------------------------------MAPA ME DA MEDALLA----------------------------------------*/

void mapa_me_da_medalla(t_entrenador *entrenador)
{
	char *ruta_para_copiar_medalla = recibir_mensaje(entrenador->mapa_actual->server, 80);
	copiar(entrenador->directorio_de_bill, ruta_para_copiar_medalla);
}


/*--------------------------------------MAPA ME BLOQUEO-----------------------------------------------*/
int mapa_me_bloqueo(t_entrenador *entrenador)
{
	entrenador->cantidad_de_veces_bloqueado++;
	return 1;
}

/*--------------------------------------MAPA ME DESBLOQUEO-----------------------------------------------*/
int mapa_me_desbloqueo(t_entrenador *entrenador)
{
	return 1;
}




/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR AL MAPA----------------------------------------*/
void solicitar_ubicacion_pokenest(t_mapa *mapa,char *pokemonBuscado)
{
	char *mensaje = armar_mensaje("up", pokemonBuscado);
	enviar_mensaje(mapa->server, mensaje);
}

void solicitar_moverse(t_mapa *mapa,char *coordenadaDestino)
{
	char *mensaje = armar_mensaje("mp", coordenadaDestino);
	enviar_mensaje(mapa->server, mensaje);
}

void solicitar_captura_pokemon(t_mapa *mapa, char *pokemonAcapturar)
{
	char *mensaje = armar_mensaje("cp", pokemonAcapturar);
	enviar_mensaje(mapa->server, mensaje);
}

void notificar_fin_objetivos(t_mapa *mapa)
{
	enviar_mensaje(mapa->server, "fp");
	close(mapa->server);
}



/*---------------------------------------SECUNDARIOS----------------------------------------*/
char* armar_mensaje(char *header, char *payload)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	string_append(&mensaje,payload);
	return mensaje;
}

void copiar(char* origen, char* destino)
{
	char* mensaje = string_new();
	string_append(&mensaje, "cp ");
	string_append(&mensaje, origen);
	string_append(&mensaje, " ");
	string_append(&mensaje, destino);
	system(mensaje);
	free(mensaje);
}

void eliminar(char* elemento){
		char* mensaje = string_new();
		string_append(&mensaje, "rm ");
		string_append(&mensaje, elemento);
		system(mensaje);
		free(mensaje);
}











