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
sem_t semaforo_terminacion;
sem_t semaforo_cola_entrenadores_sin_objetivos;
pthread_mutex_t mutex_manipular_cola_listos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_nuevos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_bloqueados = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_manipular_cola_finalizados = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cola_entrenadores_sin_objetivos = PTHREAD_MUTEX_INITIALIZER;

int servidor_debe_terminar = 0;
/*------------------------------------------EXECUTE----------------------------------------------------------------*/
void ejecutar_mapa(char *nombre, char *rutaPokedex)
{
	iniciar_semaforos();
	iniciar_logs(nombre);

	mapa = mapa_create(nombre, rutaPokedex);
	log_info(informe_mapa, "Mapa creado correctamente");

	//mapa_mostrate_en_pantalla();
	mapa_hacete_visible_para_entrenadores();
	planificador_create_segun_cual_seas();
	sem_wait(&semaforo_terminacion);
}

void iniciar_semaforos()
{
	sem_init(&semaforo_entrenadores_listos,1,0);
	sem_init(&semaforo_cola_bloqueados,1,0);
	sem_init(&semaforo_hay_algun_entrenador_listo,1,0);
	sem_init(&semaforo_cola_entrenadores_sin_objetivos,1,0);
	sem_init(&semaforo_servidor,1,0);
	sem_init(&semaforo_terminacion,0,0);
}

void iniciar_logs(char *nombre)
{
	char *nombre_log = string_new();
	string_append(&nombre_log, "Log ");
	string_append(&nombre_log,nombre);
	informe_mapa =log_create(nombre_log, "Proceso mapa", 0, LOG_LEVEL_INFO);
	free(nombre_log);
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

int mapa_decime_si_entrenador_esta_abortado(t_entrenador *entrenador)
{
	if(entrenador->objetivo_cumplido==ABORTADO)
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

int mapa_decime_si_entrenador_finalizo_su_objetivo(int socket_entrenador)
{
	char *key = string_itoa(socket_entrenador);
	return(dictionary_has_key(mapa->entrenadores->lista_entrenadores_finalizados, key));
	free(key);
}

void mapa_actualiza_distancia_del_entrenador(t_entrenador *entrenador)
{
	t_posicion *posicion_final = mapa_dame_coordenadas_de_pokenest(entrenador->pokenest_objetivo);
	int distancia_en_x;
	int distancia_en_y;
	if((posicion_final->x) - (entrenador->posicion_actual->x) > 0)
	{
		distancia_en_x = (posicion_final->x) - (entrenador->posicion_actual->x);
	}
	else
	{
		distancia_en_x = (entrenador->posicion_actual->x) - (posicion_final->x);
	}
	if((posicion_final->y) - (entrenador->posicion_actual->y) > 0)
	{
		distancia_en_y = (posicion_final->y) - (entrenador->posicion_actual->y);
	}
	else
	{
		distancia_en_y = (entrenador->posicion_actual->y)-(posicion_final->y);
	}
	entrenador->distancia_hasta_objetivo = distancia_en_x+distancia_en_y;
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

 	//t_pokeNest *pokenest = mapa_buscame_pokenest(id_pokenest);
 	//char id = (char)(pokenest->identificador[0]);
 	//restarRecurso(mapa->items_para_mostrar_en_pantalla, id);

 }

void mapa_agrega_pokenest_a_items_para_pantalla()
{
	//dictionary_iterator(mapa->pokeNests, closure);
	//log_info(informe_mapa, "Se agregan pokenest a interfaz gráfica");

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
	//CrearPersonaje(mapa->items_para_mostrar_en_pantalla, id,x,y);
	//nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	//INICIO LOG
	//char *mensaje_A_loggear = string_new();
	//string_append(&mensaje_A_loggear, "Se grafica a nuevo entrenador identificado con el simbolo ");
	//string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	//log_info(informe_mapa, mensaje_A_loggear);
	//free(mensaje_A_loggear);
	//FIN LOG


}

void mapa_mostra_actualizacion_de_entrenador(t_entrenador *entrenador)
{

	char id = (char)entrenador->simbolo_identificador[0];
	int x = entrenador->posicion_actual->x;
	int y= entrenador->posicion_actual->y;
	//MoverPersonaje(mapa->items_para_mostrar_en_pantalla, id,x,y);
	//nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

	//INICIO LOG
	//char *mensaje_A_loggear = string_new();
	//string_append(&mensaje_A_loggear, "Se grafica movimiento de entrenador identificado con el simbolo ");
	//string_append(&mensaje_A_loggear, entrenador->simbolo_identificador);
	//log_info(informe_mapa, mensaje_A_loggear);
	//free(mensaje_A_loggear);
	//FIN LOG


}

void mapa_borra_entrenador_de_pantalla(t_entrenador *entrenador)
{

	char id = *(entrenador->simbolo_identificador);
	//BorrarItem(mapa->items_para_mostrar_en_pantalla, id);
	//nivel_gui_dibujar(mapa->items_para_mostrar_en_pantalla, mapa->nombre);

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
	encolacion_entrenadores_iniciada = NO_INICIADO;
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(string_equals_ignore_case(mapa->info_algoritmo->algoritmo, "RR"))
	{
		pthread_create(&thread,NULL,ejecutar_planificador_rr, NULL);
		pthread_attr_destroy(&attr);
		log_info(informe_mapa, "Se crea hilo de planificador RR");
	}
	else{
		pthread_create(&thread,NULL,ejecutar_planificador_srdf,NULL);
		pthread_attr_destroy(&attr);
		log_info(informe_mapa, "Se crea hilo de planificador SRDF");
	}
}

int mapa_decime_si_planificador_es(int planificador)
{
	switch(planificador)
	{
		case(PLANIFICADOR_RR):return(string_equals_ignore_case(mapa->info_algoritmo->algoritmo,"RR"));break;
		case(PLANIFICADOR_SRDF):return(string_equals_ignore_case(mapa->info_algoritmo->algoritmo,"SRDF"));break;
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


