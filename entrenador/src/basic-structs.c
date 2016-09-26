/*
 * basic-structs.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "basic-structs.h"

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
  	entrenador_new->directorio_de_bill = obtener_direccion_directorio_de_bill(ruta, nombre);
  	entrenador_new->simbolo= entrenador_simbolo(entrenador_new->configuracion);
  	entrenador_new->hoja_de_viaje= entrenador_hoja_de_viaje(entrenador_new->configuracion);
  	entrenador_new->vidas= entrenador_vidas(entrenador_new->configuracion);
  	entrenador_new->reintentos=0;
  	entrenador_new->tiempo_total_aventura=0;
  	entrenador_new->tiempo_bloqueado_pokenest=0;
  	entrenador_new->muertes=0;
  	entrenador_new->ubicacion = ubicacion_create(0,0);
  	entrenador_new->paso_anterior = ubicacion_create(0,0);
  	entrenador_new->cantidad_de_veces_bloqueado = 0;
  	return(entrenador_new);
  }

t_ubicacion* ubicacion_create(int x, int y)
{
	t_ubicacion *new_ubi = malloc(sizeof(t_ubicacion));
	new_ubi->x=x;
	new_ubi->y=y;
	return new_ubi;
}

t_mapa* mapa_create(char* nombre_mapa, char *ruta_pokedex, t_entrenador *entrenador)
{
	t_config *config = configuracion_metadata_mapa_create(nombre_mapa, ruta_pokedex);
	t_mapa *mapa_new = malloc(sizeof(t_mapa));
	mapa_new->nombre = nombre_mapa;
	mapa_new->puerto = config_get_string_value(config, "Puerto");
	mapa_new->ip = config_get_string_value(config, "IP");
	mapa_new->objetivos = asociar_objetivos_por_mapa(nombre_mapa,entrenador);
	return mapa_new;
	config_destroy(config);
}

t_config* configuracion_metadata_mapa_create(char *nombre, char *ruta)
{
	char *ruta_final = string_new();
	ruta_final = obtener_ruta_especifica(ruta, "Mapas", nombre);
	ruta_final = obtener_ruta_especifica(ruta_final, "metadata", NULL);
	t_config *config_new = config_create(ruta_final);
	return config_new;;
}
/*--------------------------------------------OBTENCION DE DATOS DEL ENTRENADOR----------------------------------------------------------*/
char* obtener_direccion_directorio_de_bill(char* ruta_pokedex, char* nombre)
{
	char *ruta_final = obtener_ruta_especifica(ruta_pokedex, "Entrenadores",nombre);
	char *directorio = string_new();
	string_append(&directorio, "Dir de Bill/");
	ruta_final = obtener_ruta_especifica(ruta_final, directorio, NULL);
	return ruta_final;
}

t_list* entrenador_hoja_de_viaje(t_config* configuracion)
{
	char *resultado = config_get_string_value(configuracion, "hojaDeViaje");
	char **mapas_a_recorrer = mapas_a_Recorrer(resultado);
	t_list *hoja_de_viaje = foreach_hoja_de_viaje(mapas_a_recorrer);
	return hoja_de_viaje;
}

char** mapas_a_Recorrer(char *mapas_con_corchetes)
{
	char **sin_corchete_izquierdo = string_split(mapas_con_corchetes, "[");
	char *aux_sin_corchete_izquierdo = string_new();
	string_append(&aux_sin_corchete_izquierdo, sin_corchete_izquierdo[0]);


	char **sin_corchete_derecho = string_split(aux_sin_corchete_izquierdo,"]");
	char *aux_sin_corchete_derecho = string_new();
	string_append(&aux_sin_corchete_derecho, sin_corchete_derecho[0]);

	char **por_separado = string_split(aux_sin_corchete_derecho, ",");

	return (por_separado);
	free(aux_sin_corchete_derecho);
	free(aux_sin_corchete_izquierdo);
}

t_list* foreach_hoja_de_viaje(char **hoja_de_viaje)
{
	int cantidad_de_elementos_en_hoja =0;
	int i = 0;
	while(hoja_de_viaje[i] !=NULL)
	{
		cantidad_de_elementos_en_hoja++;
		i++;
	}

	t_list *lista = list_create();
	for(i=0; i<cantidad_de_elementos_en_hoja;i++)
	{
		list_add(lista, hoja_de_viaje[i]);
	}
	return lista;
}

t_list* asociar_objetivos_por_mapa(char *nombre_mapa, t_entrenador *entrenador)
{
	char *objetivo_especifico = string_new();
	string_append(&objetivo_especifico,"obj[");
	string_append(&objetivo_especifico,nombre_mapa);
	string_append(&objetivo_especifico, "]");
	char *objetivos = config_get_string_value(entrenador->configuracion, objetivo_especifico);
	char **objetivos_por_separado = mapas_a_Recorrer(objetivos);
	t_list *lista = foreach_hoja_de_viaje(objetivos_por_separado);
	return lista;
}

char* entrenador_simbolo(t_config* configuracion)
{
	return(config_get_string_value(configuracion, "simbolo"));
}

int entrenador_vidas(t_config* configuracion)
{
	return(config_get_int_value(configuracion, "vidas"));
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
