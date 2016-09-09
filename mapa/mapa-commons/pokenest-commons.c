/*
 * pokenest-commons.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */
#include "pokenest-commons.h"



/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_pokeNest* pokenest_create(char* nombre,char *ruta)
{
	t_pokeNest *new_pokenest = malloc(sizeof(t_pokeNest));
	new_pokenest->configuracion = config_create(obtener_ruta_determinada(ruta, nombre, "metadata"));
	new_pokenest->tipo = obtener_info_pokenest_tipo(new_pokenest->configuracion);
	new_pokenest->posicion = obtener_info_pokenest_posicion(new_pokenest->configuracion);
	new_pokenest->identificador = obtener_info_pokenest_id(new_pokenest->configuracion);
	new_pokenest->pokemones = obtener_info_pokenest_pokemones(nombre, ruta);
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
	return new_pokemon;
	config_destroy(config);
}


/*--------------------------------------------SECUNDARIAS----------------------------------------------------------*/
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


t_list* obtener_info_pokenest_pokemones(char *nombrePokenest, char *ruta)
{
	t_list *new_list_pokemones = list_create();
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_determinada(ruta, nombrePokenest, nombrePokenest);
	t_list *lista_directorios = nombre_de_archivos_del_directorio(ruta_final);
	list_remove_by_condition(lista_directorios,es_metadata);
	foreach_pokenest(lista_directorios, new_list_pokemones, ruta_final);

	return new_list_pokemones;
}

void foreach_pokenest(void *lista_origen,void *lista_destino, void *ruta)
{
	t_list *lista_pokemones_a_modelar = (t_list*)lista_origen;
	t_list *lista_pokemons_a_devolver = (t_list*)lista_destino;

	int tamanio = list_size(lista_pokemones_a_modelar);
	int i;
	for(i=0; i<tamanio;i++)
	{
		char *ruta_final = (char*) ruta;
		char *elemento =list_get(lista_pokemones_a_modelar, i);
		ruta_final = obtener_ruta_determinada(ruta_final, elemento, NULL);
		t_pokemon *pokemon_new = pokemon_create(ruta_final);
		list_add(lista_pokemons_a_devolver, pokemon_new);
	}

}

bool es_metadata(void *argumento)
{
	char *palabra = (char*) argumento;
	return string_equals_ignore_case(palabra, "metadata" );
}


char* obtener_ruta_determinada(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo)
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



