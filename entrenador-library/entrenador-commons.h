/*
 * entrenador-commons.h
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */

#ifndef ENTRENADOR_COMMONS_H_
#define ENTRENADOR_COMMONS_H_
#include "comunication.h"
#include "basic-structs.h"
#include "time.h"
#include "semaphore.h"
#include "pthread.h"

#define INICIO 0
#define FIN 1
#define AUMENTAx_DISMINUIy 2
#define DISMINUIx_AUMENTAy 3
#define AUMENTAx_AUMENTAy 4
#define DISMINUIx_DISMINUIy 5

#define AUMENTAR 6
#define DISMINUIR 7
#define EJE_X 8
#define EJE_Y 9
char *hora_de_inicio;
char *hora_de_fin;

void entrenador_registra_hora(int rango);

void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex);
void entrenador_recorre_hoja_de_viaje();
void entrenador_cumpli_objetivos_del_mapa();
void entrenador_cumpli_objetivo(int indice_objetivo);
void entrenador_busca_mapa(int index);

void entrenador_espera_turno();
void entrenador_espera_a_que_mapa_te_bloquee();
void entrenador_espera_a_que_mapa_te_desbloquee();

void entrenador_pedi_ubicacion_pokenest(int indice_objetivo);
void entrenador_recibi_y_copia_pokemon();
void entrenador_registra_tiempo_bloqueo(char *hora_inicio, char *hora_fin);

void entrenador_ubicate_para_donde_caminar();
void entrenador_movete_en_eje(int eje, int orientacion);
void entrenador_camina_hacia_destino(int orientacion_x, int orientacion_y);
int diferencia_de_posiciones(t_ubicacion *posicion_actual, t_ubicacion *posicion_final);

int entrenador_llego_a_destino();
void entrenador_camina_hacia_destino();
int ubicacion_coincide(t_ubicacion *ubicacion1,t_ubicacion *ubicacion2);
int paso_anterior_fue_en_x();
int entrenador_llego_a_posicion_y();
int entrenador_llego_a_posicion_x();

void entrenador_movete_alternado(int orientacion_x, int orientacion_y);
void entrenador_informa_movimiento();
void entrenador_avisa_que_terminaste_en_este_mapa();
void entrenador_captura_pokemon(int indice_obejtivo);;

#endif /* ENTRENADOR_COMMONS_H_ */
