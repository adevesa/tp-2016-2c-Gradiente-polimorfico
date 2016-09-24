/*
 * map-commons.c
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */
#include "map-commons.h"

/*------------------------------------------VARIABLES GLOBALES----------------------------------------------------*/
t_mapa *mapa;

sem_t semaforo_entrenadores_listos;
sem_t semaforo_cola_bloqueados;
sem_t semaforo_hay_algun_entrenador_listo;
sem_t semaforo_servidor;
pthread_mutex_t mutex_manipular_cola_listos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_nuevos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_bloqueados = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_finalizados = PTHREAD_MUTEX_INITIALIZER;
int nadie_me_saca = 1;
int servidor_debe_terminar = 0;

/*------------------------------------------EXECUTE----------------------------------------------------------------*/
void ejecutar_mapa(char *nombre, char *rutaPokedex)
{
	iniciar_semaforos();
	iniciar_logs();

	mapa = mapa_create(nombre, rutaPokedex);
	log_info(informe_mapa, "Mapa creado correctamente");

	mapa_mostrate_en_pantalla();
	mapa_hacete_visible_para_entrenadores();
	planificador_create_segun_cual_seas();
	while(nadie_me_saca)
	{

	}
}

void iniciar_semaforos()
{
	sem_init(&semaforo_entrenadores_listos,1,0);
	sem_init(&semaforo_cola_bloqueados,1,0);
	sem_init(&semaforo_hay_algun_entrenador_listo,1,0);
	sem_init(&semaforo_servidor,1,0);
}

void iniciar_logs()
{
	informe_mapa = log_create("Informe mapa", "Proceso mapa", 0,LOG_LEVEL_INFO);
}

/*--------------------------------------------PRINCIPALES----------------------------------------------------------*/
t_posicion* mapa_dame_coordenadas_de_pokenest(char* identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = mapa_buscame_pokenest(identificador_pokemon);
	return pokeNest_buscado->posicion;
}

t_pokeNest* mapa_buscame_pokenest(char *identificador_pokemon)
{
	t_pokeNest *pokeNest_buscado = dictionary_get(mapa->pokeNests, identificador_pokemon);
	return pokeNest_buscado;
}

char* mapa_dame_medalla()
{
	char *ruta_medalla = obtener_ruta_especifica(mapa->ruta_pokedex, "Mapas",mapa->nombre);
	ruta_medalla = obtener_ruta_especifica(ruta_medalla,"medalla-",mapa->nombre);
	ruta_medalla = obtener_ruta_especifica(ruta_medalla,".jpg",NULL);
	return ruta_medalla;
}

int mapa_decime_si_hay_pokemones_en_pokenest(char *id_pokenest)
{
	t_pokeNest *pokenest = mapa_buscame_pokenest(id_pokenest);
	return (pokenest->cantidad_pokemones_disponibles>0);
}

char* mapa_dame_pokemon_de_pokenest(char *id_pokenest)
{
	t_pokeNest *pokenest = mapa_buscame_pokenest(id_pokenest);
	char *ruta_de_algun_pokemon = queue_pop(pokenest->pokemones);
	pokenest->cantidad_pokemones_disponibles--;
	return ruta_de_algun_pokemon;
}

int mapa_decime_si_entrenador_esta_bloqueado(t_entrenador *entrenador)
{
	if(entrenador->estado == BLOQUEADO)
	{
		return 1;
	}
	else {return 0;}
}

int mapa_decime_si_entrenador_estaba_bloqueado(t_entrenador *entrenador)
{
	if(entrenador->estado_anterior == 0)
		{
			return 1;
		}
		else {return 0;}
}

int mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(t_entrenador *entrenador)
{
	return (entrenador->estado==LISTO && mapa_decime_si_entrenador_estaba_bloqueado(entrenador));
}

void mapa_cambiale_estado_a_entrenador(t_entrenador *entrenador, int estado_entrante, int estado_saliente)
{
	entrenador->estado = estado_entrante;
	entrenador->estado_anterior = estado_saliente;
}

void mapa_devolve_pokemon_a_pokenest(char *ruta_pokemon)
{
	t_pokeNest *pokenest = dictionary_get(mapa->pokeNests, obtener_id_ponekest(ruta_pokemon));
	queue_push(pokenest->pokemones, ruta_pokemon);
	pokenest->cantidad_pokemones_disponibles++;
	char id = (char)(pokenest->identificador[0]);
	BorrarItem(mapa->items_para_mostrar_en_pantalla, id);
	CrearCaja(mapa->items_para_mostrar_en_pantalla,id,pokenest->posicion->x,pokenest->posicion->y,pokenest->cantidad_pokemones_disponibles);
}

int mapa_decime_si_planificador_es_rr()
{
	return(string_equals_ignore_case(mapa->info_algoritmo->algoritmo,"RR"));
}

int mapa_decime_si_entrenador_finalizo_su_objetivo(int socket_entrenador)
{
	char *key = string_itoa(socket_entrenador);
	return(dictionary_has_key(mapa->entrenadores->lista_entrenadores_finalizados, key));
	free(key);
}


/*--------------------------------------------------- FUNCIONES PARA GRAFICAR--------------------------------------------*/
 void mapa_mostrate_en_pantalla()
{

	mapa->items_para_mostrar_en_pantalla= list_create();

	int filas, columnas;
	nivel_gui_inicializar();
	nivel_gui_get_area_nivel(&filas, &columnas);
	mapa_agrega_pokenest_a_items_para_pantalla();
	nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	log_info(informe_mapa, "Mapa comienza a mostrar interfaz gráfica");

}

void mapa_actualiza_pokemones_disponibles_de_pokenest(char *id_pokenest)
 {

 	t_pokeNest *pokenest = mapa_buscame_pokenest(id_pokenest);
 	char id = (char)(pokenest->identificador[0]);
 	restarRecurso(mapa->items_para_mostrar_en_pantalla, id);

 }

void mapa_agrega_pokenest_a_items_para_pantalla()
{
	dictionary_iterator(mapa->pokeNests, closure);
	log_info(informe_mapa, "Se agregan pokenest a interfaz gráfica");

}

void closure(char *identificador, void *data )
{
	t_pokeNest *pokenest=(t_pokeNest*) data;
	char id = (char)(pokenest->identificador[0]);
	CrearCaja(mapa->items_para_mostrar_en_pantalla,id,pokenest->posicion->x,pokenest->posicion->y,pokenest->cantidad_pokemones_disponibles);
}

void mapa_mostra_nuevo_entrenador_en_pantalla(t_entrenador *entrenador)
{

	char id = (char)entrenador->simbolo_identificador[0];
	int x = entrenador->posicion_actual->x;
	int y= entrenador->posicion_actual->y;
	CrearPersonaje(mapa->items_para_mostrar_en_pantalla, id,x,y);
	nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Se grafica a nuevo entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_mapa, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG


}

void mapa_mostra_actualizacion_de_entrenador(t_entrenador *entrenador)
{

	char id = (char)entrenador->simbolo_identificador[0];
	int x = entrenador->posicion_actual->x;
	int y= entrenador->posicion_actual->y;
	MoverPersonaje(mapa->items_para_mostrar_en_pantalla, id,x,y);
	nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Se grafica movimiento de entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_mapa, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG


}

void mapa_borra_entrenador_de_pantalla(t_entrenador *entrenador)
{

	char id = *(entrenador->simbolo_identificador);
	BorrarItem(mapa->items_para_mostrar_en_pantalla, id);
	nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	//INICIO LOG
	char *mensaje_A_loggear = string_new();
	string_append(&mensaje_A_loggear, "Se borra de interfaz gráfica a entrenador identificado con el simbolo ");
	string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	log_info(informe_mapa, mensaje_A_loggear);
	free(mensaje_A_loggear);
	//FIN LOG

}

/*------------------------------ FUNCIONES PARA MANIPULACION DEL PLANIFICADOR--------------------------------------------*/
void planificador_create_segun_cual_seas()
{
	pthread_t thread;
	if(string_equals_ignore_case(mapa->info_algoritmo->algoritmo, "RR"))
	{
		pthread_create(&thread,NULL,ejecutar_planificador_rr, NULL);
		log_info(informe_mapa, "Se crea hilo de planificador RR");
	}
	else{
		pthread_create(&thread,NULL,ejecutar_planificador_srdf,NULL);
		log_info(informe_mapa, "Se crea hilo de planificador SRDF");
	}
}


/*----------------------- FUNCIONES PARA MANIPULACION DE ENTRENADORES (MEDIANTE SOCKETS)-------------------------------*/

void mapa_hacete_visible_para_entrenadores()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_t hilo_servidor;
	pthread_create(&hilo_servidor,&attr,ejecutar_servidor,(void*)mapa->info_socket);

	log_info(informe_mapa, "Se comienza a ejecutar hilo de escucha");
	pthread_attr_destroy(&attr);
}


