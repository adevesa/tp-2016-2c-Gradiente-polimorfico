/*
 * entrenador_commons.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef ENTRENADOR_COMMONS_H_
#define ENTRENADOR_COMMONS_H_

typedef struct coordenada{
	int posx;
	int posy;
}t_coordenada;

typedef struct mapa{
	int server;
	t_list* objetivos;
} t_mapa;

typedef struct entrenador{
	char* nombre;
	char* ruta_pokedex;
	t_config* configuracion;
	char* simbolo;
	t_list* mapas;
	t_mapa* mapa_actual;
	int vidas;
	int reintentos;
	int tiempo_total_aventura;
	int tiempo_bloqueado_pokenest;
	int muertes;
	t_coordenada* ubicacion;
	t_coordenada* pokenest;
}t_entrenador;

#endif /* ENTRENADOR_COMMONS_H_ */
