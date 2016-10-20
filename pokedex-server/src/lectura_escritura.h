/*
 * lectura_escritura.h
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */

#ifndef LECTURA_ESCRITURA_H_
#define LECTURA_ESCRITURA_H_
#include "osada.h"
#include "file_manipuling.h"
#include "osada_generales.h"

/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
void osada_write_file(t_to_be_write* file);
void osada_b_write_file_void(t_to_be_write* archivo);
int archivo_vacio(t_file_osada* arch);
void asignar_nuevo_bloque_datos(osada_file* archivo);

/*----------------------------------------------LECTURA-----------------------------------------------------------*/

/*
 * @NAME:void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
 * @DESC: Dado un archivo osada,y un disco, devuelve TODA la información completa de dicho archivo
 */
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
int calcular_byte_final_a_recuperar_de_file(int file_size); //<-- La usa la función anteriror

#endif /* LECTURA_ESCRITURA_H_ */
