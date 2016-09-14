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
	//t_pokeNest *pokenest = list_find(mapa->pokeNests, pokenest_tipoo);
	return pokeNest_buscado->posicion;
}


char* buscar_medalla_del_mapa()
{
	return obtener_ruta_especifica(mapa->ruta_pokedex, "Mapas",mapa->nombre);
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

