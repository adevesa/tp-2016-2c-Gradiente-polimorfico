/*
 * basic-structs.c
 *
 *  Created on: 12/9/2016
 *      Author: utnso
 */
#include "basic-structs.h"

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_entrenador* entrenador_create(int id_proceso, int socket_entrenador)
{
	t_entrenador *new_entrenador = malloc(sizeof(t_entrenador));
	new_entrenador->id_proceso = id_proceso;
	new_entrenador->socket_entrenador =socket_entrenador;
	new_entrenador->estado = 1;
	new_entrenador->objetivo_cumplido = 0;
	new_entrenador->tiene_objetivo = 0;
	new_entrenador->pokemones_capturados = list_create();
	new_entrenador->posicion_actual = posicion_create(0,0);
	new_entrenador->tiempo_consumido = 0;
	return new_entrenador;
}

t_pokeNest* pokenest_create(char* nombre,char *ruta)
{
	t_pokeNest *new_pokenest = malloc(sizeof(t_pokeNest));
	new_pokenest->ruta_en_pokeDex = ruta;
	new_pokenest->configuracion = config_create(obtener_ruta_especifica(ruta,"metadata", NULL));
	new_pokenest->tipo = obtener_info_pokenest_tipo(new_pokenest->configuracion);
	new_pokenest->posicion = obtener_info_pokenest_posicion(new_pokenest->configuracion);
	new_pokenest->identificador = obtener_info_pokenest_id(new_pokenest->configuracion);
	new_pokenest->pokemones = obtener_info_pokenest_pokemones(nombre, ruta, new_pokenest->identificador);
	new_pokenest->cantidad_pokemones_disponibles = queue_size(new_pokenest->pokemones);
	return new_pokenest;
}

t_posicion* posicion_create(int x, int y)
{
	t_posicion *new_posicion = malloc(sizeof(t_posicion));
	new_posicion->x = x;
	new_posicion->y = y;
	return new_posicion;
}

t_pokemon* pokemon_create(char *ruta)
{
	t_pokemon *new_pokemon = malloc(sizeof(t_pokemon));
	t_config *config = config_create(ruta);
	new_pokemon->nivel = config_get_int_value(config, "Nivel");
	new_pokemon->ruta_en_pokedex = ruta;
	return new_pokemon;
	config_destroy(config);
}

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
	new_controllers->lista_entrenadores_finalizados = dictionary_create();
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

t_config* configuracion_metadata_create(char *nombre, char *ruta)
{
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(ruta, "Mapas", nombre);
	ruta_final = obtener_ruta_especifica(ruta_final, "metadata", NULL);
	t_config *config_new = config_create(ruta_final);
	return config_new;;
}

/*-----------------------------------------------------DESTROYERS----------------------------------------------------*/

/*---------------------------- FUNCIONES PARA OBTENER DATOS BASICOS DE UN MAPA--------------------------------------*/

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

t_dictionary* obtener_info_mapa_pokenest(char *nombreMapa, char *rutaPokedex)
{

	t_dictionary *new_diccionary_pokenest = dictionary_create();

	char *ruta_final = obtener_ruta_especifica(rutaPokedex, "Mapas", nombreMapa);
	ruta_final = obtener_ruta_especifica(ruta_final, "PokeNest", NULL);
	t_list *lista_directorios = nombre_de_archivos_del_directorio(ruta_final);
	foreach_pokenest_modelate(lista_directorios, new_diccionary_pokenest, ruta_final);


	return new_diccionary_pokenest;
}

void foreach_pokenest_modelate(void *lista_origen,void *lista_destino, void *ruta)
{
	t_list *lista_pokemones_a_modelar = (t_list*)lista_origen;
	t_dictionary *lista_pokemons_a_devolver = (t_dictionary*)lista_destino;

	int tamanio = list_size(lista_pokemones_a_modelar);
	int i;
	for(i=0; i<tamanio;i++)
	{
		char *ruta_final = (char*) ruta;
		char *elemento =list_get(lista_pokemones_a_modelar, i);
		ruta_final = obtener_ruta_especifica(ruta_final, elemento, NULL);
		t_pokeNest *pokenest = pokenest_create(elemento,ruta_final);
		dictionary_put(lista_pokemons_a_devolver, pokenest->identificador, pokenest);
	}

}


/*---------------------------- FUNCIONES PARA OBTENER DATOS BASICOS DE UN POKENEST--------------------------------------*/
char* obtener_info_pokenest_tipo(t_config *configuracion)
{
	char *pokenest_tipo = config_get_string_value(configuracion, "Tipo");
	return pokenest_tipo;
}

t_posicion* obtener_info_pokenest_posicion(t_config *configuracion)
{
	char *string_de_coordenada = config_get_string_value(configuracion, "Posicion");
	char **array_de_coordenada = string_split(string_de_coordenada, ";");
	int x =  atoi(array_de_coordenada[0]);
	int y =  atoi(array_de_coordenada[1]);
	t_posicion *new_posicion = posicion_create(x,y);
	return new_posicion;
}

char* obtener_info_pokenest_id(t_config *configuracion)
{
	char *id = config_get_string_value(configuracion, "Identificador");
	return id;
}

t_queue* obtener_info_pokenest_pokemones(char *nombrePokenest, char *ruta_final, char *identificador)
{
	t_queue *new_cola_pokemones = queue_create();
	t_list *lista_directorios = nombre_de_archivos_del_directorio(ruta_final);
	foreach_pokenest(lista_directorios, new_cola_pokemones, ruta_final, identificador);
	list_clean(lista_directorios);
	return new_cola_pokemones;
	free(ruta_final);
}

void foreach_pokenest(void *lista_origen,void *lista_destino, void *ruta, void *identificador)
{
	t_list *lista_pokemones_a_modelar = (t_list*)lista_origen;
	t_queue *cola_pokemons_a_devolver = (t_queue*)lista_destino;
	char *identificador_pokenest = (char*) identificador;

	int tamanio = list_size(lista_pokemones_a_modelar);
	int i;
	for(i=0; i<tamanio;i++)
	{
		char *ruta_final =string_new();
		string_append(&ruta_final, (char*) ruta);
		char *elemento =list_get(lista_pokemones_a_modelar, i);
		ruta_final = obtener_ruta_especifica(ruta_final, elemento, NULL);
		t_pokemon *pokemon_new = pokemon_create(ruta_final);
		pokemon_new->identificador_pokenest = identificador_pokenest;
		queue_push(cola_pokemons_a_devolver, pokemon_new);
	}

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
	}
	else return ruta;
}

t_list* nombre_de_archivos_del_directorio(char *ruta)
{

	/* Variables */
		DIR *dirp;
		struct dirent *direntp;

	/* Abrimos el directorio */
		dirp = opendir(ruta);
		if (dirp == NULL){
			printf("Error: No se puede abrir el directorio\n");
			exit(1);
		}
		t_list *lista = list_create();
		while ((direntp = readdir(dirp)) != NULL) {
			 if(!string_equals_ignore_case(direntp->d_name, ".")){
				  if(!string_equals_ignore_case(direntp->d_name, "..")){
					  if(!string_equals_ignore_case(direntp->d_name, "metadata")){
						 list_add(lista, direntp->d_name);
					  	 }
				  }
			 }
		}
		 	 return lista;
		 	 /* Cerramos el directorio */
		 	 closedir(dirp);
	}
