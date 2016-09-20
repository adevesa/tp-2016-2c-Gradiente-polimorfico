/*
 * commons_leer_archivos.c
 *
 *  Created on: 8/9/2016
 *      Author: utnso
 */

#include "commons_leer_archivos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <sys/types.h>
#include <dirent.h>


//--FUNCIONES TRONCALES--//


t_list* nombre_de_archivos_del_directorio(char *ruta){

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


void mostrar_archivo(char* ruta){
	FILE *archivo;
	archivo = fopen(ruta, "r");

			if ( archivo==NULL ){
					printf("Error al abrir el fichero\n");
			}

				else
				{
					while( !feof(archivo) ){
						printf("%c",getc(archivo));
					}
				}
			fclose(archivo);
	}


char* quitar_corchetes(char *string){
	int tamanio = string_length(string);
	char* aux=malloc(tamanio-2);
	char caracter;

	int i,j = 0;
	for (i = 0;i<tamanio ;i++) {
	    caracter = string[i];
	    if (caracter != '[') {
	    	if (caracter != ']'){
	          aux[j] = caracter;
	          j++;
	    	   }
	       }

	   }
	return aux;
	free(aux);
	}


char* buscar_datos_asociados(char *palabra, char *ruta_de_archivo ){
	char linea[100];
	FILE *archivo = fopen(ruta_de_archivo,"r");
	while(!feof(archivo)){
		fgets(linea,100,archivo);
		if (string_starts_with(linea, palabra)){
			char** array = string_split(linea, "=");
			return array[1];
		}
	}
	fclose(archivo);
}


t_list* de_array_a_lista(char** array){

	t_list *lista = list_create();
	int i = 0;
	while (array[i] != NULL){
		list_add(lista, array[i]);
		i++;
	}
	return lista;
}


//--DIRECCIONES A METADATAS--//

char* direccion_a_entrenador(char *direccion_a_pokedex, char *nombre){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/entrenadores/");
				string_append(&ruta, nombre);
				string_append(&ruta, "/metadata");
	return ruta;
	free(ruta);
}

char* direccion_a_mapa(char *direccion_a_pokedex ,char *nombre){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/Mapas/");
				string_append(&ruta, nombre);
				string_append(&ruta, "/metadata");
	return ruta;
	free(ruta);
}

char* direccion_a_pokenest(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/Mapas/");
				string_append(&ruta, nombre_mapa);
				string_append(&ruta, "/PokeNests/");
				string_append(&ruta, nombre_pokenest);
				string_append(&ruta, "/metadata");
	return ruta;
	free(ruta);
}

char* direccion_a_pokemon(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest, char *nombre_pokemon){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/Mapas/");
				string_append(&ruta, nombre_mapa);
				string_append(&ruta, "/PokeNests/");
				string_append(&ruta, nombre_pokenest);
				string_append(&ruta, "/");
				string_append(&ruta, nombre_pokemon);
	return ruta;
	free(ruta);
}

//--FUNCIONES PARA MOSTRAR METADATAS--//

void mostrar_metadata_entrenador(char *direccion_a_pokedex ,char *nombre){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre);
	mostrar_archivo(ruta);
}

void mostrar_metadata_mapa(char *direccion_a_pokedex, char *nombre){
	char *ruta = direccion_a_mapa(direccion_a_pokedex, nombre);
	mostrar_archivo(ruta);
}

void mostrar_metadata_pokenest(char *direccion_a_pokedex ,char *nombre_mapa, char *nombre_pokenest){
	char *ruta = direccion_a_pokenest(direccion_a_pokedex, nombre_mapa, nombre_pokenest);
	mostrar_archivo(ruta);
}

void mostrar_datos_de_pokemon(char *direccion_a_pokedex, char *nombre_mapa, char *nombre_pokenest, char *nombre_pokemon){
	char *ruta= direccion_a_pokemon(direccion_a_pokedex, nombre_mapa, nombre_pokenest, nombre_pokemon);
			mostrar_archivo(ruta);
}

//-----------RETORNAR VALORES DEL METADATA ENTRENADOR-----------------//



int entrenador_vidas(char *nombre_entrenador, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	char *vidas = buscar_datos_asociados("vidas", ruta);
	return atoi(vidas);
}

char* entrenador_nombre(char *nombre_entrenador, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	return buscar_datos_asociados("nombre", ruta);
}

char* entrenador_simbolo(char *nombre_entrenador, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	return buscar_datos_asociados("simbolo", ruta);
}

int entrenador_reintentos(char *nombre_entrenador, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	char *reintentos = buscar_datos_asociados("reintentos", ruta);
	return atoi(reintentos);
}

t_list* entrenador_hoja_de_viaje(char *nombre_entrenador, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	char *hoja_de_viaje = buscar_datos_asociados("hojaDeViaje", ruta);
	char *hoja_de_viaje_sin_corchetes = quitar_corchetes(hoja_de_viaje);
	char **array_de_mapas = string_split(hoja_de_viaje_sin_corchetes, ",");
	return de_array_a_lista(array_de_mapas);
}

t_list* entrenador_objetivos_por_mapa(char *nombre_entrenador, char *mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_entrenador(direccion_a_pokedex, nombre_entrenador);
	char *palabraclave = string_new();
		string_append(&palabraclave, "obj[");
		string_append(&palabraclave, mapa);
	char *objetivos = buscar_datos_asociados(palabraclave, ruta);
	char *objetivos_sin_corchetes = quitar_corchetes(objetivos);
	char **array_de_objetivos = string_split(objetivos_sin_corchetes, ",");
	return de_array_a_lista(array_de_objetivos);
}

//-----------RETORNAR VALORES DEL METADATA MAPA-----------------//

int mapa_tiempo_chequeo_deadlock(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	char *deadlock = buscar_datos_asociados("TiempoChequeoDeadlock", ruta);
	return atoi(deadlock);
}

int mapa_batalla(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	char *batalla = buscar_datos_asociados("Batalla", ruta);
	return atoi(batalla);
}

char* mapa_algoritmo(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	return buscar_datos_asociados("algoritmo", ruta);

}

int mapa_quiantum(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	char *quantum = buscar_datos_asociados("quantum", ruta);
	return atoi(quantum);
}

int mapa_retardo(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	char *retardo = buscar_datos_asociados("retardo", ruta);
	return atoi(retardo);
}

char* mapa_ip(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	return buscar_datos_asociados("IP", ruta);
}

int mapa_puerto(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_mapa(direccion_a_pokedex ,nombre_mapa);
	char *puerto = buscar_datos_asociados("Puerto", ruta);
	return atoi(puerto);
}

//-----------RETORNAR VALORES DEL METADATA POKENESt-----------------//

char* pokenest_tipo(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta= direccion_a_pokenest(nombre_pokenes, nombre_mapa, direccion_a_pokedex );
	return buscar_datos_asociados("Tipo", ruta);
}

char* pokenest_identificador(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta= direccion_a_pokenest(nombre_pokenes, nombre_mapa, direccion_a_pokedex );
	return buscar_datos_asociados("Identificador", ruta);
}

t_coordenada* pokenest_coordenada(char *nombre_pokenes, char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta= direccion_a_pokenest(nombre_pokenes, nombre_mapa, direccion_a_pokedex );
	char *string_de_coordenada = buscar_datos_asociados("coordenada", ruta);
	char **array_de_coordenada = string_split(string_de_coordenada, ";");
	t_coordenada* coordenada = malloc(sizeof(t_coordenada));
	coordenada->x =  atoi(array_de_coordenada[0]);
	coordenada->y =  atoi(array_de_coordenada[1]);
	return coordenada;
	free(coordenada);
}


t_list* pokemon_en_pokenest(char *nombre_pokenest, char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/mapas/");
				string_append(&ruta, nombre_mapa);
				string_append(&ruta, "/PokeNests/");
				string_append(&ruta, nombre_pokenest);
				t_list *lista = nombre_de_archivos_del_directorio(ruta);
				return lista;

	free(ruta);
	}

//-----------RETORNAR VALORES DE POKEMON-----------------//

int pokemon_nivel(char* nombre_pokemon, char *nombre_pokenest, char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = direccion_a_pokemon(direccion_a_pokedex, nombre_mapa, nombre_pokenest, nombre_pokemon);
	char *nivel = buscar_datos_asociados("Nivel", ruta);
		return atoi(nivel);
}

//---------LEER ARCHIVOS DE DIRECTORIO------------------------//


t_list* mapa_pokenest(char *nombre_mapa, char *direccion_a_pokedex ){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/Mapas/");
				string_append(&ruta, nombre_mapa);
				string_append(&ruta, "/PokeNests");
	return nombre_de_archivos_del_directorio(ruta);
	free(ruta);
}

t_list* entrenador_medallas(char* nombre_entrenador, char *direccion_a_pokedex){
	char *ruta = string_new();
			string_append(&ruta, direccion_a_pokedex);
			string_append(&ruta, "/entrenadores/");
			string_append(&ruta, nombre_entrenador);
			string_append(&ruta, "/medallas");
	return nombre_de_archivos_del_directorio(ruta);
	free(ruta);
}

t_list* lista_de_mapas(char *direccion_a_pokedex){
	char *ruta = string_new();
			string_append(&ruta, direccion_a_pokedex);
			string_append(&ruta, "/Mapas");
	return nombre_de_archivos_del_directorio(ruta);
	free(ruta);
}

t_list* lista_de_entrenadores(char *direccion_a_pokedex){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/entrenadores");
		return nombre_de_archivos_del_directorio(ruta);
		free(ruta);
}

t_list* pokemon_en_dir_de_bill(char* nombre_entrenador, char *direccion_a_pokedex){
	char *ruta = string_new();
				string_append(&ruta, direccion_a_pokedex);
				string_append(&ruta, "/entrenadores/");
				string_append(&ruta, nombre_entrenador);
				string_append(&ruta, "/Dir de Bill");
		return nombre_de_archivos_del_directorio(ruta);
		free(ruta);
}


