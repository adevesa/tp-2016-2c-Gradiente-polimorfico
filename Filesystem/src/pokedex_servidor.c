/*
 ============================================================================
 Name        : Filesystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//#include "pokedex_servidor.h" ME TIRA ERROR AL DESMARCAR ESTO :S
#include "osada_struct.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>




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

t_bitarray* guardar_en_datos_bitarray(int cantidad_de_bloques, int bloque_inicial){
	int tamanio_en_bytes = cantidad_de_bloques * blocksize;
	void* resultado = malloc(tamanio_en_bytes);
	memcpy(resultado, archivo_osada + ( bloque_inicial * blocksize), cantidad_de_bloques *blocksize);
	return bitarray_create(resultado,tamanio_en_bytes);
}

int guardar_en_datos_enteros(int cantidad_de_bloques, int bloque_inicial){
	int tamanio_en_bytes = cantidad_de_bloques * blocksize;
	t_bitarray* bitarray = guardar_en_datos_bitarray(cantidad_de_bloques, bloque_inicial);
	int i;
	int bits;
	int bits_totales =  tamanio_en_bytes * 8;
	for(i=0;i<=bits_totales;i++){
	bits = bitarray_test_bit(bitarray, i);
	//printf("%d", bits);
	}
	return bits;
}




//------------------Header-----------------//

osada_header* header_create(){
	osada_header* header_osada  = malloc(sizeof(osada_header));
	memcpy(header_osada,archivo_osada,blocksize);
	return(header_osada);
}



//-----------------------BITMAP------------------------//


t_bitmap* bitmap_create(osada_header* header){
	t_bitmap* bitmap_osada  = malloc(sizeof(t_bitmap));
	bitmap_osada->bloque_inicial = 1;
	bitmap_osada->bloque_final = header->bitmap_blocks +1;
	bitmap_osada->size = header->bitmap_blocks;
	bitmap_osada->datos = guardar_en_datos_bitarray(bitmap_osada->size, bitmap_osada->bloque_inicial);
	return bitmap_osada;
}

int primer_bloque_libre(t_bitmap* bitmap){
	size_t tamanio_del_bitArray = bitarray_get_max_bit(bitmap->datos);
	off_t i = 0;
	while( bitarray_test_bit(bitmap->datos, i) != 0){
		i++;
	}
	if(1024 > i){
		return i;
	}
	else{
		printf("No hay espacio disponible\n");
		return -1;
	}
}

void cambiar_estado_bloque(off_t posicion, t_bitmap* bitmap){
	int bit = bitarray_test_bit(bitmap->datos, posicion);
	if ( bit == 0){
	//	 bitarray_set_bit(bitmap->datos, posicion) = 1;
	}
	else {
		 bitarray_clean_bit(bitmap->datos, posicion);
	}
}

//-------------Tabla de Archivos--------------------------//

//----SIN TESTEAR, REVISAR!!!!!!
osada_file* osada_file_create(int posicion, osada_header* header){
	osada_file* osada_file  = malloc(sizeof(osada_file));
	int cantidad_de_posiciones = redondear_si_es_necesario((bloques_osada_file * blocksize),  file_tamanio);
	if(posicion>cantidad_de_posiciones){
		printf("ERROR: posicion inexistente");
	}
	else{
		int posicion_file = blocksize + (header->bitmap_blocks * blocksize) + (posicion * blocksize);
		memcpy(osada_file, archivo_osada + posicion_file, 32);
		return osada_file;
	}
}

t_list* generar_tablas_de_archivos(osada_header* header){
	t_list* lista = list_create();
	int i = 0;
	for(i=0;i<2048;i++){
	list_add(lista, osada_file_create(i, header));
	}
	return lista;
}

t_list* nombre_de_archivos(t_list* tablas_de_archivos){
	int tamanio = list_size(tablas_de_archivos);
	t_list* nombres = list_create();
	int i;
	for(i=0; tamanio>=i; i++){
		osada_file* tabla= list_get(tablas_de_archivos, i);
		if(tabla->state != '0'){
		list_add(nombres, tabla->fname);
		}
	}
	return nombres;
}



//-----------------Tabla de Asignaciones--------------------//

int bloques_tabla_de_asignacion(osada_header* header){
	int bitmap_tamanio = header->bitmap_blocks;
	int archivo_tamanio = getpagesize ();
	int resultado = (archivo_tamanio - bitmap_tamanio - 1 - bloques_osada_file) * 4;
	return redondear_si_es_necesario (resultado , blocksize);
}


t_asignaciones* tabla_de_asignaciones_create(osada_header* header){
	t_asignaciones* tabla = malloc(sizeof(t_asignaciones));
	tabla->bloque_inicial = header->allocations_table_offset;
	tabla->size = bloques_tabla_de_asignacion(header);
	tabla->bloque_final = tabla->size + tabla->bloque_inicial;
	tabla->datos = guardar_en_datos_bitarray(tabla->size, tabla->bloque_inicial);
	return tabla;
}


int proximo_bloque(int posicion_actual, t_asignaciones* tabla){
	int dato = bitarray_test_bit(tabla->datos,  posicion_actual);
	return bitarray_test_bit(tabla->datos,  dato);
}


//-------------------Bloque de Datos-----------------------//

t_bloques_de_datos* bloques_de_datos_create(osada_header* header){
	t_bloques_de_datos* datos = malloc(sizeof(t_bloques_de_datos));
	datos->size = header->data_blocks;
	datos->bloque_inicial = 1 + header->bitmap_blocks + header->fs_blocks + header->allocations_table_offset;
	datos->bloque_final = datos->bloque_inicial + datos->size;
	datos->datos = guardar_en_datos_bitarray(datos->size, datos->bloque_inicial);
	return datos;
}



//------------------------------------------//


t_osada* ejecutar_osada(){
	t_osada* osada = malloc(sizeof(t_osada));
	osada->header = header_create();
	osada->bitmap = bitmap_create(osada->header);
	//osada->file_list= generar_tablas_de_archivos(header);
	osada->tabla_de_asignaciones = tabla_de_asignaciones_create(osada->header);
	osada->bloques_de_datos = bloques_de_datos_create(osada->header);
	return osada;
}



int main(int argc, char** argv){

	t_osada* osada = ejecutar_osada();
	//printf("magic_number: %s \n", osada->header->magic_number);
	//printf("bitmap tamanio: %d \n", osada->bitmap->size);

	//HEADER
	osada_header* header = header_create();
	//printf("magic_number: %s \n", osada->magic_number);
	//printf("version: %d \n", header->version);
	//printf("fs blocks: %d \n", header->fs_blocks);
	//printf("bitmap_blocks: %d \n", header->bitmap_blocks);
	//printf("llocations_table_offset: %d \n", header->allocations_table_offset);
	//printf("data_blocks: %d \n", header->data_blocks);

	//BITMAP
	t_bitmap* bitmap = bitmap_create(header);
	//printf("bitmap inicio: %d \n", bitmap->bloque_inicial);
	//printf("bitmap final: %d \n", bitmap->bloque_final);
	//printf("bitmap tamanio: %d \n", bitmap->size);
	//int i;
	//for(i=0;i<= bitarray_get_max_bit(bitmap->datos);i++){
	//printf("%d", bitarray_test_bit(bitmap->datos,i));
	//}

	//TABLA DE ARCHIVOS
	//t_list* archivos = generar_tablas_de_archivos(header);
	//osada_file* file = osada_file_create(2, header);
	//printf("size: %d \n", file->file_size);


	//TABLA DE ASIGNACIONES
	t_asignaciones* tabla_de_asignaciones = tabla_de_asignaciones_create(header);
	//printf("tda inicio: %d \n", tabla_de_asignaciones->bloque_inicial);
	//printf("tda final: %d \n", tabla_de_asignaciones->bloque_final);
	//printf("tda tamanio: %d \n", tabla_de_asignaciones->size);
	//int i;
	//for(i=0;i<= bitarray_get_max_bit(tabla_de_asignaciones->datos);i++){
	//printf("%d", bitarray_test_bit(tabla_de_asignaciones->datos,i));
	//}


	//BLOQUES DE DATOS
	t_bloques_de_datos* bloques_de_datos = bloques_de_datos_create(header);
	//printf("bdd inicio: %d \n", bloques_de_datos->bloque_inicial);
	//printf("bdd final: %d \n", bloques_de_datos->bloque_final);
	//printf("bdd tamanio: %d \n", bloques_de_datos->size);
	//int i;
	//for(i=0;i<= bitarray_get_max_bit(bloques_de_datos->datos);i++){
	//printf("%d", bitarray_test_bit(bloques_de_datos->datos,i));
	//}




	//TESTEO DE DATOS DE LAS STRUCT
	//int i;
	//t_bloques_de_datos* estructura = bloques_de_datos_create(header);
	//printf("bits: %d\n", bitarray_get_max_bit(estructura->datos));
	//printf("bytes: %d\n", bitarray_get_max_bit(estructura->datos)/8);
	//printf("bloques %d\n",bitarray_get_max_bit(estructura->datos)/512);
	//for(i=0;i<= bitarray_get_max_bit(estructura->datos);i++){
	//printf("%d", bitarray_test_bit(estructura->datos,i));
	//}

	//TESTEO DE QUE SE MODIFICAN Y SE GUARDAN LOS DATOS
	//t_bitmap* bitmap = bitmap_create(header);
	//bitarray_clean_bit(bitmap->datos,5);
	//printf("%d", bitarray_test_bit(bitmap->datos,5));

	return EXIT_SUCCESS;

}


