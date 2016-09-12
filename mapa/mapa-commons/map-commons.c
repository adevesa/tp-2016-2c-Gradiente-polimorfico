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


/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/


/*------------------------------ FUNCIONES PARA MANIPULACION DEL PLANIFICADOR--------------------------------------------*/
void planificador_create_segun_cual_seas()
{
	extern t_mapa *mapa;
	pthread_t thread;
	if(string_equals_ignore_case(mapa->info_algoritmo->algoritmo, "RR"))
	{
		pthread_create(&thread,NULL,planificador_rr_create,(void*) NULL);
	}
		pthread_create(&thread,NULL,planificador_srdf_create,(void*) NULL);
}


void planificador_libera_pokemons_de(t_entrenador *entrenador, t_list *lista_pokemones)
{
	list_add_all(lista_pokemones, entrenador->pokemones_capturados);
	list_destroy(entrenador->pokemones_capturados); //<-- debe usarse destroy&destroyElements
}

void planificador_elimina_entrenador_de_tus_listas(t_entrenador *entrenador, t_controllers *entrenadores)
{

}

void foreach(void *lista,void *cola,void(*funcion_de_lista)(void*, void*))
{
	t_list *lista_a_planificar = (t_list*)lista;
	t_queue *cola_listos = (t_queue*)cola;
	int tamanio = list_size(lista);
	int i;
	for(i=0; i<tamanio;i++)
	{
		funcion_de_lista(list_get(lista_a_planificar, i),cola_listos);
	}
}

void mapa_encola_nuevos_entrenadores(t_controllers *listas_y_colas)
{
	/*
	 * OJO! DEBE USARSE SEMAFORO PARA QUE NO OCURRA ERROR.
	 * UN NUEVO ENTRENADOR PORDRIA QUERER ACCEDER MIENTRAS MOVEMOS A LOS QUE YA ESTABAN
	 */

	if(!list_is_empty(listas_y_colas->lista_entrenadores_a_planificar))
	{
		foreach(listas_y_colas->lista_entrenadores_a_planificar, listas_y_colas->cola_entrenadores_listos,mapa_modela_nuevo_entrenador_y_encolalo);
		list_clean(listas_y_colas->lista_entrenadores_a_planificar);
	}
}

void mapa_modela_nuevo_entrenador_y_encolalo(void *entrenador,void*cola_listos)
{
	t_entrenador *entrenador_a_modelar = (t_entrenador *) entrenador;
	t_queue *cola = (t_queue*) cola_listos;
	t_entrenador *new_entrenador = entrenador_create(entrenador_a_modelar->id_proceso, entrenador_a_modelar->socket_etrenador);
	queue_push(cola,new_entrenador);
}




/*----------------------- FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)-------------------------------*/

void mapa_hacete_visible_para_entrenadores()
{
	extern t_mapa *mapa;
	ejecutar_hilo_socket(mapa->info_socket->puerto, mapa->info_socket->ip, mapa->entrenadores->lista_entrenadores_a_planificar);
}

