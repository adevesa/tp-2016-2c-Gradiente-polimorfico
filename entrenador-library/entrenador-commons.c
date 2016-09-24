/*
 * entrenador-commons.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "entrenador-commons.h"

t_entrenador *entrenador;
sem_t turno_entrenador;
sem_t bloqueado;
sem_t desbloqueado;


void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex)
{
	sem_init(&turno_entrenador,0,0);
	sem_init(&bloqueado,0,0);
	sem_init(&desbloqueado,0,0);
	entrenador_registra_hora(INICIO);
	entrenador = entrenador_create(nombre_entrenador, ruta_pokedex);
	entrenador_recorre_hoja_de_viaje();
	entrenador_registra_hora(FIN);
	//SI ESTA ACA, ES QUE YA TERMINO DE RECORRER
}

void entrenador_registra_hora(int rango)
{
	switch(rango)
	{
	case(INICIO):hora_de_inicio = temporal_get_string_time(); break;
	case(FIN):
			{
				hora_de_fin = temporal_get_string_time();
				entrenador->tiempo_total_aventura = (int) diferencia_de_tiempos(hora_de_inicio,hora_de_fin);
			}break;
	}

}


/*--------------------------------------------LOGICA DE CUMPLIR LOS OBJETIVOS DE TODOS LOS MAPAS---------------------------------*/
void entrenador_recorre_hoja_de_viaje()
{
	int i;
	int cantidad = list_size(entrenador->hoja_de_viaje);
	for(i = 0; i<cantidad ; i++)
	{
		entrenador_busca_mapa(i);
		conectar_a_mapa(entrenador->mapa_actual);
		enviar_mensaje_a_mapa(entrenador->mapa_actual,OTORGAR_SIMBOLO_ENTRENADOR, entrenador->simbolo);
		entrenador_cumpli_objetivos_del_mapa();
	}
}

void entrenador_busca_mapa(int index)
{
	entrenador->mapa_actual = mapa_create(list_get(entrenador->hoja_de_viaje, index), entrenador->ruta_pokedex, entrenador);
}

/*--------------------------------------------LOGICA DE CUMPLIR LOS OBJETIVOS DE UN MAPA---------------------------------*/
void entrenador_cumpli_objetivos_del_mapa()
{
	int i;
	int cantidad_objetivos = list_size(entrenador->mapa_actual->objetivos);
	for(i=0; i<cantidad_objetivos; i++)
	{
		entrenador_espera_turno();
		sem_wait(&turno_entrenador);
		entrenador_pedi_ubicacion_pokenest(i);
		entrenador_cumpli_objetivo(i);
	}
	entrenador_espera_turno();
	sem_wait(&turno_entrenador);
	entrenador_avisa_que_terminaste_en_este_mapa();
	close(entrenador->mapa_actual->server);
	list_destroy_and_destroy_elements(entrenador->mapa_actual->objetivos, free);
}

void entrenador_espera_turno()
{
	char *turno =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_TURNO);
	if(string_equals_ignore_case(turno, "tr;"))
	{
		sem_post(&turno_entrenador);
	}
}

void entrenador_pedi_ubicacion_pokenest(int indice_objetivo)
{
	char *identificador_pokenest =list_get(entrenador->mapa_actual->objetivos,indice_objetivo);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,SOLICITAR_COORDENADAS_POKENEST ,identificador_pokenest);
	if(string_equals_ignore_case(escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_TURNO), "ur;"))
	{
		char *coordenada = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_COORDENADAS_POKENEST);
		t_ubicacion *ubicacion_pokenest = desarmar_coordenada(coordenada);
		entrenador->pokenest = ubicacion_pokenest;
	}
}

/*--------------------------------------------LOGICA DE CAMINAR HACIA POKENEST--------------------------------*/
void entrenador_cumpli_objetivo(int indice_obejtivo)
{
	while(!entrenador_llego_a_destino())
	{
		entrenador_espera_turno();
		sem_wait(&turno_entrenador);
		entrenador_ubicate_para_donde_caminar();
		entrenador_informa_movimiento();
	}
	entrenador_espera_turno();
	sem_wait(&turno_entrenador);
	entrenador_captura_pokemon(indice_obejtivo);
}

/*--------------------------------------------LOGICA DEL MOVIMIENTO DEL ENTRENADOR---------------------------------*/

void entrenador_ubicate_para_donde_caminar()
{
	switch(diferencia_de_posiciones(entrenador->ubicacion, entrenador->pokenest))
	{
		case(AUMENTAx_AUMENTAy): entrenador_camina_hacia_destino(AUMENTAR,AUMENTAR) ;break;
		case(AUMENTAx_DISMINUIy): entrenador_camina_hacia_destino(AUMENTAR,DISMINUIR);break;
		case(DISMINUIx_AUMENTAy): entrenador_camina_hacia_destino(DISMINUIR,AUMENTAR);break;
		case(DISMINUIx_DISMINUIy): entrenador_camina_hacia_destino(DISMINUIR,DISMINUIR);break;
	}
}

int diferencia_de_posiciones(t_ubicacion *posicion_actual, t_ubicacion *posicion_final)
{
	if((posicion_final->x - posicion_actual->x )>0)
	{
		if((posicion_final->y - posicion_actual->y) >0)
		{
			return AUMENTAx_AUMENTAy;
		}
		return AUMENTAx_DISMINUIy;
	}
	else
	{
		if((posicion_final->y - posicion_actual->y) >0)
		{
			return DISMINUIx_AUMENTAy;
		}
		return DISMINUIx_DISMINUIy;
	}
}

void entrenador_camina_hacia_destino(int orientacion_x, int orientacion_y)
{
	if(entrenador_llego_a_posicion_y())
	{
		entrenador_movete_en_eje(EJE_X,orientacion_x);

	}
	else
	{
		if(entrenador_llego_a_posicion_x())
		{
			entrenador_movete_en_eje(EJE_Y,orientacion_y);
		}
		else
		{
			entrenador_movete_alternado(orientacion_x,orientacion_y);
		}
	}

}

int entrenador_llego_a_destino()
{
	return ubicacion_coincide(entrenador->ubicacion, entrenador->pokenest);
}

int entrenador_llego_a_posicion_y()
{
	if(entrenador->pokenest->y == entrenador->ubicacion->y)
	{
		return 1;
	}
	else { return 0; }
}

int entrenador_llego_a_posicion_x()
{
	if(entrenador->pokenest->x == entrenador->ubicacion->x)
		{
			return 1;
		}
		else { return 0; }
}

int ubicacion_coincide(t_ubicacion *ubicacion1,t_ubicacion *ubicacion2)
{
	if((ubicacion1->x == ubicacion2->x) && (ubicacion1->y == ubicacion2->y))
	{
		return 1;
	}
	else {return 0; }
}

void entrenador_movete_en_eje(int eje, int orientacion)
{
	switch(eje)
	{
		case(EJE_X):
		{
			if(orientacion == AUMENTAR)
			{
				entrenador->ubicacion->x++;
			}
			else { entrenador->ubicacion->x--; }
		} break;
		case(EJE_Y):
		{
			if(orientacion == AUMENTAR)
			{
				entrenador->ubicacion->y++;
			}
			else { entrenador->ubicacion->y--; }
		}
	}

}

int paso_anterior_fue_en_x()
{
	if(entrenador->paso_anterior->x == 1)
	{
		return 1;
	}
	else { return 0; }
}

void entrenador_movete_alternado(int orientacion_x, int orientacion_y)
{
	if(paso_anterior_fue_en_x())
	{
		entrenador_movete_en_eje(EJE_Y,orientacion_y);
		entrenador->paso_anterior->x = 0;
		entrenador->paso_anterior->y = 1;
	}
	else
	{
		entrenador_movete_en_eje(EJE_X, orientacion_x);
		entrenador->paso_anterior->x = 1;
		entrenador->paso_anterior->y = 0;
	}
}

void entrenador_informa_movimiento()
{
	char *posicion_actual = armar_coordenada(entrenador->ubicacion->x, entrenador->ubicacion->y);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,REPORTAR_MOVIMIENTO, posicion_actual);
}

/*--------------------------------------------LOGICA DE ATRAPAR POKEMON-------------------------------------*/
void entrenador_captura_pokemon(int indice_objetivo)
{
	enviar_mensaje_a_mapa(entrenador->mapa_actual,SOLICITAR_CAPTURA_POKEMON,NULL);

	//MAPA ME BLOQUEA//
	entrenador_espera_a_que_mapa_te_bloquee();
	sem_wait(&bloqueado);
	entrenador->cantidad_de_veces_bloqueado++;
	char *hora_inicio_bloqueado = temporal_get_string_time();

	//ESPERO QUE MAPA ME DESBLOQUEE//
	entrenador_espera_a_que_mapa_te_desbloquee();
	sem_wait(&desbloqueado);
	char *hora_fin_desbloqueado = temporal_get_string_time();

	entrenador_recibi_y_copia_pokemon();
	entrenador_registra_tiempo_bloqueo(hora_inicio_bloqueado, hora_fin_desbloqueado);
}

void entrenador_recibi_y_copia_pokemon()
{
	char *solicitud = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	if(MAPA_ME_DA_POKEMON == mapa_me_dice(solicitud))
	{
		char *pokemon = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_POKEMON);
		copiar(pokemon,entrenador->directorio_de_bill);
	}
}

void entrenador_registra_tiempo_bloqueo(char *hora_inicio, char *hora_fin)
{
	int tiempo = diferencia_de_tiempos(hora_inicio, hora_fin);
	entrenador->tiempo_bloqueado_pokenest = entrenador->tiempo_bloqueado_pokenest + tiempo;
}

void entrenador_espera_a_que_mapa_te_bloquee()
{
	char *bloq =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_BLOQUEA);
	if(MAPA_ME_BLOQUEA == mapa_me_dice(bloq))
	{
		sem_post(&bloqueado);
	}
}

void entrenador_espera_a_que_mapa_te_desbloquee()
{
	char *bloq =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_BLOQUEA);
	if(MAPA_ME_DESBLOQUEA == mapa_me_dice(bloq))
		{
			sem_post(&desbloqueado);
		}
}
/*------------------------------------------LOGICA DE TERMINAR EN EL MAPA------------------------------------*/
void entrenador_avisa_que_terminaste_en_este_mapa()
{
	enviar_mensaje_a_mapa(entrenador->mapa_actual, REPORTAR_FIN_OBJETIVOS, NULL);
	char *esperar_confirmacion_de_medalla = escuchar_mensaje_mapa(entrenador->mapa_actual,MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	if(string_equals_ignore_case(esperar_confirmacion_de_medalla, "mr;"))
	{
		char *medalla_del_mapa = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_MEDALLA);
		char *ruta_medallas = obtener_ruta_especifica(entrenador->directorio_de_bill, "medallas", NULL);
		string_append(&ruta_medallas,"/");
		copiar(medalla_del_mapa, ruta_medallas);
		free(ruta_medallas);
	}
	else { perror("No se puede interpretar mensaje"); }
}

