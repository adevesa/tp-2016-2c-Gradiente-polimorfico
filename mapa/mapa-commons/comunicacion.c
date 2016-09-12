/*
 * comunicacion.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */

#include "comunicacion.h"

/*-------------------------------------------DECODIFICACION DE RESPUESTAS------------------------------------------------*/
void tratar_respuesta(char* respuesta_del_entrenador, t_entrenador *entrenador)
{
	if(string_equals_ignore_case(respuesta_del_entrenador, "up"))
	{
		entrenador_busca_coordenadas_pokenest(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador,"mp"))
	{
		entrenador_se_va_a_mover(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "cp"))
	{
		entrenador_quiere_capturar_pokemon(entrenador);
	}
	if(string_equals_ignore_case(respuesta_del_entrenador, "fp"))
	{
		entrenador_quiere_finalizar_objetivos(entrenador);
	}
}

/*-------------------------------------------OTORGAR TURNO-----------------------------------------------------------*/
void otorgar_turno_a_entrenador(t_entrenador *entrenador)
{
		char *mensaje = armar_mensaje("tr", "");
		enviar_mensaje(entrenador->socket_etrenador, mensaje);
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
	return recibir_mensaje(entrenador->socket_etrenador, 3);
}
/*-------------------------------------------RECIBE Y RESPONDE-----------------------------------------------------------*/

/*--------------------UBICACION POKENEST---------------------------------------------------------------------------------*/
void entrenador_busca_coordenadas_pokenest(t_entrenador *entrenador)
{
	char *pokemon_buscado = string_new();
	escuchar_que_pokemon_busca(entrenador);
	t_posicion *posicion = buscar_coordenadas(pokemon_buscado);
	otorgar_posicion_pokenest_a_entrenador(entrenador->socket_etrenador, posicion->x, posicion->y);
}

//CREAR BUSCAR_COORDENADAS

char* escuchar_que_pokemon_busca(t_entrenador *entrenador)
{
	char *pokemon_buscado = string_new();
	pokemon_buscado=recibir_mensaje(entrenador->socket_etrenador,15);
	return pokemon_buscado;
}

void otorgar_posicion_pokenest_a_entrenador(int entrenador, int x, int y)
{
	char *posx = string_itoa(x);
	char *posy =string_itoa(y);
	char *posicion = armar_mensaje(posx, posy);
	char *mensaje = armar_mensaje("ur", posicion);
	enviar_mensaje(entrenador, mensaje);
	free(posx);
	free(posy);
	free(posicion);
}


/*--------------------MOVIMIENTO DEL ENTRENADOR---------------------------------------------------------------------------*/

void entrenador_se_va_a_mover(t_entrenador *entrenador)
{
	escuchar_a_que_direccion_se_mueve(entrenador);
}

void escuchar_a_que_direccion_se_mueve(t_entrenador *entrenador)
{
	char *coordenadas = string_new();
	coordenadas = recibir_mensaje(entrenador->socket_etrenador, 15);
	decodificar_coordenadas(coordenadas, &(entrenador->posicion_actual->x), &(entrenador->posicion_actual->y));
}

void decodificar_coordenadas(char *payload, int *x, int*y)
{
	string_trim(&payload);
	char **coordenadas= string_split(payload, ",");
	*x = atoi(coordenadas[0]);
	*y= atoi(coordenadas[1]);
}


/*--------------------CAPTURA POKEMON---------------------------------------------------------------------------------------*/
//OJO ACA, FALTA LA LOGICA DE BLOQUEO//
void entrenador_quiere_capturar_pokemon(t_entrenador *entrenador)
{
	char *pokemon_que_quiere_capturar = escuchar_captura_pokemon(entrenador->socket_etrenador);
	//planificador_bloquea_a(entrenador, pokemon_que_quiere_capturar)
	avisar_bloqueo_a_entrenador(entrenador->socket_etrenador);
}

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
void dar_pokemon_a_entrenador(t_entrenador *entrenador, t_pokemon *pokemon, int tiempo_bloqueado)
{
	avisar_desbloqueo_a_entrenador(entrenador->socket_etrenador, tiempo_bloqueado);
	otorgar_pokemon_a_entrenador(entrenador->socket_etrenador, pokemon->nivel);
}

void otorgar_pokemon_a_entrenador(int entrenador, int nivelPokemon)
{
	char *nivel = string_itoa(nivelPokemon);
	char *mensaje = armar_mensaje("sr", nivel);
	enviar_mensaje(entrenador, mensaje);
	free(nivel);
}

void avisar_desbloqueo_a_entrenador(int entrenador, int tiempo_bloqueado)
{
	char *tiempo_bloq = string_itoa(tiempo_bloqueado);
	char *mensaje = armar_mensaje("fb", tiempo_bloq);
	enviar_mensaje(entrenador, mensaje);;
	free(tiempo_bloq);
}


/*--------------------FIN DE OBJETIVOS DE ENTRENADOR-------------------------------------------------------------------------*/
//COMPLETAR LA BUSQUEDA DE MEDALLA DEL MAPA!
void entrenador_quiere_finalizar_objetivos(t_entrenador *entrenador)
{
	char *ruta_medalla_del_mapa = buscar_medalla_del_mapa();
	otorgar_ruta_medalla_a_entrenador(entrenador->socket_etrenador, ruta_medalla_del_mapa);
}

void otorgar_ruta_medalla_a_entrenador(int entrenador, char *rutaMedalla)
{
	char *mensaje = armar_mensaje("mr", rutaMedalla);
	enviar_mensaje(entrenador, mensaje);
}



