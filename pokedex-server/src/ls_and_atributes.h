/*
 * ls_and_atributes.h
 *
 *  Created on: 13/10/2016
 *      Author: utnso
 */

#ifndef LS_AND_ATRIBUTES_H_
#define LS_AND_ATRIBUTES_H_

#include "osada.h"
#include "osada_generales.h"
#include "borrados.h"
/*----------------------------------------------LISTAR--------------------------------------------------------------------*/
t_list* osada_listar_hijos(char* path);
void agregar_a_lista_si_es_hijo(t_file_osada *path_padre, osada_file* hijo, t_list* lista);
int verificar_si_son_mismo_files(osada_file *file_actual, osada_file *file_expected);
int es_el_padre(osada_file* file_hijo,t_file_osada *path_padre);

/*---------------------------------------------TAMAÑO DE UN DIRECTORIO---------------------------------------------------*/
int osada_b_calculate_size_of_directory(char *path_directory);
int realizar_sumatoria_size_hijos(char *path);



#endif /* LS_AND_ATRIBUTES_H_ */
