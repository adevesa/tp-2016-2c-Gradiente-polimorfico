#include "entrenador_commons.h"

/*--------------------------------------------CREATES---------------------------------------------------------------*/
t_config* configuracion_metadata_create(char *nombre, char *ruta)
{
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(ruta, "Entrenadores", nombre);
	ruta_final = obtener_ruta_especifica(ruta_final, "metadata", NULL);
	t_config *config_new = config_create(ruta_final);
	return config_new;
}

t_entrenador* entrenador_create(char* nombre, char* ruta)
  {
  	t_entrenador* entrenador_new  = malloc(sizeof(t_entrenador));
  	entrenador_new->nombre=nombre;
  	entrenador_new->ruta_pokedex = ruta;
  	entrenador_new->configuracion = configuracion_metadata_create(nombre,ruta);
  	entrenador_new->simbolo= entrenador_simbolo(entrenador_new->configuracion);
  	entrenador_new->mapas= entrenador_mapas(entrenador_new->configuracion);
  	//entrenador_new->mapa_actual = list_get(entrenador_new->mapas, 0);
  	entrenador_new->vidas= entrenador_vidas(entrenador_new->configuracion);
  	entrenador_new->reintentos=0;
  	entrenador_new->tiempo_total_aventura=0;
  	entrenador_new->tiempo_bloqueado_pokenest=0;
  	entrenador_new->muertes=0;
  	entrenador_new->ubicacion = ubicacion_create(0,0);
  	//entrenador_new->pokenest = ubicacion_create(NULL, NULL);  NO HACE FALTAAAA
  	//analizar_paso_anterior(entrenador_new); NO DEBERIA IR ACA
  	return(entrenador_new);
  }

t_ubicacion* ubicacion_create(int x, int y)
{
	t_ubicacion *new_ubi = malloc(sizeof(t_ubicacion));
	new_ubi->x=0;
	new_ubi->y=0;
	new_ubi->pa=0;
	return new_ubi;
}

/*--------------------------------------------FUNCIONES--------------------------------------------------------------*/
void analizar_paso_anterior(t_entrenador* entrenador_new){
	(entrenador_new->ubicacion->x == entrenador_new->pokenest->x)? entrenador_new->ubicacion->pa=-2 :
		(entrenador_new->ubicacion->y == entrenador_new->pokenest->y)? entrenador_new->ubicacion->pa=-1 :
		(entrenador_new->ubicacion->x == entrenador_new->pokenest->x && entrenador_new->ubicacion->x == entrenador_new->pokenest->x)? (entrenador_new->ubicacion->pa=-3) : (entrenador_new->ubicacion->pa=0);
}

t_list* entrenador_mapas(t_config* configuracion){
	return(config_get_array_value(configuracion, "hojaDeViaje"));
}


char* entrenador_simbolo(t_config* configuracion){
	return(config_get_string_value(configuracion, "simbolo"));
}

int entrenador_vidas(t_config* configuracion){
	return(config_get_int_value(configuracion, "vidas"));
}

//REVISAR ACA
t_list* entrenador_mapas(t_config* configuracion){
	return(config_get_array_value(configuracion, "hojaDeViaje"));
}





//COMPLETAR ACA
t_mapa* mapa_actual(t_entrenador* entrenador, char* mapa)
{
	t_mapa* mapa_actual = malloc(sizeof(t_mapa));
	mapa_actual->server = preguntar_por_server(mapa);
	mapa_actual->objetivos = entrenador_objetivos_por_mapa(entrenador, mapa, entrenador->ruta_pokedex);
	return(mapa_actual);
}

void conocer_siguiente_pokenest(t_entrenador* entrenador)
{
	char* pokemonBuscado;
	pokemonBuscado = list_get(entrenador->mapa_actual->objetivos, 0);
	char* mensaje = string_new();
	string_append(&mensaje, "up;");
	string_append(&mensaje, pokemonBuscado);
	sent(entrenador->mapa_actual->server, mensaje, sizeof(mensaje));

	char* mensaje_recibido = malloc(sizeof(char) * 12 );
	recv(entrenador->mapa_actual->server, mensaje_recibido , sizeof(mensaje_recibido));

	char* ubiqueishon= string_new();
	ubiqueishon = string_substring(mensaje_recibido, "ur;");

	decodificar_coordenadas(ubiqueishon, &(entrenador->pokenest->x), &(entrenador->pokenest->y));
	free(mensaje);
	free(mensaje_recibido);
	free(ubiqueishon);
}



int tengo_pokemons_por_atrapar(t_entrenador entrenador){
	if( list_is_empty(entrenador->mapa_actual->objetivos)) return 0;
	return (1);
}

int conozco_el_pokenest(t_entrenador entrenador){
	if((entrenador->pokenest->x) != NULL) return 1;
	return 0;
}

void mando_el_fin(t_entrenador entrenador){
	send(entrenador->mapa_actual->server, "fp; ", sizeof(char)*5);
	list_remove(entrenador->mapas, 0);
	entrenador->mapa_actual= list_get(entrenador->mapas, 0);
}

void moverme(t_entrenador* entrenador)
{
	switch(entrenador->ubicacion->pa){
			case(0):
					(entrenador->ubicacion->x > entrenador->pokenest->x) ? entrenador->ubicacion->x-- : entrenador->ubicacion->x++;
					(entrenador->ubicacion->x == entrenador->pokenest->x)? (entrenador->ubicacion->pa=-2) : (entrenador->ubicacion->pa=1);
					break;
			case(1):
					(entrenador->ubicacion->y > entrenador->pokenest->y) ? entrenador->ubicacion->y-- : entrenador->ubicacion->y++;
					(entrenador->ubicacion->y == entrenador->pokenest->y)? (entrenador->ubicacion->pa=-1) : (entrenador->ubicacion->pa=0);
					break;
			case(-1):
					(entrenador->ubicacion->x > entrenador->pokenest->x) ? entrenador->ubicacion->x-- : entrenador->ubicacion->x++;
					(entrenador->ubicacion->x == entrenador->pokenest->x) ?  (entrenador->ubicacion->pa=-3) : (entrenador->ubicacion->pa= -1);
					break;
			case(-2):
					(entrenador->ubicacion->y > entrenador->pokenest->y) ? entrenador->ubicacion->y-- : entrenador->ubicacion->y++;
					(entrenador->ubicacion->y == entrenador->pokenest->y) ?  (entrenador->ubicacion->pa=-3) : (entrenador->ubicacion->pa= -2);
					break;
			case(-3):
					choque_con_pokenest(entrenador);
					break;
			}
	char* mensaje = string_new();
	string_append(&mensaje, "mp;");
	char* posx = malloc(sizeof(char)*5);
	char* posy = malloc(sizeof(char)*5);
	itoa(entrenador->ubicacion->x,posx,10);
	itoa(entrenador->ubicacion->y,posy,10);
	string_append(&mensaje,posx);
	string_append(&mensaje,";");
	string_append(&mensaje,posy);
	solicitar_moverse(entrenador->mapa_actual->server, mensaje);
	free(mensaje);
	free(posx);
	free(posy);
}

void choque_con_pokenest(t_entrenador entrenador){
	capturar_pokemon(entrenador->mapa_actual,entrenador);
	// caso deadlock
}

void capturar_pokemon(t_entrenador entrenador){
	char* pokemonBuscado = string_new();
	pokemonBuscado = list_get(entrenador->mapa_actual->objetivos, 0);
	char* mensaje = string_new();
	string_append(&mensaje, "cp;");
	string_append(&mensaje, pokemonBuscado);
	sent(entrenador->mapa_actual->server, mensaje, sizeof(mensaje));
	char* mensaje_recibido = malloc(sizeof(char) * 4 );
	recv(entrenador->mapa_actual->server, mensaje_recibido, sizeof(char) * 4);
	setear_a_null_pokenest(entrenador);
	ejecutar_lo_que_deba_hacer(entrenador);
	free(pokemonBuscado);
	free(mensaje);
	free(mensaje_recibido);
	}

void setear_a_null_pokenest(t_entrenador entrenador){
	entrenador->pokenest->x = NULL;
	entrenador->pokenest->y = NULL;
	entrenador->ubicacion->pa = 0;
}

void ejecutar_lo_que_deba_hacer(t_entrenador *entrenador)
{
	while(esperar_turno(entrenador->mapa_actual->server) !=1)
	{no_hacer_nada();}

	(tengo_pokemons_por_atrapar(entrenador))?
			conozco_el_pokenest(entrenador)?
					moverse(entrenador) : conocer_siguiente_pokenest(entrenador)
					: mando_el_fin(entrenador->mapa_actual, entrenador);
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

void ejecutar_entrenador(char *nombre_entrenador, char *rutaPokedex) // EL MAIN
{
	t_entrenador *entrenador_new = entrenador_create(nombre_entrenador, rutaPokedex);
	ejecutar_lo_que_deba_hacer(entrenador_new);
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
