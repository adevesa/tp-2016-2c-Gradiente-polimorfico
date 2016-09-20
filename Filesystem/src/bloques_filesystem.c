/*
 ============================================================================
 Name        : Filesystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "bloques_filesystem.h"
#include "osada.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


#define blocksize 64
//double blocksize = 64;

//-----------------CALCULAR TAMAÑO DE BLOQUES-----------------------//


int redondear_si_es_necesario(double x, double y){
	double resultado= ((x)/(y));
	double resto = fmod((x), (y)); //fmod: Devuelve el resto de d1/d2
	if (resto == 0){
		(int) resultado;
		return resultado;
		}
	else {
		(int) resultado;
		return (resultado+1);
		}
}

int bloques_en_header(){
	return 1;
}

int bloques_en_bidmap(double tamanio_en_bytes) {
	double cantidad = ((tamanio_en_bytes / blocksize) / 8);
	return redondear_si_es_necesario(cantidad, blocksize);
}

int bloques_en_tabla_de_archivos(){
 return 1024;
 }

int bloques_tabla_de_asignacion(double tamanio_en_bytes){
	int bidmap = bloques_en_bidmap(tamanio_en_bytes);
	double cantidad = ((tamanio_en_bytes / blocksize) - bloques_en_header() - bidmap - bloques_en_tabla_de_archivos()) * 4;
	return redondear_si_es_necesario(cantidad, blocksize);
}

int bloques_de_datos(double tamanio_en_bytes){
	int bidmap = bloques_en_bidmap(tamanio_en_bytes);
	int tda = bloques_tabla_de_asignacion(tamanio_en_bytes);
	double cantidad = (tamanio_en_bytes / blocksize) - bloques_en_header() - bidmap - bloques_en_tabla_de_archivos() - tda;
	return redondear_si_es_necesario(cantidad, blocksize);
}


//----------------------------------------------------------------------//

osada_header* header_create(double tamanio_en_bytes){
	osada_header* header  = malloc(sizeof(osada_header));
	header->magic_number[7] = "OsadaFS";
	header->version = 1;
	header->fs_blocks = tamanio_en_bytes / blocksize;
	header->bitmap_blocks = bloques_en_bidmap(tamanio_en_bytes);
	header->allocations_table_offset =  1 + bloques_en_bidmap(tamanio_en_bytes) + 1024;
	header->data_blocks = bloques_de_datos(tamanio_en_bytes);
	//recordatorio del agu: preguntar que meter en el pedding
	return(header);
}

//-----------------------BITMAP------------------------//


int primer_bloque_libre(int* array){
	int posicion = 0;
	while( array[posicion] != 0){
		posicion++;
	  }
		return posicion;
}


void cambiar_estado_bloque(int posicion, int* array){
	if (array[posicion] == 0){
		array[posicion] = 1;
	}
	else {
		array[posicion] = 0;
	}
}

//-----------------Tabla de Asignaciones--------------------//


int proximo_bloque(int bloque_actual, int* array){
	return array[bloque_actual];
}



int main(int argc, char** argv){

	int array[3] = {3,1,2};
	int resultado =proximo_bloque(0, array);
	printf("%d",resultado);
	return EXIT_SUCCESS;
}


