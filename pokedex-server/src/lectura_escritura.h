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

enum
{
	HAY_ESPACIO_SUFICIENTE = 1,
	NO_HAY_ESPACIO_SUFICIENTE = 0
};
/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
int osada_check_space_to_write(t_to_be_write *to_write);
int calcular_espacio_disponible_ultimo_bloque(osada_file *file);
void impactar_en_tabla_de_asignaciones(int posicion, int valor);


void osada_write_file(t_to_be_write* file);

void asignar_nuevo_bloque_datos(t_file_osada *archivo);

int calcular_bloque_relativo_datos_dado_absoluto(int numero_bloque_absoluto);

/*----------------------------------------------LECTURA-----------------------------------------------------------*/

/*
 * @NAME:void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
 * @DESC: Dado un archivo osada,y un disco, devuelve TODA la información completa de dicho archivo
 */
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
int calcular_byte_final_a_recuperar_de_file(int file_size); //<-- La usa la función anteriror

void* osada_b_read_file(osada_file *file, t_disco_osada *disco, t_to_be_read *to_read);
int calcular_bloque_final_por_leer(int bloque_inicial,t_to_be_read *to_read);
#endif /* LECTURA_ESCRITURA_H_ */
