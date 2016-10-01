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
#include <commons/bitarray.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


#define blocksize 64
#define path "/home/utnso/workspace/prueba"
#define archivo_osada abrir_archivo()
#define header header_create()
#define bitmap bitmap_create()
#define tabla_de_asignaciones tabla_de_asignaciones_create()
#define bloques_osada_file 1024
#define file_tamanio 32
//osada_header* header = header_create("/home/utnso/workspace/prueba");
//double blocksize = 64;

//-----------------FUNCIONES TRONCALES-----------------------//


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


char* abrir_archivo(){
	int fd = open (path, O_RDWR);
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

void generar_magic_number(char * texto_devuelto_por_funcion)
{
	int cantidad_palabras = string_length(texto_devuelto_por_funcion);
	int i = 0;

	while(i < cantidad_palabras)
	{
		//header->magic_number[i] = texto_devuelto_por_funcion[i];
		i++;
	}
}


//------------------Header-----------------//

osada_header* header_create(){
	osada_header* header_osada  = malloc(sizeof(osada_header));
	memcpy(header_osada,archivo_osada,blocksize);
	return(header_osada);
}



//-----------------------BITMAP------------------------//


t_bitmap* bitmap_create(){
	t_bitmap* bitmap_osada  = malloc(sizeof(t_bitmap));
	bitmap_osada->bloque_inicial = 1;
	bitmap_osada->bloque_final = header->bitmap_blocks +1;
	bitmap_osada->size = header->bitmap_blocks;
	int tamanio_en_bytes = header->bitmap_blocks * blocksize;
	memccpy (bitmap_osada->datos, archivo_osada, blocksize + 1, tamanio_en_bytes);
	return bitmap_osada;
}

//t_bitmap* bitmap_create()
//{
//	t_bitmap *bitmap_new = malloc(sizeof(t_bitmap));
//	bitmap_new->bloque_inicial=1;
//	bitmap_new->bloque_final = header->bitmap_blocks +1;
//	bitmap_new->size = header->bitmap_blocks;
//	int byte_final_bloque = bitmap_new->bloque_final *blocksize;
//	bitmap_new->datos = bitarray_create(texto_entre(blocksize, byte_final_bloque, path), bitmap_new->size );
//	return bitmap_new;
//}

int primer_bloque_libre()
{
	size_t tamanio_del_bitArray = bitarray_get_max_bit(bitmap->datos);
	off_t i = 0;
	int encontre_byte_free = 0;
	do
	{
		encontre_byte_free = bitarray_test_bit(bitmap->datos,i);
		i++;
	}
	while(!encontre_byte_free);

	return (i-1);

}




//int primer_bloque_libre(){
//	int posicion = 0;
//	while( (bitmap->datos)[posicion] != 0){
//		posicion++;
//	  }
//		return posicion;
//}


//void cambiar_estado_bloque(int posicion){
//	if ((bitmap->datos)[posicion] == 0){
//		(bitmap->datos)[posicion] = 1;
//	}
//	else {
//		(bitmap->datos)[posicion] = 0;
//	}
//}

//-------------Tabla de Archivos--------------------------//

//SIN TESTEAR
osada_file* osada_file_create(int posicion){
	osada_file* osada_file  = malloc(sizeof(osada_file));
	float cantidad_de_posiciones = redondear_si_es_necesario((bloques_osada_file * blocksize),  file_tamanio);
	if(posicion>cantidad_de_posiciones){
		printf("ERROR: posicion inexistente");
	}
	else{
		int posicion_file = blocksize + (header->bitmap_blocks * blocksize) + (posicion * blocksize);
		memccpy (osada_file, archivo_osada, posicion_file, file_tamanio);
		return osada_file;
	}
}



//-----------------Tabla de Asignaciones--------------------//

int bloques_tabla_de_asignacion(){
	int bitmap_tamanio = header->bitmap_blocks;
	int archivo_tamanio = getpagesize ();
	int resultado = (archivo_tamanio - bitmap_tamanio - 1 - bloques_osada_file) * 4;
	return redondear_si_es_necesario (resultado , blocksize);
}


t_asignaciones* tabla_de_asignaciones_create(){
	t_asignaciones* tabla = malloc(sizeof(t_asignaciones));
	tabla->bloque_inicial = header->allocations_table_offset;
	tabla->size = bloques_tabla_de_asignacion();
	tabla->bloque_final = tabla->size + tabla->bloque_inicial;
	memccpy (tabla->datos, archivo_osada, tabla->bloque_inicial * blocksize, tabla->size * blocksize);
	return tabla;
}


//int proximo_bloque(int posicion_actual){
//	int tamanio = tabla_de_asignaciones->size;
// 	int datos[tamanio] = tabla_de_asignaciones->datos;
//	return datos[posicion_actual];
//}


//-------------------Bloque de Datos-----------------------//

t_bloques_de_datos* bloque_de_datos_create(){
	t_bloques_de_datos* datos = malloc(sizeof(t_bloques_de_datos));
	datos->size = header->data_blocks;
	datos->bloque_inicial = header->magic_number + header->bitmap_blocks + header->fs_blocks + header->allocations_table_offset;
	datos->bloque_final = datos->bloque_inicial + datos->size;
	memccpy (datos->datos, archivo_osada, datos->bloque_inicial * blocksize, datos->size * blocksize);
	return datos;
}


int main(int argc, char** argv){



	//printf("magic_number: %s \n", header->magic_number);
	//printf("version: %d \n", header->version);
	//printf("fs blocks: %d \n", header->fs_blocks);
	//printf("bitmap_blocks: %d \n", header->bitmap_blocks);
	//printf("llocations_table_offset: %d \n", header->allocations_table_offset);
	//printf("data_blocks: %d \n", header->data_blocks);


	printf("bitmap inicio: %d \n", bitmap->bloque_inicial);
	printf("bitmap tamanio: %d \n", bitmap->size);

	int tamanio_en_bytes = header->bitmap_blocks * blocksize;
	int prueba;
	memccpy (prueba, archivo_osada, blocksize + 1, tamanio_en_bytes);
	printf("bitmap datos: %s \n", prueba);
	printf("bitmap datos: %d \n", prueba);

	return EXIT_SUCCESS;
}


