/*
 * planificador-rr.c
 *
 *  Created on: 16/9/2016
 *      Author: utnso
 */
#include "planificador-rr.h"

t_planificador_rr *planificador;

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg)
{
	planificador = planificador_rr_create();
	planificador_rr_organiza_entrenadores();
	pthread_exit(NULL);
}

/*--------------------------------------------ROUND ROBBIN----------------------------------------------------------*/
void quamtum_disminuite(int *q)
{
	*q = *q -1;
}

int quamtum_se_termino(int q)
{
	if(q == 0) {return 0;}
	else {return 1;}
}

void planificador_rr_organiza_entrenadores()
{
	while(mapa_decime_si_planificador_es_rr())
	{
		t_entrenador *entrenador_listo = planificador_pop_entrenador_listo(planificador);
		int quamtum_restante = planificador->quantum;
		planificador_rr_es_el_turno_de(entrenador_listo, &quamtum_restante);
		planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores();
	}
}

void planificador_rr_es_el_turno_de(t_entrenador *entrenador_listo, int *quamtum)
{
	while(!quamtum_se_termino(*quamtum))
	{
		if(mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(entrenador_listo))
		{
			planificador_rr_volve_a_bloquear_a_entrenador_si_es_necesario(entrenador_listo, quamtum);
		}
		else
		{
			planificador_rr_dale_nuevo_turno_a_entrenador(entrenador_listo,quamtum);
		}
	}
}

void planificador_rr_dale_nuevo_turno_a_entrenador(t_entrenador *entrenador_listo, int *quamtum_restante)
{
	enviar_mensaje_a_entrenador(entrenador_listo, OTORGAR_TURNO, NULL);
	char * mensaje_del_entrenador = escuchar_mensaje_entrenador(entrenador_listo, SOLICITUD_DEL_ENTRENADOR);
	switch(tratar_respuesta(mensaje_del_entrenador,entrenador_listo))
	{
		case (ENTRENADOR_ESTA_BUSCANDO_COORDENADAS_POKENEST):
			{
				planificador_dale_coordenadas_a_entrenador(entrenador_listo);
				quamtum_disminuite(quamtum_restante);
			} break;
		case(ENTRENADOR_QUIERE_MOVERSE):
			{
				planificador_entrenador_se_mueve(entrenador_listo);
				quamtum_disminuite(quamtum_restante);
			} break;
		case(ENTRENADOR_QUIERE_CAPTURAR_POKEMON):
			{
				planificador_entrenador_quiere_capturar_pokemon(entrenador_listo);
				quamtum_disminuite(quamtum_restante);
			} break;
		case(ENTRENADOR_FINALIZO_OBJETIVOS):
			{
				planificador_finaliza_entrenador(entrenador_listo);
				quamtum_restante = 0;
			} break;
		default: perror("No se puede interpretar lo que quiere el entrnador");
	}
}

void planificador_rr_volve_a_bloquear_a_entrenador_si_es_necesario(t_entrenador *entrenador, int *quamtum)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		enviar_mensaje_a_entrenador(entrenador, OTORGAR_POKEMON,mapa_dame_pokemon_de_pokenest(entrenador->pokenest_objetivo)->ruta_en_pokedex);
		quamtum_disminuite(quamtum);
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador,BLOQUEADO,LISTO);
		planificador_push_entrenador_a_bloqueado(entrenador);
	}

}
