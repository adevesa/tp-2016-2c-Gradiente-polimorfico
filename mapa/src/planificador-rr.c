/*
 * planificador-rr.c
 *
 *  Created on: 16/9/2016
 *      Author: utnso
 */
#include "planificador-rr.h"

t_planificador_rr *planificador;
extern sem_t semaforo_entrenadores_listos;

/*-----------------------------------EXECUTE PLANIFICADOR RR---------------------------------------------------------*/
void* ejecutar_planificador_rr(void* arg)
{
	planificador_inicia_log();
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
	if(q == 0) {return 1;}
	else {return 0;}
}

void planificador_rr_organiza_entrenadores()
{

	if(encolacion_entrenadores_iniciada == NO_INICIADO)
	{
		planificador_inicia_encolacion_nuevos_entrenadores();
	}
	while(mapa_decime_si_planificador_es(PLANIFICADOR_RR))
	{
		if(queue_is_empty(planificador->listas_y_colas->cola_entrenadores_listos))
		{
			sem_wait(&semaforo_entrenadores_listos);
		}
		t_entrenador *entrenador_listo = planificador_pop_entrenador_listo(planificador);
		int quamtum_restante = planificador->quantum;
		int estado_anterior = entrenador_listo->estado;
		mapa_cambiale_estado_a_entrenador(entrenador_listo, EXECUTE, estado_anterior);
		planificador_rr_es_el_turno_de(entrenador_listo, &quamtum_restante);
		planificador_volve_a_encolar_a_listo_si_es_necesario(entrenador_listo);
		planificador_revisa_si_hay_recursos_para_desbloquear_entrenadores();
	}
}

void planificador_rr_es_el_turno_de(t_entrenador *entrenador_listo, int *quamtum)
{
	while(!quamtum_se_termino(*quamtum) && (entrenador_listo->objetivo_cumplido != ABORTADO) )
	{
		usleep(planificador->retardo*1000);
		if(mapa_decime_si_entrenador_esta_listo_pero_estaba_bloqueado(entrenador_listo))
		{
			planificador_rr_volve_a_bloquear_a_entrenador_si_es_necesario(entrenador_listo, quamtum);
			planificador_rr_dale_nuevo_turno_a_entrenador(entrenador_listo,quamtum);
		}
		else
		{
			planificador_rr_dale_nuevo_turno_a_entrenador(entrenador_listo,quamtum);
		}
	}

}

void planificador_rr_dale_nuevo_turno_a_entrenador(t_entrenador *entrenador_listo, int *quamtum_restante)
{
	enviar_mensaje_a_entrenador(entrenador_listo,OTORGAR_TURNO,NULL);
	char *mensaje_del_entrenador = escuchar_mensaje_entrenador(entrenador_listo, SOLICITUD_DEL_ENTRENADOR);
	switch(tratar_respuesta(mensaje_del_entrenador,entrenador_listo))
	{
		case(ENTRENADOR_DESCONECTADO):
			{
				planificador_aborta_entrenador(entrenador_listo);
				quamtum_restante = 0;
			};break;
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
				planificador_entrenador_quiere_capturar_pokemon(entrenador_listo, PERMITIR_SI_ES_POSIBLE);
				if(entrenador_listo->estado == BLOQUEADO)
				{
					quamtum_restante = 0;
				}
				else { quamtum_disminuite(quamtum_restante); }
			} break;
		default:
			{
				planificador_aborta_entrenador(entrenador_listo);
				quamtum_restante = 0;
			}
	}
}

void planificador_rr_volve_a_bloquear_a_entrenador_si_es_necesario(t_entrenador *entrenador, int *quamtum)
{
	if(mapa_decime_si_hay_pokemones_en_pokenest(entrenador->pokenest_objetivo))
	{
		planificador_entrega_pokemon_a(entrenador);
		quamtum_disminuite(quamtum);
	}
	else
	{
		mapa_cambiale_estado_a_entrenador(entrenador,BLOQUEADO,LISTO);
		planificador_push_entrenador_a_bloqueado(entrenador);
		int cero = 0;
		quamtum = &cero;
	}

}
