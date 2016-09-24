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

/*---------------------------------------SECUNDARIOS----------------------------------------*/
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
			char *tamanio_payload_a_enviar = string_repeat(' ',2);
			string_append(&tamanio_payload_a_enviar, string_itoa(tamanio_payload));
			string_append(&mensaje, tamanio_payload_a_enviar);
			string_append(&mensaje,";");
			string_append(&mensaje,payload);
			return mensaje;
		}
		else
		{
			char *tamanio_payload_a_enviar = string_repeat(' ',1);
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

void copiar(char* origen, char* destino)
{
	string_path_replace_spaces(origen, " ", "\\");
	string_path_replace_spaces(destino, " ","\\");
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
		char* mensaje = string_new();
		string_append(&mensaje, "rm ");
		string_append(&mensaje, elemento);
		system(mensaje);
		free(mensaje);
}

char* armar_coordenada(int x, int y)
{
	char *coordenada_x = string_itoa(x);
	char *coordenada_y = string_itoa(y);
	int longitud_eje_x = strlen(coordenada_x);
	int longitud_eje_y = strlen(coordenada_y);
	if(longitud_eje_x < (MAX_BYTES_COORDENADA))
	{
		char *nueva_coordenada_x = string_repeat(' ', MAX_BYTES_COORDENADA - longitud_eje_x);
		string_append(&nueva_coordenada_x, coordenada_x);
		if(longitud_eje_y < (MAX_BYTES_COORDENADA))
		{
			char *nueva_coordenada_y = string_repeat(' ', (MAX_BYTES_COORDENADA - longitud_eje_y));
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
					char *nueva_coordenada_y = string_repeat(' ', (MAX_BYTES_COORDENADA - longitud_eje_y));
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

t_ubicacion* desarmar_coordenada(char *coordenada)
{
	char **por_separado = string_split(coordenada, ";");
	string_trim_left(&por_separado[0]);
	string_trim_left(&por_separado[1]);
	t_ubicacion *ubi = ubicacion_create(atoi(por_separado[0]),atoi(por_separado[1]));
	return (ubi);
	free_string_array(por_separado);

}

void string_replace(char *palabra, char *este_caracter,char *por_este)
{
	char **por_separado=string_split(palabra, este_caracter);
	char *palabra_new= string_new();
	int i = 0;
	while(por_separado[i] !=NULL)
	{
		string_append(&palabra_new, por_separado[i]);
		string_append(&palabra_new,por_este);
		i++;
	}
	strcpy(palabra, palabra_new);
	free_string_array(por_separado);
	free(palabra_new);
}

int string_contais(char *palabra, char *conteiner)
{
	int i =0;
	int esta_en_palabra =0;
	while((palabra[i] != NULL) && !esta_en_palabra)
	{
		if(palabra[i] == conteiner[0])
		{
			esta_en_palabra = 1;
		}
	}
	return esta_en_palabra;
}

void string_path_replace_spaces(char *path, char *este_caracter, char *por_este)
{
	char **por_separado = string_split(path,"/");
	int i = 0;
	char *aux = string_new();
	while(por_separado[i] != NULL)
	{
		if(string_contais(por_separado[i],este_caracter))
		{
			string_replace(por_separado[i],este_caracter,por_este);
		}
		string_append(&aux, por_separado[i]);
		i++;
	}
	strcpy(path, aux);
	free(aux);
	free_string_array(por_separado);
}

void free_string_array(char **path)
{
	int i =0;
	while(path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
}



