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
	mapa->server= cliente_create(atoi(mapa->puerto),mapa->ip);

	char *mensaje = string_new();
	string_append(&mensaje, "CONECTADO con ");
	string_append(&mensaje, mapa->nombre);
	log_info(info_entrenador,mensaje);
	free(mensaje);

	return mapa->server;
}

int mapa_me_dice(char *mapa_dice)
{
	if(string_equals_ignore_case(mapa_dice, "sr;"))
	{
		return MAPA_ME_DA_POKEMON;
	}
	if(string_equals_ignore_case(mapa_dice, "mr;"))
	{
		return MAPA_ME_DA_MEDALLA;
	}
	if(string_equals_ignore_case(mapa_dice, "bq;"))
	{
		return MAPA_ME_BLOQUEA;
	}
	if(string_equals_ignore_case(mapa_dice, "fb;"))
	{
			return MAPA_ME_DESBLOQUEA;
	}
	if(string_equals_ignore_case(mapa_dice, "ur;"))
	{
		return MAPA_ME_DA_COORDENADAS_POKENEST;
	}
	else return 0;
}

void enviar_mensaje_a_mapa(t_mapa *mapa, int header, char *payload)
{
	switch(header)
	{
		case(OTORGAR_SIMBOLO_ENTRENADOR): enviar_mensaje(mapa->server,payload);break;
		case(SOLICITAR_COORDENADAS_POKENEST): solicitar_ubicacion_pokenest(mapa, payload);break;
		case(SOLICITAR_CAPTURA_POKEMON):enviar_mensaje(mapa->server, "cp;");break;
		case(REPORTAR_MOVIMIENTO): solicitar_moverse(mapa, payload);break;
		case(REPORTAR_FIN_OBJETIVOS):enviar_mensaje(mapa->server, "fp;");
	}
}

char* escuchar_mensaje_mapa(t_mapa *mapa, int header)
{
	switch(header)
	{
		case(MAPA_ME_DA_TURNO): return( recibir_mensaje(mapa->server, 3)); break;
		case(MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR): return(recibir_mensaje(mapa->server,3)); break;
		case(MAPA_ME_DA_POKEMON): return(recibir_mensaje_especifico(mapa->server)); break;
		case(MAPA_ME_DA_COORDENADAS_POKENEST): return(recibir_mensaje_especifico(mapa->server)); break;
		case(MAPA_ME_DA_MEDALLA): return(recibir_mensaje_especifico(mapa->server)); break;
		case(MAPA_ME_BLOQUEA): return(recibir_mensaje(mapa->server, 3)); break;
		case(MAPA_ME_DESBLOQUEA): return(recibir_mensaje(mapa->server,3));break;
		default: return("NO SE PUEDE INTERPRETAR");
	}
}

void entrenador_dale_a_mapa_tu_simbolo()
{
	extern t_entrenador *entrenador;
	enviar_mensaje(entrenador->mapa_actual->server, entrenador->simbolo);
}

/*---------------------------------------MENSAJES QUE ENVIA EL ENTRENADOR AL MAPA----------------------------------------*/
void solicitar_ubicacion_pokenest(t_mapa *mapa,char *pokemonBuscado)
{
	char *mensaje = armar_mensaje("up", pokemonBuscado,MAX_BYTES_TOTAL_A_ENVIAR);
	enviar_mensaje(mapa->server, mensaje);
}

void solicitar_moverse(t_mapa *mapa,char *coordenadaDestino)
{
	char *mensaje = armar_mensaje("mp", coordenadaDestino, MAX_BYTES_TOTAL_A_ENVIAR);
	enviar_mensaje(mapa->server, mensaje);
}


/*---------------------------------------SECUNDARIOS----------------------------------------*/
t_ubicacion* desarmar_coordenada(char *coordenada)
{
	char **por_separado = string_split(coordenada, ";");
	string_trim_left(&por_separado[0]);
	string_trim_left(&por_separado[1]);
	t_ubicacion *ubi = ubicacion_create(atoi(por_separado[0]),atoi(por_separado[1]));
	return (ubi);
	free_string_array(por_separado);

}

void copiar(char* origen, char* destino)
{
	string_path_replace_spaces(origen, " ", "\ ");
	string_path_replace_spaces(destino, " ","\ ");
	char* mensaje = string_new();
	string_append(&mensaje, "cp ");
	string_append(&mensaje, origen);
	string_append(&mensaje, " ");
	string_append(&mensaje, destino);
	system(mensaje);
	free(mensaje);
}

void eliminar(char* elemento)
{
		string_path_replace_spaces(elemento, " ", "\ ");
		char* mensaje = string_new();
		string_append(&mensaje, "rm ");
		string_append(&mensaje, elemento);
		system(mensaje);
		free(mensaje);
}

void borrar_todos_los_archivos_del_directorio(char* ruta) // SUPER PELIGROSISIMA.
{
	string_path_replace_spaces(ruta," ","\ ");
	char* mensaje = string_new();
	string_append(&mensaje,"find ");
	string_append(&mensaje,ruta);
	string_append(&mensaje," -type f -exec rm -f \"{}\" \+");
	system(mensaje);
	free(mensaje);

}
