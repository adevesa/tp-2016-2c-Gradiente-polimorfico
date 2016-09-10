#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <stdbool.h>
#include <commons/config.h>
#include <commons/txt.h>
#include <entrenador_commons.h>

void ejecutar_entrenador(char *nombre_entrenador, char *rutaPokedex)
{
	//t_entrenador *entrenador_new  =entrenador_create(nombre_entrenador, rutaPokedex);
	//entrenador_busca_primer_objetivo(entrenador_new); //<-- ver abajo
}

/*
void entrenador_busca_primer_objetivo(t_entrenador *entrenador)
{
	//TODA LA LOGICA DE BUSCAR UN PRIMER MAPA, UNA VES QUE LO ENCUENTRO
	char *mapa =  buscar_objetivo(entrenador);
	int server_obtenido=conectarse_a_mapa(mapa);
	entrenador->mapa_actual->server = server_obtenido;
}
*/

/*
 * t_list* obtener_mapas(char *nombreEntrenador, char *ruta,t_list *mapas_entrenador )
 * {
 * 	//logica de meterse en el archivo y leer todos los strings mapas
 * 	foreach(string nombreMapa in mapas)
 * 	{
 * 		list_add(mapas_entrenador, modelar_mapa(nombreMapa));
 * 	}
 * }
 *
 * t_mapa* modelar_mapa(char *nombreMapa)
 * {
 * 		t_mapa *mapa_new = malloc.....
 * 		mapa_new->objetivos = ....
 * }
 */

t_config* configuracion_metadata_create(char *nombre, char *ruta)
{
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(ruta, "Entreandores", nombre);
	ruta_final = obtener_ruta_especifica(ruta_final, "metadata", NULL);
	t_config *config_new = config_create(ruta_final);
	return config_new;;
}

t_entrenador* entrenador_create(char* nombre, char* ruta)
  {
  	t_entrenador* entrenador_new  = malloc(sizeof(t_entrenador));
  	entrenador_new->nombre=nombre;
  	entrenador_new->ruta_pokedex = ruta;
  	entrenador_new->configuracion = configuracion_metadata_create(nombre,ruta);
  	entrenador_new->simbolo= entrenador_simbolo(entrenador_new->configuracion);
  	entrenador_new->mapas= entrenador_mapas(entrenador_new->configuracion);
  	entrenador_new->mapa_actual = list_get(entrenador_new->mapas, 0);
  	entrenador_new->vidas= entrenador_vidas(nombre,ruta);
  	entrenador_new->reintentos=0;
  	entrenador_new->tiempo_total_aventura=0;
  	entrenador_new->tiempo_bloqueado_pokenest=0;
  	entrenador_new->muertes=0;
  	entrenador_new->ubicacion->posx=0;
  	entrenador_new->ubicacion->posy=0;
  	entrenador_new->pokenest->posx=NULL;
  	entrenador_new->pokenest->posy=NULL;
  	return(entrenador_new);
  }

char* entrenador_simbolo(t_config* configuracion){
	return(config_get_string_value(configuracion, "Simbolo"));
}

int entrenador_vidas(t_config* configuracion){
	return(config_get_int_value(configuracion, "Vidas"));
}

t_list* entrenador_mapas(t_config* configuracion){
	return(config_get_array_value(configuracion, "Hoja de Viaje"));
}



/* ¿COMO ES EL LOOP DEL ENTRENADOR?
 * 1.	tengo pokemons por atrapar?
 * 		si-> conozco el pokenest? no->mando el fin
 * 2.	conozco el pokenest?
 * 		si-> me muevo hacia alla, choque?  no->conocer pokenest
 * 3.	choque?
 * 		si-> capture?	no->me muevo hacia alla, choque?
 * 4. 	capture?
 * 		si-> set pokenest null, tengo pokemons por atrapar?	no-> depende
 */


void conocer_siguiente_pokenest(t_entrenador* entrenador){
	char* pokemonBuscado;
	pokemonBuscado = list_get(entrenador->mapa_actual->objetivos, 0);
	char* mensaje = string_new();
	string_append(&mensaje, "up;");
	string_append(&mensaje, pokemonBuscado);
	sent(entrenador->mapa_actual->server, mensaje, sizeof(mensaje));
	char* mensaje_recibido = malloc(sizeof(char) * 9 );
	recv(entrenador->mapa_actual->server, mensaje_recibido , sizeof((char) * 9);
	char* ubiqueishon= string_new();
	ubiqueishon = string_substring(mensaje2, "ur;");
	char* posicionx = string_new();
	posicionx = string_substring_until(ubiqueishon,2);
	char* ubiqueishon2 = string_new();
	ubiqueishon2 = string_substring(ubiqueishon, ";");
	char* posiciony = string_new();
	posiciony = string_substring_until(ubiqueishon,2);
	entrenador->ubicacion->posx = posicionx;
	entrenador->ubicacion->posy = posiciony;
	free(mensaje);
	free(mensaje_recibido);
	free(ubiqueishon);
	free(ubiqueishon2);
	free(posicionx);
	free(posiciony);
}

void tengo_pokemons_por_atrapar(t_entrenador entrenador){
	if( list_is_empty(entrenador->mapa_actual->objetivos)) mando_el_fin(entrenador->mapa_actual, entrenador);
}

int conozco_el_pokenest(t_entrenador entrenador){
	if((entrenador->pokenest->posx) != NULL) return 1;
	return 0;
}

/*
 * VER ACA
 */
void mando_el_fin(t_entrenador entrenador){
	send(entrenador->mapa_actual->id, "fp; ", sizeof(char)*5);
	list_remove(entrenador->mapas, 0);
	entrenador->mapa_actual= list_get(entrenador->mapas, 0);
}

int puedo_moverme(t_entrenador entrenador){
	sent(entrenador->mapa_actual->id, "puedo moverme?", sizeof(eso));
	char *mensajote = malloc(sizeof(char) * 4);
	recv(entrenador->mapa_actual->id, mensajote, sizeof(mensajote));
	return 1;

}


//MIRA ESTO AGUS//
void ejecutar_lo_que_deba_hacer(t_entrenador *entrenador)
{
	while(esperar_turno(entrenador->mapa_actual->server) !=1)
	{no_hacer_nada()} //<--fijate que claramente, esto lo debe controlar un semaforo// }

	//si estoy aca es que sali del while, osea esperar_turno devolvio 1, que es TURNO CONCEDIDO
	//supongamos que tengo qe moverme
	moverme(entrenador);

}
void moverme(t_entrenador *entrenador)
{
	//TODA LA LOGICA PARA SABER A DONDE TENES QUE MOVERTE
	//GENERAS UN STRING CON EL FORMATO "POSX;POSY"
	char *posicion = generar_string_posicion(x,y);
	solicitar_moverse(entrenador->mapa_actual->server, posicion)
}

//REPLANTER
void moverse_hacia_pokenest(t_entrenador entrenador)
	{
		while(((entrenador->ubicacion->posx != entrenador->pokenest->posx) || (entrenador->ubicacion->posy != entrenador->pokenest->posy)) && puede_moverme(entrenador))
		{
			if(entrenador->ubicacion->posx > entrenador->pokenest->posx)
			{
				sent(entrenador->mapa_actual->id, "miz", sizeof(char)*4);
				entrenador->ubicacion->posx--;
			}
			else if(entrenador->ubicacion->posx < entrenador->pokenest->posx)
			{
				sent(entrenador->mapa_actual->id, "mde", sizeof(char)*4);
				entrenador->ubicacion->posx++;
			}
			if (entrenador->ubicacion->posy > entrenador->pokenest->posy && puedo_moverme(entrenador))
			{
				sent(entrenador->mapa_actual->id, "mab", sizeof(char)*4);
				entrenador->ubicacion->posy--;
			}
			else if(entrenador->ubicacion->posy < entrenador->pokenest->posy && puedo_moverme(entrenador))
			{
				sent(entrenador->mapa_actual->id, "mar", sizeof(char)*4);
				entrenador->ubicacion->posy++;
			}

	}
	    };


int he_chocado_con_pokenest(t_entrenador entrenador){
	if(entrenador->pokenest->posx == entrenador->ubicacion->posx && entrenador->pokenest->posy == entrenador->ubicacion->posy)// COINCIDE POS
		return 1;
	return 0;
	}

void choque_con_pokenest(t_entrenador entrenador){
	if(he_chocado_con_pokenest(entrenador)) capturar_pokemon(entrenador->mapa_actual,entrenador);
	// caso deadlock
}

void capturar_pokemon(t_entrenador entrenador){
	char* pokemonBuscado = string_new();
	pokemonBuscado = list_get(entrenador->mapa_actual->objetivos, 0); // funcion head.
	char* mensaje = string_new();
	string_append(&mensaje, "cp;");
	string_append(&mensaje, pokemonBuscado);
	sent(entrenador->mapa_actual->id, mensaje, sizeof(mensaje));
	char* mensaje_recibido = malloc(sizeof(char) * 4 );
	recv(entrenador->mapa_actual->id, mensaje_recibido, sizeof(char) * 4);
	setear_a_null_pokenest(entrenador);
	conozco_el_pokenest(entrenador);
	}
}

void setear_a_null_pokenest(t_entrenador entrenador){
	entrenador->pokenest->posx = NULL;
	entrenador->pokenest->posy = NULL;
}

