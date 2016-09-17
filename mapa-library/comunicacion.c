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
		return 1;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador,"mp"))
	{
		return 2;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "cp"))
	{
		return 3;
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "fp"))
	{
		return 4;
	}
	else {return 0;}
}

/*-------------------------------------------OTORGAR TURNO-----------------------------------------------------------*/
void otorgar_turno_a_entrenador(t_entrenador *entrenador)
{
		char *mensaje = armar_mensaje("tr", "");
		enviar_mensaje(entrenador->socket_entrenador, mensaje);
}

char* armar_mensaje(char *header, char *payload)
{
	char *mensaje =string_new();
	string_append(&mensaje,header);
	string_append(&mensaje,";");
	string_append(&mensaje,payload);
	return mensaje;
}

/*-------------------------------------------ESCUCHA AL ENTRENADOR-----------------------------------------------------------*/
char* escuchar_al_entrenador(t_entrenador *entrenador)
{
	return recibir_mensaje(entrenador->socket_entrenador, 3);
}
/*-------------------------------------------RECIBE Y RESPONDE-----------------------------------------------------------*/

/*--------------------UBICACION POKENEST---------------------------------------------------------------------------------*/
void darle_coordenadas_pokenst_a_entrenador(t_entrenador *entrenador)
{
	char *pokemon_buscado = string_new();
	pokemon_buscado=escuchar_que_pokemon_busca(entrenador);
	t_pokeNest *pokenest_buscado = mapa_buscame_pokenest(pokemon_buscado);
	otorgar_posicion_pokenest_a_entrenador(entrenador, pokenest_buscado);
}

char* escuchar_que_pokemon_busca(t_entrenador *entrenador)
{
	char *pokemon_buscado = string_new();
	pokemon_buscado=recibir_mensaje(entrenador->socket_entrenador,15);
	return pokemon_buscado;
}

void otorgar_posicion_pokenest_a_entrenador(t_entrenador *entrenador, t_pokeNest *pokenest_buscado)
{
	entrenador->pokenest_objetivo = pokenest_buscado;
	char *posx = string_itoa(pokenest_buscado->posicion->x);
	char *posy =string_itoa(pokenest_buscado->posicion->y);
	char *posicion = armar_mensaje(posx, posy);
	char *mensaje = armar_mensaje("ur", posicion);
	enviar_mensaje(entrenador->socket_entrenador, mensaje);
	free(posx);
	free(posy);
	free(posicion);
}


/*--------------------MOVIMIENTO DEL ENTRENADOR---------------------------------------------------------------------------*/
void escuchar_a_que_direccion_se_mueve_entrenador(t_entrenador *entrenador)
{
	char *coordenadas = string_new();
	coordenadas = recibir_mensaje(entrenador->socket_entrenador, 15);
	decodificar_coordenadas(coordenadas, &(entrenador->posicion_actual->x), &(entrenador->posicion_actual->y));
}

void decodificar_coordenadas(char *payload, int *x, int*y)
{
	string_trim(&payload);
	char **coordenadas= string_split(payload, ",");
	*x = atoi(coordenadas[0]);
	*y= atoi(coordenadas[1]);
	free(payload);
}


/*--------------------CAPTURA POKEMON---------------------------------------------------------------------------------------*/
char* escuchar_captura_pokemon(int entrenador)
{
	char *payload = string_new();
	payload=recibir_mensaje(entrenador, 15);
	return payload;
}

void avisar_bloqueo_a_entrenador(int entrenador)
{
	char *mensaje = armar_mensaje("bq", "");
	enviar_mensaje(entrenador, mensaje);
}

/*----------------OTORGANDO POKEMON------------------------------------------------------------------------------------------*/
void dar_pokemon_a_entrenador(t_entrenador *entrenador, t_pokemon *pokemon)
{
	avisar_desbloqueo_a_entrenador(entrenador->socket_entrenador);
	otorgar_pokemon_a_entrenador(entrenador, pokemon);
}

void otorgar_pokemon_a_entrenador(t_entrenador *entrenador,t_pokemon *pokemon)
{
	char *mensaje = armar_mensaje("sr", pokemon->ruta_en_pokedex);
	list_add(entrenador->pokemones_capturados, pokemon);
	enviar_mensaje(entrenador->socket_entrenador, mensaje);
}

void avisar_desbloqueo_a_entrenador(int entrenador)
{
	enviar_mensaje(entrenador, "fb");
}

void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla)
{
	char *mensaje = armar_mensaje("mr", rutaMedalla);
	enviar_mensaje(entrenador, mensaje);
}



