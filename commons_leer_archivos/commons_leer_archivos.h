/*
 * commons_leer_archivos.h
 *
 *  Created on: 8/9/2016
 *      Author: utnso
 */

#ifndef COMMONS_LEER_ARCHIVOS_H_
#define COMMONS_LEER_ARCHIVOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <sys/types.h>
#include <dirent.h>


typedef struct coordenada{
	int x;
	int y;
}t_coordenada;

//--FUNCIONES TRONCALES--//

//dada una ruta a una carpeta, muestra el nombre de todos los archivos (.txt, .jpg, otras capertas, etc)
t_list* nombre_de_archivos_del_directorio(char *ruta);

//dada una ruta a un .txt, muestra su contenido en pantalla
void mostrar_archivo(char* ruta);

//dado un string, le quita los corchetes
char* quitar_corchetes(char *string);

//dado una palabra y una ruta a un .txt, devuelve lo que esta despues de palabra=
char* buscar_datos_asociados(char *palabra, char *ruta_de_archivo );

// dado un array ["casa", "perro", NULL], lo transforma en una lista
t_list* de_array_a_lista(char** array);

//--DIRECCIONES A METADATAS--//

//dado una direccion a la pokedex y nombres de las carpetas muestran las ruta a los metadata

char* direccion_a_entrenador(char *direccion_a_pokedex, char *nombre);

char* direccion_a_mapa(char *direccion_a_pokedex ,char *nombre);

char* direccion_a_pokenest(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest);

char* direccion_a_pokemon(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest, char *nombre_pokemon);

//--FUNCIONES PARA MOSTRAR METADATAS--//

// muestran en pantalla los metadata

void mostrar_metadata_entrenador(char *direccion_a_pokedex ,char *nombre);

void mostrar_metadata_mapa(char *direccion_a_pokedex, char *nombre);

void mostrar_metadata_pokenest(char *direccion_a_pokedex ,char *nombre_mapa, char *nombre_pokenest);

void mostrar_datos_de_pokemon(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest, char *nombre_pokemon);

//-----------RETORNAR VALORES DEL METADATA ENTRENADOR-----------------//

//dada el nombre de la carpeta de untrenador y la direccion a la pokedex, devuelve datos del metadata del entrenador

int entrenador_vidas(char *nombre_entrenador, char *direccion_a_pokedex );

char* entrenador_nombre(char *nombre_entrenador, char *direccion_a_pokedex );

char* entrenador_simbolo(char *nombre_entrenador, char *direccion_a_pokedex );

int entrenador_reintentos(char *nombre_entrenador, char *direccion_a_pokedex );

t_list* entrenador_hoja_de_viaje(char *nombre_entrenador, char *direccion_a_pokedex );

t_list* entrenador_objetivos_por_mapa(char *nombre_entrenador, char *mapa, char *direccion_a_pokedex );

//-----------RETORNAR VALORES DEL METADATA MAPA-----------------//

//lo mismo que antes pero para mapas

int mapa_tiempo_chequeo_deadlock(char *nombre_mapa, char *direccion_a_pokedex );

int mapa_batalla(char *nombre_mapa, char *direccion_a_pokedex );

char* mapa_algoritmo(char *nombre_mapa, char *direccion_a_pokedex );

int mapa_quiantum(char *nombre_mapa, char *direccion_a_pokedex );

int mapa_retardo(char *nombre_mapa, char *direccion_a_pokedex );

char* mapa_ip(char *nombre_mapa, char *direccion_a_pokedex );

int mapa_puerto(char *nombre_mapa, char *direccion_a_pokedex );

//-----------RETORNAR VALORES DEL METADATA POKENESt-----------------//

// lo mismo que antes pero para pokenes un determinado mapa

char* pokenest_tipo(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex );

char* pokenest_identificador(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex );

t_coordenada* pokenest_coordenada(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex );

//-----------RETORNAR VALORES DE POKEMON-----------------//

//lo mismo que antes pero para pokemon en una determinada pokenest

int pokemon_nivel(char* nombre_pokemon, char *nombre_pokenest, char *nombre_mapa, char *direccion_a_pokedex );

//---------LEER ARCHIVOS DE DIRECTORIO------------------------//


//devuelve el nombre de todas las carpetas de los pokenest de un determinado mapa
t_list* mapa_pokenest(char *nombre_mapa, char *direccion_a_pokedex );

//devuelve el nombre de todas las imagenes de la carpeta medallas de un entrenador
t_list* entrenador_medallas(char* nombre_entrenador, char *direccion_a_pokedex);

//devuelve todos los mapas en la pokedex
t_list* lista_de_mapas(char *direccion_a_pokedex);

//devuelve todos los entrenador en la pokedex
t_list* lista_de_entrenadores(char *direccion_a_pokedex);

//devuelve todos los pokemon en dir de bill de un entrenador
t_list* pokemon_en_dir_de_bill(char* nombre_entrenador, char *direccion_a_pokedex);


//REVISAR LA IMPLEMENTACION DE list_remove_by_condition, FUNCIONA MAL!
//devuelve todos los pokemon en una pokenest
t_list* pokemon_en_pokenest(char *nombre_pokenest, char *nombre_mapa, char *direccion_a_pokedex);

#endif /* COMMONS_LEER_ARCHIVOS_H_ */
