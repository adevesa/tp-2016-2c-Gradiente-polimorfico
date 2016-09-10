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

	/*
	 * ACA SE EJECTURARIA EL PLANIFICADOR CON ALGUNA FUNCION ASI:
	 * t_planificador *planificador = planificador_create(mapa);
	 */

	/*
	 * nivel_gui_inicializar();
	 */

	mapa_hacete_visible_para_entrenadores(mapa);


}

/*--------------------------------------------CREATES---------------------------------------------------------------*/

t_mapa* mapa_create(char *nombre, char *rutaPokedex)
{
	t_mapa *new_mapa = malloc(sizeof(t_mapa));
	new_mapa->nombre = nombre;
	new_mapa->ruta_pokedex = rutaPokedex;
	new_mapa->configuracion = configuracion_metadata_create(nombre, rutaPokedex);
	new_mapa->entrenadores = controllers_create();
	new_mapa->info_socket = obtener_info_mapa_socket(new_mapa->configuracion);
	new_mapa->info_algoritmo = obtener_info_mapa_algoritmo(new_mapa->configuracion);
	new_mapa->batalla = obtener_info_mapa_batalla(new_mapa->configuracion);
	new_mapa->tiempo_chequeo_deadlock = obtener_info_mapa_tiempo_deadlock(new_mapa->configuracion);
	new_mapa->pokeNests = obtener_info_mapa_pokenest(nombre,rutaPokedex);
	return new_mapa;

}

t_controllers* controllers_create()
{
	t_controllers *new_controllers = malloc(sizeof(t_controllers));
	new_controllers->cola_entrenadores_listos = queue_create();
	new_controllers->cola_entrenadores_bloqueados = queue_create();
	new_controllers->lista_entrenadores_finalizados = list_create();
	new_controllers->lista_entrenadores_a_planificar = list_create();
	return new_controllers;
}

t_info_socket* info_socket_create(int puerto, char *ip)
{
	t_info_socket *new_info_socket = malloc(sizeof(t_info_socket));
	new_info_socket->ip = ip;
	new_info_socket->puerto=puerto;
	return new_info_socket;
}

t_info_algoritmo* info_algoritmo_create(char *algoritmo, int quamtum, int retardo)
{
	t_info_algoritmo *new_info_algoritmo = malloc(sizeof(t_info_algoritmo));
	new_info_algoritmo->algoritmo = algoritmo;
	new_info_algoritmo->quamtum = quamtum;
	new_info_algoritmo->retardo = retardo;
	return new_info_algoritmo;
}

/*

//MUDADO A POKENEST-COMMONS DESDE ACA
t_pokeNest* pokenest_create(char *nombreDelMapa, char *rutaPokedex)
{
	t_pokeNest *new_pokenest = malloc(sizeof(t_pokeNest));
	new_pokenest->tipo = obtener_info_pokenest_tipo(nombreDelMapa,rutaPokedex);
	new_pokenest->posicion = obtener_info_pokenest_posicion(nombreDelMapa,rutaPokedex);
	new_pokenest->identificador = obtener_info_pokenest_id(nombreDelMapa,rutaPokedex);
	new_pokenest->pokemones = obtener_info_pokenest_pokemones(nombreDelMapa,rutaPokedex);
	return new_pokenest;
}

t_posicion* posicion_create(int x, int y)
{
	t_posicion *new_posicion = malloc(sizeof(t_posicion));
	new_posicion->x = x;
	new_posicion->y = y;
	return new_posicion;
}

t_pokemon* pokemon_create(int nivel)
{
	t_pokemon *new_pokemon = malloc(sizeof(t_pokemon));
	new_pokemon->nivel = nivel;
	return new_pokemon;
}
//HASTA ACA
*/

t_entrenador* entrenador_create(int id_proceso)
{
	t_entrenador *new_entrenador = malloc(sizeof(t_entrenador));
	new_entrenador->id_proceso = id_proceso;
	new_entrenador->estado = 1;
	new_entrenador->objetivo_cumplido = 0;
	new_entrenador->pokemones_capturados = list_create();
	new_entrenador->posicion_actual = posicion_create(0,0);
	new_entrenador->tiempo_consumido = 0;
	return new_entrenador;
}

t_config* configuracion_metadata_create(char *nombre, char *ruta)
{
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(ruta, "Mapas", nombre);
	ruta_final = obtener_ruta_especifica(ruta_final, "metadata", NULL);
	t_config *config_new = config_create(ruta_final);
	return config_new;;
}
/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/

t_posicion* mapa_dame_ubicacion_pokenest(t_list *pokenest,char *nombrePokemon)
{
	//ACA VA LA LOGICA DE BUSCAR DENTRO DE CADA POKENEST CUAL CONTIENE AL POKEMON SOLICITADO//
	t_posicion *posicion = posicion_create(0,0);
	return posicion;
}


t_pokemon* mapa_atrapa_pokemon(char *nombrePokemon) //DEBO SABER QUIEN QUIERE EL POKEMON
{
	//ACA VA TODA LA LOGICA DE ATRAPAR UN POKEMON//
	t_pokemon *pokemon_buscado = pokemon_create(0); //<----- ES SOLO UN EJEMPLO
	return pokemon_buscado;
}

void mapa_registra_movimiento_de_entrenador(t_mapa *mapa,int id_entrenador)
{

}


/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/

/*
 * FUNCIONES PARA OBTENER DATOS BASICOS DE UN MAPA
 */

t_info_socket* obtener_info_mapa_socket(t_config *configuracion)
{

	t_info_socket *info_socket = info_socket_create(config_get_int_value(configuracion, "Puerto"), config_get_string_value(configuracion, "IP"));
	return info_socket;
}

t_info_algoritmo* obtener_info_mapa_algoritmo(t_config *configuracion)
{
	t_info_algoritmo *info_algoritmo = info_algoritmo_create(config_get_string_value(configuracion, "algoritmo"), config_get_int_value(configuracion, "quantum"), config_get_int_value(configuracion, "retardo"));
	return info_algoritmo;
}

int obtener_info_mapa_batalla(t_config *configuracion)
{
	int batalla = config_get_int_value(configuracion, "Batalla");
	return batalla;
}

int obtener_info_mapa_tiempo_deadlock(t_config *configuracion)
{
	int tiempo = config_get_int_value(configuracion,"TiempoChequeoDeadlock");
	return tiempo;
}

t_list* obtener_info_mapa_pokenest(char *nombreMapa, char *rutaPokedex)
{
	t_list *new_list_pokenest = list_create();
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(rutaPokedex, "Mapas", nombreMapa);
	ruta_final = obtener_ruta_especifica(ruta_final, "PokeNest", NULL);


	//ACA VA LA LOGICA DE RECORRER CADA POKENEST Y CREAR LOS POKENEST//

	//A  MODO EJEMPLO//
	t_pokeNest *new_pokenest =pokenest_create("pikachu", ruta_final);
	list_add(new_list_pokenest, new_pokenest);
	//FIN EJEMPLO//

	return new_list_pokenest;
}


char* obtener_ruta_especifica(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo)
{
	char* ruta = string_new();
	string_append(&ruta,ruta_inicial);
	string_append(&ruta, "/");
	string_append(&ruta, directorio_o_nombre_archivo);
	if(sub_directorio_o_nombre_archivo != NULL)
	{	string_append(&ruta, "/");
		string_append(&ruta,sub_directorio_o_nombre_archivo);
		return ruta;
		free(ruta);
	}
	else return ruta;
	free(ruta);
}



/*
 * FUNCIONES PARA MANIPULACION DEL PLANIFICADOR
 */
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

void planificador_crea_nuevo_entrenador_en_mapa_si_es_necesario(t_controllers *listas_y_colas)
{
	/*
	 * OJO! DEBE USARSE SEMAFORO PARA QUE NO OCURRA ERROR.
	 * UN NUEVO ENTRENADOR PORDRIA QUERER ACCEDER MIENTRAS MOVEMOS A LOS QUE YA ESTABAN
	 */

	if(!list_is_empty(listas_y_colas->lista_entrenadores_a_planificar))
	{
		foreach(listas_y_colas->lista_entrenadores_a_planificar, listas_y_colas->cola_entrenadores_listos,planificador_modela_nuevo_entrenador_y_encolalo);
		list_clean(listas_y_colas->lista_entrenadores_a_planificar);
	}
}

void planificador_modela_nuevo_entrenador_y_encolalo(int id_proceso,t_queue *cola_listos)
{
	t_entrenador *new_entrenador = entrenador_create(id_proceso);
	queue_push(cola_listos,new_entrenador);
}






/*
 * FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)
 */
void mapa_hacete_visible_para_entrenadores(t_mapa *mapa)
{
	ejecutar_hilo_socket(mapa->info_socket->puerto, mapa->info_socket->ip, mapa->entrenadores->lista_entrenadores_a_planificar);
}

