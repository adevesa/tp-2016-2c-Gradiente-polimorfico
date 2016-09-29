/*
 ============================================================================
 Name        : Filesystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "osada_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <commons/string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

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


//----------------------------------------------------------------------//

char* abrir_archivo(char* path){
	int fd = open ( "/home/utnso/workspace/prueba", O_RDWR);
	int size = getpagesize (); //consigo tamaño del archivo
	char* datos = mmap((caddr_t) 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	return datos;
	//munmap(fd);
}

char* texto_entre(int desde, int hasta, char* texto){
	char* texto_cortado = string_substring_until(texto, hasta);
	return string_substring_from(texto_cortado, desde);
}

int numero_entre(int desde, int hasta, char* texto){
	char* texto_cortado = texto_entre(desde, hasta, texto);
	int numero = atoi(texto_cortado);
	return numero;
}

osada_header* header_create(char* path){
	char* archivo = abrir_archivo(path);
	osada_header* header  = malloc(sizeof(osada_header));
	header->magic_number[7]= texto_entre(0, 7, archivo);
	printf("ejemplo 1: %s \n", texto_entre(0, 7, archivo));
	printf("ejemplo 2: %s \n", header->magic_number);
	header->version = numero_entre(8, 9, archivo);
	printf("ejemplo 3: %d \n", numero_entre(8, 9, archivo));
	printf("ejemplo 4: %d \n", header->version);

//	header->fs_blocks;
//	header->bitmap_blocks;
//	header->allocations_table_offset;
//	header->data_blocks;
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

	//int numero = numero_entre(8, blocksize, bloque);
	//printf("%d", numero);

	//char* bloque = abrir_archivo("/home/utnso/workspace/prueba");
	//char* texto = texto_entre(0, 7, bloque);
	//printf("ejemplo 1: %s \n", bloque);

	osada_header* header = header_create("/home/utnso/workspace/prueba");
	//printf("magic_number= %s \n", header->magic_number);
	//printf("version= %d \n", header->version);

	//int fd = open ( "/home/utnso/workspace/prueba", O_RDWR);
	//int size = getpagesize (); //consigo tamaño del archivo
	//char* datos = mmap((caddr_t) 0, size, PROT_READ, MAP_SHARED, fd, 0);
	//printf("%s", bloque);


	return EXIT_SUCCESS;
}


