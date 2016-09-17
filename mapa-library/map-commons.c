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
	mapa_mostrate_en_pantalla();
	mapa_hacete_visible_para_entrenadores();

}


/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* mapa_dame_coordenadas_de_pokenest(char* identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = mapa_buscame_pokenest(identificador_pokemon);
	return pokeNest_buscado->posicion;
}

t_pokeNest* mapa_buscame_pokenest(char *identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = dictionary_get(mapa->pokeNests, identificador_pokemon);
	return pokeNest_buscado;
}

char* mapa_dame_medalla()
{
	return obtener_ruta_especifica(mapa->ruta_pokedex, "Mapas",mapa->nombre);
}

int mapa_decime_si_hay_pokemones_en_pokenest(t_pokeNest *pokenest)
{
	return (pokenest->cantidad_pokemones_disponibles>0);
}

t_pokemon* mapa_dame_pokemon_de_pokenest(t_pokeNest *pokenest)
{
	t_pokemon *poke = queue_pop(pokenest->pokemones);
	pokenest->cantidad_pokemones_disponibles--;
	return poke;
}

void mapa_actualiza_pokemones_disponibles_de_pokenest(t_pokeNest *pokenest)
{
	pokenest->cantidad_pokemones_disponibles = queue_size(pokenest->pokemones);
	char id = *(pokenest->identificador);
	restarRecurso(mapa->items_para_mostrar_en_pantalla, id);
}

int mapa_decime_si_entrenador_esta_bloqueado(t_entrenador *entrenador)
{
	if(entrenador->estado == 0)
	{
		return 1;
	}
	else {return 0;}
}

int mapa_decime_si_entrenador_estaba_bloqueado(t_entrenador *entrenador)
{
	if(entrenador->estado_anterior == 0)
		{
			return 1;
		}
		else {return 0;}
}

int mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(t_entrenador *entrenador)
{
	return( !mapa_decime_si_entrenador_esta_bloqueado(entrenador) && mapa_decime_si_entrenador_estaba_bloqueado(entrenador));
}

void mapa_cambiale_estado_a_entrenador(t_entrenador *entrenador, int estado_entrante, int estado_saliente)
{
	entrenador->estado = estado_entrante;
	entrenador->estado_anterior = estado_saliente;
}

void mapa_devolve_pokemon_a_pokenest(t_pokemon *pokemon)
{
	t_pokeNest *pokenest = dictionary_get(mapa->pokeNests, pokemon->identificador_pokenest);
	queue_push(pokenest->pokemones, pokemon);
	pokenest->cantidad_pokemones_disponibles++;
}

int mapa_decime_si_planificador_es_rr()
{
	return(string_equals_ignore_case(mapa->info_algoritmo->algoritmo,"RR"));
}

int mapa_decime_si_entrenador_finalizo_su_objetivo(int socket_entrenador)
{
	char *key = string_itoa(socket_entrenador);
	return(dictionary_has_key(mapa->entrenadores->lista_entrenadores_finalizados, key));
	free(key);
}


/*--------------------------------------------------- FUNCIONES PARA GRAFICAR--------------------------------------------*/
 void mapa_mostrate_en_pantalla()
{
	mapa->items_para_mostrar_en_pantalla= list_create();

	int filas, columnas;
	nivel_gui_inicializar();
	nivel_gui_get_area_nivel(&filas, &columnas);
	mapa_agrega_pokenest_a_items_para_pantalla();
	nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);
}

void mapa_agrega_pokenest_a_items_para_pantalla()
{
	dictionary_iterator(mapa->pokeNests, closure);
}

void closure(char *identificador, void *data )
{
	t_pokeNest *pokenest=(t_pokeNest*) data;
	char id = *(pokenest->identificador);
	CrearCaja(mapa->items_para_mostrar_en_pantalla,id,pokenest->posicion->x,pokenest->posicion->y,pokenest->cantidad_pokemones_disponibles);
}

void mapa_mostra_nuevo_entrenador_en_pantalla(t_entrenador *entrenador)
{
	char id = *(entrenador->simbolo_identificador);
	CrearPersonaje(mapa->items_para_mostrar_en_pantalla, id, entrenador->posicion_actual->x,entrenador->posicion_actual->y);
}

void mapa_mostra_actualizacion_de_entrenador(t_entrenador *entrenador)
{
	char id = *(entrenador->simbolo_identificador);
	MoverPersonaje(mapa->items_para_mostrar_en_pantalla, id,entrenador->posicion_actual->x,entrenador->posicion_actual->y);
}

void mapa_borra_entrenador_de_pantalla(t_entrenador *entrenador)
{
	char id = *(entrenador->simbolo_identificador);
	BorrarItem(mapa->items_para_mostrar_en_pantalla, id);
}
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

