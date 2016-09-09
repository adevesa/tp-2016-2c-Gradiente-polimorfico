/*
 * mapa-testing.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */
#include "mapa-testing.h"


void mapa_reconoce_nombre()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->nombre, "mapa1");
}

void mapa_reconoce_rutaPokedex()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->ruta_pokedex, "/home/utnso/Directorio/Pokedex");
}


void mapa_reconoce_tiempo_deadlock()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->tiempo_chequeo_deadlock,10000);
}

void mapa_reconoce_tiempo_batalla()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->batalla,1);
}

void mapa_reconoce_puerto()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->info_socket->puerto,5001);
}

void mapa_reconoce_ip()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->info_socket->ip,"127.0.0.1");
}

void mapa_reconoce_algoritmo_completo()
{
	t_mapa *new_mapa = mapa_create("mapa1","/home/utnso/Directorio/Pokedex");
	CU_ASSERT_EQUAL(new_mapa->info_algoritmo->algoritmo,"RR");
	CU_ASSERT_EQUAL(new_mapa->info_algoritmo->quamtum,3);
	CU_ASSERT_EQUAL(new_mapa->info_algoritmo->retardo,500);

}
