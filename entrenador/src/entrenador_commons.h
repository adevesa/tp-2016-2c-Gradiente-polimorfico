/*
 * entrenador_commons.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef ENTRENADOR_COMMONS_H_
#define ENTRENADOR_COMMONS_H_

#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <stdbool.h>
#include <commons/config.h>
#include <commons/txt.h>

typedef struct coordenada{
	int x;
	int y;
	int pa; //paso anterior
}t_ubicacion;

typedef struct mapa{
	int server;
	t_list* objetivos;
} t_mapa;

typedef struct entrenador{
	char* nombre;
	char* ruta_pokedex;
	t_config* configuracion;
	char* simbolo;
	t_list* mapas;
	t_mapa* mapa_actual;
	int vidas;
	int reintentos;
	int tiempo_total_aventura;
	int tiempo_bloqueado_pokenest;
	int muertes;
	t_ubicacion* ubicacion;
	t_ubicacion* pokenest;
}t_entrenador;

void analizar_paso_anterior(t_entrenador* entrenador_new);
t_config* configuracion_metadata_create(char *nombre, char *ruta);
t_entrenador* entrenador_create(char* nombre, char* ruta);
char* entrenador_simbolo(t_config* configuracion);
int entrenador_vidas(t_config* configuracion);
t_list* entrenador_mapas(t_config* configuracion);
t_mapa* mapa_actual(t_entrenador* entrenador, char* mapa);
void conocer_siguiente_pokenest(t_entrenador* entrenador);
void decodificar_coordenadas(char *payload, int *x, int*y);
int tengo_pokemons_por_atrapar(t_entrenador entrenador);
int conozco_el_pokenest(t_entrenador entrenador);
void mando_el_fin(t_entrenador entrenador);
void moverme(t_entrenador* entrenador);
void choque_con_pokenest(t_entrenador entrenador);
void capturar_pokemon(t_entrenador entrenador);
void setear_a_null_pokenest(t_entrenador entrenador);
void ejecutar_lo_que_deba_hacer(t_entrenador *entrenador);
void ejecutar_entrenador(char *nombre_entrenador, char *rutaPokedex);

#endif /* ENTRENADOR_COMMONS_H_ */
