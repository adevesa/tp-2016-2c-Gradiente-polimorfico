/*
 * planificador-rr.h
 *
 *  Created on: 16/9/2016
 *      Author: utnso
 */

#ifndef PLANIFICADOR_RR_H_
#define PLANIFICADOR_RR_H_
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include "map-commons.h"
#include "comunicacion.h"
#include "basic-structs.h"

enum RESPUESTA_DEL_ENTRENADOR
{
	ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST = 1,
	ENTRENADOR_QUIERE_MOVERSE = 2,
	ENTRENADOR_QUIERE_CAPTURAR_POKEMON = 3,
	ENTRENADOR_FINALIZO_OBJETIVOS = 4,
	ENTRENADOR_DESCONECTADO = 0
} ;

#define LISTO 1
#define BLOQUEADO 0
#define MUERTO -1


/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg);

/*--------------------------------------------ROUND ROBBIN----------------------------------------------------------*/
void planificador_rr_organiza_entrenadores();
void planificador_rr_es_el_turno_de(t_entrenador *entrenador_listo, int *quamtum);
void planificador_rr_dale_nuevo_turno_a_entrenador(t_entrenador *entrenador_listo, int *quamtum_restante);
void planificador_rr_volve_a_bloquear_a_entrenador_si_es_necesario(t_entrenador *entrenador, int *quamtum);;
int quamtum_se_termino(int q);
void quamtum_disminuite(int *q);


#endif /* PLANIFICADOR_RR_H_ */
