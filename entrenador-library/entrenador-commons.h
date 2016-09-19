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

int entrenador_llego_a_destino();
void entrenador_camina_hacia_destino();
int ubicacion_coincide(t_ubicacion *ubicacion1,t_ubicacion *ubicacion2);
int entrenador_llego_a_posicion_y();
int entrenador_llego_a_posicion_x();
int paso_anterior_fue_en_x();

void entrenador_movete_en_x();
void entrenador_movete_en_y();
void entrenador_movete_alternado();
void entrenador_informa_movimiento();
void entrenador_avisa_que_terminaste_en_este_mapa();
void entrenador_captura_pokemon(int indice_obejtivo);;

#endif /* ENTRENADOR_COMMONS_H_ */
