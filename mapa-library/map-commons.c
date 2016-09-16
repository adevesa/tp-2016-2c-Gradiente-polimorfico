/*
 * map-commons.c
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */
#include "map-commons.h"

/*------------------------------------------VARIABLES GLOBALES----------------------------------------------------*/
t_mapa *mapa;


/*------------------------------------------EXECUTE----------------------------------------------------------------*/
void ejecutar_mapa(char *nombre, char *rutaPokedex)
{
	mapa = mapa_create(nombre, rutaPokedex);
	planificador_create_segun_cual_seas();
	//nivel_gui_inicializar();
	mapa_hacete_visible_para_entrenadores();

}


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* buscar_coordenadas(char* identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = dictionary_get(mapa->pokeNests, identificador_pokemon);
	return pokeNest_buscado->posicion;
}

//REVISAR//
t_pokeNest* buscar_pokenest(char *identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = dictionary_get(mapa->pokeNests, identificador_pokemon);
	return pokeNest_buscado;
}

char* buscar_medalla_del_mapa()
{
	return obtener_ruta_especifica(mapa->ruta_pokedex, "Mapas",mapa->nombre);
}

int hay_pokemones_en_pokenest(t_pokeNest *pokenest)
{
	return (pokenest->cantidad_pokemones_disponibles>0);
}


t_pokemon* pokenest_dame_pokemon(t_pokeNest *pokenest)
{
	t_pokemon *poke = queue_pop(pokenest->pokemones);
	return poke;
}

void pokenest_actualiza_tu_cantidad_pokemones_disponibles(t_pokeNest *pokenest)
{
	pokenest->cantidad_pokemones_disponibles = queue_size(pokenest->pokemones);
}

/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/


/*------------------------------ FUNCIONES PARA MANIPULACION DEL PLANIFICADOR--------------------------------------------*/
void planificador_create_segun_cual_seas()
{
	pthread_t thread;
	if(string_equals_ignore_case(mapa->info_algoritmo->algoritmo, "RR"))
	{
		pthread_create(&thread,NULL,ejecutar_planificador_rr, NULL);
	}
	else{	pthread_create(&thread,NULL,ejecutar_planificador_srdf,NULL); }
}


/*----------------------- FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)-------------------------------*/

void mapa_hacete_visible_para_entrenadores()
{
	pthread_t hilo_servidor;
	pthread_create(&hilo_servidor,NULL,ejecutar_servidor,(void*)mapa->info_socket);
}

