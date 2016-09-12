/*
 * map-commons.c
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */
#include "map-commons.h"



/*------------------------------------------EXECUTE----------------------------------------------------------------*/
void ejecutar_mapa(char *nombre, char *rutaPokedex)
{
	t_mapa *mapa = mapa_create(nombre, rutaPokedex);
	planificador_create_segun_cual_seas();
	nivel_gui_inicializar();
	mapa_hacete_visible_para_entrenadores();


}


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* buscar_coordenadas(char* pokemon_buscado)
{
	extern t_mapa *mapa;
	t_pokeNest *pokenest = list_find(mapa->pokeNests, pokenest_tipo);
	return pokenest->posicion;
}

bool pokenest_tipo(void *arg)
{
	t_pokeNest *pokenest = (t_pokeNest*) arg;
	extern char* pokemon_buscado;
	return string_equals_ignore_case(pokenest->identificador, pokemon_buscado);
}


char* buscar_medalla_del_mapa()
{
	extern t_mapa *mapa;
	return obtener_ruta_especifica(mapa->ruta_pokedex, "Mapas",mapa->nombre);
}

/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/


/*------------------------------ FUNCIONES PARA MANIPULACION DEL PLANIFICADOR--------------------------------------------*/
void planificador_create_segun_cual_seas()
{
	extern t_mapa *mapa;
	pthread_t thread;
	if(string_equals_ignore_case(mapa->info_algoritmo->algoritmo, "RR"))
	{
		pthread_create(&thread,NULL,ejecutar_planificador_rr, NULL);
	}
		pthread_create(&thread,NULL,ejecutar_planificador_srdf,NULL);
}


/*----------------------- FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)-------------------------------*/

void mapa_hacete_visible_para_entrenadores()
{
	extern t_mapa *mapa;
	ejecutar_hilo_socket(mapa->info_socket->puerto, mapa->info_socket->ip);
}

