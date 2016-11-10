/*
 * deadlock.h
 *
 *  Created on: 3/11/2016
 *      Author: utnso
 */

#ifndef DEADLOCK_H_
#define DEADLOCK_H_

t_pokemon* conseguir_mejor_pokemon(t_entrenador* entrenador);
t_pokemon* obtener_pokemon_de_entrenador(t_list* pokemones_del_entrenador, int index);
char* array_get_last_element(char* path);
char* adaptar_nombre_pokemon(char* nombre_sucio);

#endif /* DEADLOCK_H_ */
