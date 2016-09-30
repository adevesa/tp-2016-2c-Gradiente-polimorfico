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

<<<<<<< HEAD
=======

>>>>>>> 7f564c9175319a0f1d208b672321315b04b31e14
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

<<<<<<< HEAD
=======

>>>>>>> 7f564c9175319a0f1d208b672321315b04b31e14
char* texto_entre(int desde, int hasta, char* texto){
	char* texto_cortado = string_substring_until(texto, hasta);
	return string_substring_from(texto_cortado, desde);
}

int numero_entre(int desde, int hasta, char* texto){
	char* texto_cortado = texto_entre(desde, hasta, texto);
	int numero = atoi(texto_cortado);
	return numero;
}

<<<<<<< HEAD
osada_header* header_create(char* path){
	char* archivo = abrir_archivo(path);
	osada_header* header  = malloc(sizeof(osada_header));
	header->magic_number= texto_entre(0, 6, archivo);
	printf("ejemplo 1: %s \n", texto_entre(0, 7, archivo));
	printf("ejemplo 2: %s \n", header->magic_number);
	header->version = numero_entre(8, 9, archivo);
	printf("ejemplo 3: %d \n", numero_entre(8, 9, archivo));
	printf("ejemplo 4: %d \n", header->version);

//	header->fs_blocks;
//	header->bitmap_blocks;
//	header->allocations_table_offset;
//	header->data_blocks;
=======
void generar_magic_number(osada_header *header,char * texto_devuelto_por_funcion)
{
	int cantidad_palabras = string_length(texto_devuelto_por_funcion);
	int i = 0;

	while(i < cantidad_palabras)
	{
		header->magic_number[i] = texto_devuelto_por_funcion[i];
		i++;
	}
}



osada_header* header_create(char* path){
	char* archivo = abrir_archivo(path);
	osada_header* header  = malloc(sizeof(osada_header));
	char *devuelve = texto_entre(0,7, archivo);
	generar_magic_number(header, devuelve);
	header->version = numero_entre(7, 8, archivo);
	//printf("version: %d \n", header->version);
	header->fs_blocks = numero_entre(9, 13, archivo);
	//printf("fs blocks: %d \n", header->fs_blocks);
	header->bitmap_blocks = numero_entre(13, 17, archivo);
	//printf("bitmap_blocks: %d \n", header->bitmap_blocks);
	header->allocations_table_offset = numero_entre(17, 21, archivo);
	//printf("llocations_table_offset: %d \n", header->allocations_table_offset);
	header->data_blocks = numero_entre(21, 25, archivo);
	//printf("data_blocks: %d \n", header->data_blocks);
>>>>>>> 7f564c9175319a0f1d208b672321315b04b31e14
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

<<<<<<< HEAD
	osada_header* header = header_create("/home/utnso/workspace/prueba");
=======
	//osada_header* header = header_create("/home/utnso/workspace/prueba");
>>>>>>> 7f564c9175319a0f1d208b672321315b04b31e14
	//printf("magic_number= %s \n", header->magic_number);
	//printf("version= %d \n", header->version);

	//int fd = open ( "/home/utnso/workspace/prueba", O_RDWR);
	//int size = getpagesize (); //consigo tamaño del archivo
<<<<<<< HEAD
	//char* datos = mmap((caddr_t) 0, size, PROT_READ, MAP_SHARED, fd, 0);
	//printf("%s", bloque);

=======
	//int datos = mmap((caddr_t) 0, size, PROT_READ, MAP_SHARED, fd, 0);
	//printf("%d", datos);

	//int datos = abrir_archivo("/home/utnso/workspace/prueba");
	//printf("ejemplo 0: %d \n", datos);

	char* archivo = abrir_archivo("/home/utnso/workspace/prueba");
	printf("ejemplo 1: %s \n", archivo);
	printf("ejemplo 2: %d \n", atoi(archivo));


	FILE *archivo2;
		archivo2 = fopen("/home/utnso/workspace/prueba", "r");

				if ( archivo2==NULL ){
						printf("Error al abrir el fichero\n");
				}

					else
					{
						int i = 0;
						printf("ejemplo con fopen: ");
							while( !feof(archivo2) ){
								printf("%d",getc(archivo2));

								}

						}
				fclose(archivo2);
>>>>>>> 7f564c9175319a0f1d208b672321315b04b31e14

	return EXIT_SUCCESS;
}


