/*
 * bloques_filesystem.h
 *
 *  Created on: 4/9/2016
 *      Author: utnso
 */

#ifndef BLOQUES_FILESYSTEM_H_
#define BLOQUES_FILESYSTEM_H_

//Divide dos numeros, si el resto es distinto de 0 lo redondea hacia arriba.
int redondear_si_es_necesario(double x, double y);

//Devuelve la cantidad de bloques que posee cada estructura despendiendo
//del tamaño del disco (en bytes)
int bloques_en_header();
int bloques_en_bidmap(double tamanio_en_bytes);
int bloques_en_tabla_de_archivos();
int bloques_tabla_de_asignacion(double tamanio_en_bytes);
int bloques_de_datos(double tamanio_en_bytes);

//----------FUNCIONES DEL BITMAP--------------//
//dado un array de enteros muestra la posicion del primero que este libre
int primer_bloque_libre(int* array);

//Dada la posicion de un bloque y un array de enteros cambia su estado
//si estaba ocupado pasa a desocupado
//si esta desocupado pasa a ocupado
void cambiar_estado_bloque(int posicion, int* array);

//-----------------TABLA DE ASIGNACIONES--------------------//

//Dado un bloque actual y un array de enteros muestra la posicion del proximo bloque
int proximo_bloque(int bloque_actual, int* array);

//----------------HEADER----------------------//
//No estoy seguro si el filesystem ya te lo genera a partir de un tamaño en disco pero por si acaso lo cree
//osada_header* header_create(double tamanio_en_bytes);

#endif /* BLOQUES_FILESYSTEM_H_ */
