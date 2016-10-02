/*
 * pokedex_servidor.h
 *
 *  Created on: 28/9/2016
 *      Author: utnso
 */

#ifndef POKEDEX_SERVIDOR_H_
#define POKEDEX_SERVIDOR_H_
#include "osada_struct.h"
//-----------------FUNCIONES TRONCALES-----------------------//

//Divide dos numeros, si el resto es distinto de 0 lo redondea hacia arriba.
int redondear_si_es_necesario(double x, double y);

//Def:abre el archivo que esta en la variable globar path
//Resultado: la cadena de caracteres de todo el archivo
char* abrir_archivo();


//devuelve el texto que esta entre dos posiciones
char* texto_entre(int desde, int hasta, char* texto){

//devuelve los numeros que esten en un string entre dos posiciones
int numero_entre(int desde, int hasta, char* texto){

//Def: dado una cantidad de bloques y el bloque inicial se fija en el archivo_osada y
//copia sus datos en un t_bitarray*
t_bitarray* guardar_en_datos_bitarray(int cantidad_de_bloques, int bloque_inicial);


int guardar_en_datos_enteros(int cantidad_de_bloques, int bloque_inicial);




//------------------Header-----------------//
//genera un header del archivo alojado en la variable global archivo_osada
osada_header* header_create();



//-----------------------BITMAP------------------------//

//Def: dado un header genera un bitmap del archivo alojado en la variable global archivo_osada
t_bitmap* bitmap_create(osada_header* header);

//Def: busca el primer bloque libre de un bitmap
//Resultado: un int con la posicion del bloque, o -1 si no hay ninguno disponible
int primer_bloque_libre(t_bitmap* bitmap);

//Def: dado una posicion y un bitmap cambio su valor,
//si esta en 1 pasa a 0
//si esta en 0 pasa a 1 (revisar esta parte)
void cambiar_estado_bloque(off_t posicion, t_bitmap* bitmap);

//-------------Tabla de Archivos--------------------------//

//----SIN TESTEAR, REVISAR!!!!!!

//Def: dado un header y una posicion, genera el osada_file del archivo alojado en esa posicion
osada_file* osada_file_create(int posicion, osada_header* header);

//Def: dado un header genera una lista de osada_file* con todos los archivos del archivo_osada
t_list* generar_tablas_de_archivos(osada_header* header);

//Def; dada una lista de osada_file* devuelve otra lista con los nombre de esos archivos
t_list* nombre_de_archivos(t_list* tablas_de_archivos);



//-----------------Tabla de Asignaciones--------------------//

// Def: calcula la cantidad de bloques de la tabla de asignaciones
int bloques_tabla_de_asignacion(osada_header* header);

//Def: dado un header genera una tabla de asignaciones del archivo alojado en la variable global archivo_osada
t_asignaciones* tabla_de_asignaciones_create(osada_header* header);

//Def: devuelve la posicion del proximo bloque de la tabla de asignaciones
int proximo_bloque(int posicion_actual, t_asignaciones* tabla);

//-------------------Bloque de Datos-----------------------//

//Def: dado un header genera los bloques de datos del archivo alojado en la variable global archivo_osada
t_bloques_de_datos* bloques_de_datos_create(osada_header* header);


//------------------------------------------//

//Def: genera un t_osada*
t_osada* ejecutar_osada();





#endif /* POKEDEX_SERVIDOR_H_ */
