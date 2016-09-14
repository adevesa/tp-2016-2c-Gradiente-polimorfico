/*
 * entrenador-commons.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "entrenador-commons.h"

t_entrenador *entrenador;

void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex)
{
	entrenador = entrenador_create(nombre_entrenador, ruta_pokedex);
	entrenador_recorre_hoja_de_viaje();
	//SI ESTA ACA, ES QUE YA TERMINO DE RECORRER
}

void entrenador_recorre_hoja_de_viaje()
{
	//OJO, REVISAR SI LA LISTA NO SE MUEVE DE LUGAR//
	int i= 0;
	while(!list_is_empty(entrenador->hoja_de_viaje))
	{
		entrenador_busca_mapa(i);
		conectar_a_mapa(entrenador->mapa_actual);
		entrenador_cumpli_objetivos_del_mapa();
		list_remove(entrenador->hoja_de_viaje, i);
		i++;
	}
}

void entrenador_busca_mapa(int index)
{
	entrenador->mapa_actual = mapa_create(list_get(entrenador->hoja_de_viaje, index), entrenador->ruta_pokedex);
}

void entrenador_cumpli_objetivos_del_mapa()
{
	int i;
	int cantidad_objetivos = list_size(entrenador->mapa_actual->objetivos);
	for(i=0; i<cantidad_objetivos; i++)
	{
		entrenador_espera_turno();
		entrenador_pedi_ubicacion_pokenest(i);
		entrenador_cumpli_objetivo(i);
	}
	entrenador_avisa_que_terminaste_en_este_mapa();
	close(entrenador->mapa_actual->server);
	list_destroy_and_destroy_elements(entrenador->mapa_actual->objetivos, free);
}

void entrenador_espera_turno()
{
	char *respuesta = esperar_respuesta(entrenador->mapa_actual);
}

void entrenador_pedi_ubicacion_pokenest(int indice_objetivo)
{
	solicitar_ubicacion_pokenest(entrenador->mapa_actual, list_get(entrenador->mapa_actual->objetivos,indice_objetivo));
	char *respuesta = esperar_respuesta(entrenador->mapa_actual);
	tratar_respuesta(respuesta, entrenador);
}

void entrenador_cumpli_objetivo(int indice_obejtivo)
{
	while(!entrenador_llego_a_destino())
	{
		entrenador_espera_turno();
		entrenador_camina_hacia_destino();
	}
	entrenador_espera_turno();
	entrenador_captura_pokemon(indice_obejtivo);
}

/*--------------------------------------------LOGICA DEL MOVIMIENTO DEL ENTRENADOR---------------------------------*/
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

void entrenador_camina_hacia_destino()
{
	if(entrenador_llego_a_posicion_y())
	{
		entrenador_movete_en_x();
		entrenador_informa_movimiento();
	}
	if(entrenador_llego_a_posicion_x())
	{
		entrenador_movete_en_y();
		entrenador_informa_movimiento();
	}
	else
	{
		entrenador_movete_alternado();
		entrenador_informa_movimiento();
	}
}
//PREGUNTAR SI LAS POKENEST SIEMPRE ESTARAN EN PRIMER CUADRANTE
void entrenador_movete_en_x()
{
	entrenador->ubicacion->x++;
}

void entrenador_movete_en_y()
{
	entrenador->ubicacion->y++;
}

int paso_anterior_fue_en_x()
{
	if(entrenador->paso_anterior->x == 1)
	{
		return 1;
	}
	else { return 0; }
}

void entrenador_movete_alternado()
{
	if(paso_anterior_fue_en_x())
	{
		entrenador_movete_en_y();
		entrenador->paso_anterior->x = 0;
		entrenador->paso_anterior->y = 1;
	}
	else
	{
		entrenador_movete_en_x();
		entrenador->paso_anterior->x = 1;
		entrenador->paso_anterior->y = 0;
	}
}

void entrenador_informa_movimiento()
{
	char *posicion = string_itoa(entrenador->ubicacion->x);
	string_append(&posicion, ",");
	char *posy = string_itoa(entrenador->ubicacion->y);
	string_append(&posicion, posy);
	solicitar_moverse(entrenador->mapa_actual, posicion);
	free(posy);

}

/*--------------------------------------------LOGICA DE ATRAPAR POKEMON-------------------------------------*/
void entrenador_captura_pokemon(int indice_objetivo)
{
	solicitar_captura_pokemon(entrenador->mapa_actual,list_get(entrenador->mapa_actual->objetivos, indice_objetivo));
	char *respuesta = recibir_mensaje(entrenador->mapa_actual->server,3);
	tratar_respuesta(respuesta, entrenador);
}

void entrenador_espera_desbloqueo()
{
	char *respuesta = recibir_mensaje(entrenador->mapa_actual->server,3);
	tratar_respuesta(respuesta, entrenador);
}
/*------------------------------------------LOGICA DE TERMINAR EN EL MAPA------------------------------------*/
void entrenador_avisa_que_terminaste_en_este_mapa()
{
	notificar_fin_objetivos(entrenador->mapa_actual);
	char *respuesta = recibir_mensaje(entrenador->mapa_actual->server,3);
	tratar_respuesta(respuesta, entrenador);
}
