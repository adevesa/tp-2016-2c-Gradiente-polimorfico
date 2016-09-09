/*
 * pokenest-commons.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */
#include "pokenest-commons.h"

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/string.h>

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

//OJO ACA"//
t_posicion* obtener_info_pokenest_posicion(t_config *configuracion)
{
	t_posicion *new_posicion = posicion_create(config_get_int_value(configuracion,"Posicion"),config_get_int_value(configuracion,"Posicion")); //OJO ACA!
	return new_posicion;
}

char* obtener_info_pokenest_id(t_config *configuracion)
{
	char *id = config_get_string_value(configuracion, "Identificador");
	return id;
}

//OJO ESTA FUNCION!!
t_list* obtener_info_pokenest_pokemones(char *nombrePokenest, char *ruta)
{
	t_list *new_list_pokemones = list_create();
	//ACA VA TODA LA LOGICA DE BUSCAR LOS POKEMONES EN EL ARCHIVO Y AGREGARLOS A LA LISTA//

	//A MODO DE EJEMPLO//
	char *nombre_dato = string_new();
	strcpy(&nombre_dato, nombrePokenest);
	string_append(&nombre_dato, "001.dat");
	char *ruta_new = obtener_ruta_determinada(ruta, nombrePokenest, nombre_dato);
	t_pokemon *pokemon_new = pokemon_create(ruta_new);
	list_add(new_list_pokemones, pokemon_new);
	//FIN EJEMPLO

	//obtener_ruta_determinada(ruta, nombrePokenest,ARCHIVOLEIDO)
	return new_list_pokemones;
}

char* obtener_ruta_determinada(char *ruta_inicial, char *directorio_o_nombre_archivo, char *sub_directorio_o_nombre_archivo)
{
	char* ruta = string_new();
	strcpy(ruta, ruta_inicial);
	string_append(&ruta, "/");
	string_append(&ruta, directorio_o_nombre_archivo);
	if(sub_directorio_o_nombre_archivo != NULL)
	{
		string_append(&ruta,sub_directorio_o_nombre_archivo);
		return ruta;
		free(ruta);
	}
	else return ruta;
	free(ruta);
}



