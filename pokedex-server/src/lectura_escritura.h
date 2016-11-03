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

/*----------------------------------------------TRUNCATE----------------------------------------------------------*/
void osada_b_truncate_file(t_to_be_truncate *to_truncate);
void osada_b_aumentar_tamanio(t_file_osada *file, int new_size);


void asignar_un_unico_bloque_si_es_necesario(t_file_osada *file, int tamanio_a_aumentar);
void asignar_n_bloques(t_file_osada *file, int n);
void asignar_nuevo_bloque_datos(t_file_osada *archivo);
void impactar_en_tabla_de_asignaciones(int posicion, int valor);
int calcular_bloque_relativo_datos_dado_absoluto(int numero_bloque_absoluto);

void osada_b_disminuir_tamanio(t_file_osada *file, int new_size);
int cantidad_de_bloques_a_desasignar(uint32_t tamanio_actual, int new_size);
void liberar_n_bloques(t_file_osada *file, int bloques_a_liberar);
void establecer_nuevo_feof_en_tabla_de_asignaciones(int posicion);
/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
void osada_write_file(t_to_be_write* file);

void osada_b_change_size_file(t_file_osada *file,int new_size);
void osada_b_alter_data_blocks(t_file_osada *file, void *data_new, t_list *bloques);
int osada_check_space_to_write(t_to_be_write *to_write);
int calcular_espacio_disponible_ultimo_bloque(osada_file *file);

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
