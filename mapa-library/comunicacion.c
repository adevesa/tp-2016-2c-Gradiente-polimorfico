/*
 * comunicacion.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "comunicacion.h"

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
int tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador)
{
	if(string_equals_ignore_case(respuesta_del_entrenador, "up"))
	{
		return ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador,"mp"))
	{
		return ENTRENADOR_QUIERE_MOVERSE;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "cp"))
	{
		return ENTRENADOR_QUIERE_CAPTURAR_POKEMON;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "fp"))
	{
		return ENTRENADOR_FINALIZO_OBJETIVOS;
	}
	else {return 0;}
}

void enviar_mensaje_a_entrenador(t_entrenador *entrenador, int header, char *payload)
{
	switch(header)
	{
		case(OTORGAR_TURNO): enviar_mensaje(entrenador->socket_entrenador, "tr"); break;
		case(OTORGAR_COORDENADAS_POKENEST): enviar_mensaje(entrenador->socket_entrenador, armar_mensaje("ur",payload)); break;
		case(OTORGAR_MEDALLA_DEL_MAPA): otorgar_ruta_medalla_a_entrenador(entrenador->socket_entrenador, mapa_dame_medalla()); break;
		case(OTORGAR_POKEMON): dar_pokemon_a_entrenador(entrenador, payload);break;
		case(AVISAR_BLOQUEO_A_ENTRENADOR): enviar_mensaje(entrenador->socket_entrenador, "bq");  break;
		case(AVISAR_DESBLOQUEO_A_ENTRENADOR): enviar_mensaje(entrenador->socket_entrenador,"fb"); break;
		default: enviar_mensaje(entrenador->socket_entrenador, "fb");;
	}
}

char* escuchar_mensaje_entrenador(t_entrenador *entrenador, int header)
{
	switch(header)
	{
		case(SOLICITUD_DEL_ENTRENADOR): return(recibir_mensaje(entrenador->socket_entrenador,2));
		case(ENTRENADOR_OTORGA_SU_SIMBOLO): return(recibir_mensaje(entrenador->socket_entrenador,2)); break;
		case(ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST): return(recibir_mensaje(entrenador->socket_entrenador,2)); break;
		case(ENTRENADOR_QUIERE_MOVERSE): return(recibir_mensaje(entrenador->socket_entrenador,11)); break;
		default: return("0"); break;
	}
}

char* armar_mensaje(char *header, char *payload)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	int tamanio_payload = strlen(payload);
	if(tamanio_payload < MAX_BYTES_TOTAL_A_ENVIAR)
	{
		if(tamanio_payload < 10)
		{
			char *tamanio_payload_a_enviar = string_repeat('0',2);
			string_append(&tamanio_payload_a_enviar, string_itoa(tamanio_payload));
			string_append(&mensaje, tamanio_payload_a_enviar);
			string_append(&mensaje,";");
			string_append(&mensaje,payload);
			return mensaje;
		}
		else
		{
			char *tamanio_payload_a_enviar = string_repeat('0',1);
			string_append(&tamanio_payload_a_enviar, string_itoa(tamanio_payload));
			string_append(&mensaje, tamanio_payload_a_enviar);
			string_append(&mensaje,";");
			string_append(&mensaje,payload);
			return mensaje;
		}

	}
	else
	{
		string_append(&mensaje, string_itoa(MAX_BYTES_TOTAL_A_ENVIAR));
		string_append(&mensaje,";");
		string_append(&mensaje,payload);
		return mensaje;
	}
}

void dar_pokemon_a_entrenador(t_entrenador *entrenador, char *ruta_pokemon)
{
	enviar_mensaje_a_entrenador(entrenador, AVISAR_DESBLOQUEO_A_ENTRENADOR, NULL);
	char *mensaje = armar_mensaje("sr",ruta_pokemon);
	enviar_mensaje(entrenador->socket_entrenador, mensaje);
}

void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla)
{
	char *mensaje = armar_mensaje("mr", rutaMedalla);
	enviar_mensaje(entrenador, mensaje);
}

char* armar_coordenada(int x, int y)
{
	char *coordenada_x = string_itoa(x);
	char *coordenada_y = string_itoa(y);
	int longitud_eje_x = strlen(coordenada_x);
	int longitud_eje_y = strlen(coordenada_y);
	if(longitud_eje_x < (MAX_BYTES_COORDENADA))
	{
		char *nueva_coordenada_x = string_repeat('0', MAX_BYTES_COORDENADA - longitud_eje_x);
		string_append(&nueva_coordenada_x, coordenada_x);
		if(longitud_eje_y < (MAX_BYTES_COORDENADA))
		{
			char *nueva_coordenada_y = string_repeat('0', (MAX_BYTES_COORDENADA - longitud_eje_y));
			string_append(&nueva_coordenada_y, coordenada_y);
			char *coordenada_final = string_new();
			string_append(&coordenada_final, nueva_coordenada_x);
			string_append(&coordenada_final, ";");
			string_append(&coordenada_final, nueva_coordenada_y);
			return coordenada_final;
		}
		else
		{
			string_append(&coordenada_y, coordenada_y);
			char *coordenada_final = string_new();
			string_append(&coordenada_final, nueva_coordenada_x);
			string_append(&coordenada_final, ";");
			string_append(&coordenada_final,coordenada_y);
			return coordenada_final;
		}

	}
	else
	{
		string_append(&coordenada_x, coordenada_x);
				if(longitud_eje_y < (MAX_BYTES_COORDENADA))
				{
					char *nueva_coordenada_y = string_repeat('0', (MAX_BYTES_COORDENADA - longitud_eje_y));
					string_append(&nueva_coordenada_y, coordenada_y);
					char *coordenada_final = string_new();
					string_append(&coordenada_final,coordenada_x);
					string_append(&coordenada_final, ";");
					string_append(&coordenada_final, nueva_coordenada_y);
					return coordenada_final;
				}
				else
				{
					string_append(&coordenada_y, coordenada_y);
					char *coordenada_final = string_new();
					string_append(&coordenada_final,coordenada_x);
					string_append(&coordenada_final, ";");
					string_append(&coordenada_final,coordenada_y);
					return coordenada_final;
				}
	}

}

t_posicion* desarmar_coordenada(char *coordenada)
{
	char **por_separado = string_split(coordenada, ";");
	string_trim_left(&por_separado[0]);
	string_trim_right(&por_separado[1]);
	return (posicion_create(atoi(por_separado[0]),atoi(por_separado[1])));

}
