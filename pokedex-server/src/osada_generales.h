/*
 * osada_generales.h
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */

#ifndef OSADA_GENERALES_H_
#include "osada.h"
#include "file_manipuling.h"
#include "borrados.h"
/*-------------------------------------------------------CREATES Y RECUPEROS-----------------------------------------------*/

/*
 * @NAME:t_disco_osada* osada_disco_abrite(char *ruta)
 * @DESC: dada una ruta que lleva a un DISCO formateado con filesystem OSADA, devuelve una estructura de tipo
 * 		"T_DISCO_OSADA" completamente inicializado y listo para trabajar (LEER Y ESCRIBIR EN ÉL).
 */
t_disco_osada* osada_disco_abrite(char *ruta);

/*
 * @NAME: osada_header* osada_header_create(void *map)
 * @DESC: dado un puntero al mapping del disco, devuelve un puntero a una estructura tipo "osada_header"
 * 		que contiene TODOS los datos del HEADER del disco al que mappea "map"
 */
osada_header* osada_header_create(void *map);


/*
 * @NAME: t_bitarray* osada_bitmap_create(t_disco_osada *disco)
 * @DESC: dado un "t_disco_osada", crea y devuelve un puntero a una estructura "t_bitarray". La misma se encuentra
 * 		LISTA para poder ser manipulada.
 *
 * 	WARNING: ANTE CUALQUIER CAMBIO QUE SE REALICE A ESTA ESTRUCTURA QUE ES DEVUELTA, SE DEBERÁ LLAMAR A LA FUNCIÓN
 * 			"OSADA_PUSH_BLOCK()" CON SUS RESPECTIVOS PARAMETROS.
 */
t_bitarray* osada_bitmap_create(t_disco_osada *disco);


/*
 * @NAME: int disco_dame_tu_descriptor(char *ruta)
 * @DESC: dada una ruta que lleva a un DISCO formateado con filesystem OSADA, devuelve el descriptor de dicho disco.
 */
int disco_dame_tu_descriptor(char *ruta);

/*
 * @NAME: int disco_dame_tu_tamanio(char *ruta)
 * @DESC: dada una ruta que lleva a un DISCO formateado con filesystem OSADA, devuelve el TAMAÑO de dicho disco (en BYTES)
 */
int disco_dame_tu_tamanio(char *ruta);

/*
 * @NAME: void* disco_dame_mapping(int size, int file_descriptor)
 * @DESC: dado el TAMAÑO del disco y su descriptor, CREA y DEVUELVE el mapping con el que se trabajará para LEER
 * 			e IMPACTAR los cambios en dicho disco.
 */
void* disco_dame_mapping(int size, int file_descriptor);


/*----------------------------------------------OBTENCION DE BLOQUES---------------------------------------------------------*/

/*
 * @NAME: void* osada_get_blocks_relative_since(int campo, int num_block_init, int num_blocks,t_disco_osada *disco);
 * @DESC: dado un campo (HEADER, BITMAP, TABLA_DE_ARCHIVOS, TABLA_DE_ASIGNACIONES o BLOQUE_DE_DATOS), un num de bloque relativo dentro de DICHO campo
 * 			y la cantidad de bloques COMPLETOS que queremos recuperar, además de recibir un struct de tipo "t_disco_osada",
 * 			devuelve los N bloques completos.
 *
 * WARNING: AL MODIFICAR ALGUN BYTE DEL BLOQUE QUE FUE DEVUELTO, LOS CAMBIOS NO SE IMPACTARÁN HASTA QUE SE LLAME A ALGUNA DE LAS
 * 			 FUNCIONES "OSADA_PUSH_BLOCK()" CON SUS RESPECTIVOS PARAMETROS.
 */
void* osada_get_blocks_relative_since(int campo, int num_block_init, int num_blocks,t_disco_osada *disco);
void* osada_get_block_start_in(int byte_inicial, int num_blocks, void *map); // <--- FUNCION QUE ES USADA DENTRO DE "OSADA_GET_BLOCK_RELATIVE"

/*
 *  @NAME:void* osada_get_bytes_start_in(int byte_inicial, int num_bytes_total, void *map)
 *  @DESC: dado un número de BYTE inicial ABSOLUTO, el número de bytes TOTAL que queremos recuperar, y un puntero al map del disco,
 *  devuelve la cantidad de bytes pedidos comenzando en Byte inicial.
 */
void* osada_get_bytes_start_in(int byte_inicial, int num_bytes_total, void *map);

/*
 * @NAME:osada_get_start_block_absolut_of(int campo, t_disco_osada *disco)
 * @DESC: dado un CAMPO, y un disco OSADA, devuelve el PRIMER bloque ABSOLUTO de dicho campo en el disco.
 */
osada_block_pointer osada_get_start_block_absolut_of(int campo, t_disco_osada *disco);


int calcular_tamanio_tabla_de_asignaciones(osada_header *header); // <--- FUNCION QUE ES USADA DENTRO DE "OSADA_GET_BLOCK_RELATIVE"


/*
 *  @NAME: osada_block_pointer calcular_byte_inicial_relative(int campo, int numero_bloque,osada_header *header)
 *  @DESC: dado un campo (HEADER, BITMAP, TABLA_DE_ARCHIVOS, TABLA_DE_ASIGNACIONES o BLOQUE_DE_DATOS), un num de bloque relativo
 *  		dentro de DICHO campo, y el HEADER del disco, devuelve el PRIMER byte del bloque en cuestión.
 */
osada_block_pointer calcular_byte_inicial_relative(int campo, int numero_bloque,osada_header *header);

/*
 *  @NAME: osada_block_pointer calcular_byte_inicial_absolut(int numero_bloque_absoluto)
 *  @DESC: dado un numero de bloque ABSOLUTO (se considera que el PRIMER bloque es el 1) en el disco, devuelve el número del PRIMER
 *  		byte de dicho bloque.
 */
osada_block_pointer calcular_byte_inicial_absolut(int numero_bloque_absoluto);




/*----------------------------------------------IMPACTAR CAMBIOS EN UN BLOQUE-------------------------------------------------*/

/*
 * @NAME: void osada_push_block(int campo, int numero_block_relative, void *bloque,t_disco_osada *disco)
 * @DESC: dado un campo (HEADER, BITMAP, TABLA_DE_ARCHIVOS, TABLA_DE_ASIGNACIONES o BLOQUE_DE_DATOS), un num de bloque RELATIVO dentro
 * 			de dicho campo, un BLOQUE (necesita ser CASTEADO como void* al llamar a esta función), y un "t_disco_osada",
 * 			IMPACTA los cambios directamente en el disco.
 *
 * 	WARNING: CONSIDERE QUE PARA VOLVER A REALIZAR CAMBIOS SOBRE EL MISMO BLOQUE UNA VEZ QUE YA FUERON IMPACTADOS LOS CAMBIOS,
 * 			SE DEBERÁ VOLVER A LLAMAR A LA FUNCIÓN "OSADA_GET_BLOCK_RELATIVE" CON SUS RESPECTIVOS PARÁMETROS Y VOLVER A TRABAJAR
 * 			SOBRE EL BLOQUE.
 */
void osada_push_block(int campo, int numero_block_relative, void *bloque,t_disco_osada *disco);
void impactar_en_disco_bloque_completo(int byte_inicial,void *bloque, void *map); //<---  FUNCION QUE ES USADA POR "OSADA_PUSH_BLOCK"

/*
 * @NAME:osada_push_middle_block(int campo, int numero_block_relative, int offset, void *bloque, t_disco_osada *disco);
 * @DESC: Idem a la OSADA_PUSH_BLOCK, solamente que recibe un desplazamiento (OFFSET) e impacta UNICAMENTE MEDIO bloque en disco.
 *
 * 	*NOTA: LEASE MISMO WARNING QUE OSADA_PUSH_BLOCK
 */
void osada_push_middle_block(int campo, int numero_block_relative, int offset, void *bloque, t_disco_osada *disco);

void impactar_en_disco_medio_bloque(int byte_inicial,void *bytes, void *map);
void impactar_en_disco_n_bloques(int byte_inicial, int cantidad_bloques,void *bloques, void *map);


/*----------------------------------------------OBTENCION DE NUM BLOQUE ARCHIVO-----------------------------------------*/
/*
 * @NAME: t_list* osada_get_blocks_nums_of_this_file(osada_file *file, t_disco_osada *disco)
 * @DESC: Dado un archivo osada,y un disco, devuelve TODOS los bloques que lo componen, revisando la TABLA DE ASIGNACIONES.
 *
 * WARNING: DEVUELVE UNA LISTA, POR LO QUE LUEGO DE USARLA, LA MISMA DEBERÁ SER LIBERADA.
 */
t_list* osada_get_blocks_nums_of_this_file(osada_file *file, t_disco_osada *disco);

/*----------------------------------------------MANIPULACION BITARRAY----------------------------------------------------*/
/*
 * @NAME:int osada_ocupa_bit_libre_de(t_disco_osada *disco);
 * @DESC: Dado un disco osada, OCUPA un bit libre del bitarray (el primero que encuentre) y lo devuelve.
 *
 * WARNING: AL LLAMAR ESTA FUNCIÓN, LOS CAMBIOS NO SON IMPACTADOS DIRECTAMENTE EN DISCO.
 *			LA FUNCIÓN ESTA PENSADA PARA MANIPULAR UN CONJUNTO DE BITS Y LUEGO IMPACTAR LOS CAMBIOS.
 *
 *			UNA POSIBLE FORMA DE IMPACTAR EL BITARRAY COMPLETO ES LLAMAR A LA SIGUIENTE SENTENCIA:
 *			impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);
 *
 *	NOTA: RECUERDE QUE CADA POSICIÓN DEL BITARRAY REPRESENTA UN BLOQUE DEL DISCO. ADEMÁS RECUERDE QUE LAS ESTRUCTURAS ADMINISTRATIVAS
 *		(BLOQUES DE: HEADER, BITMAP, TABLA DE ARCHIVOS, Y TABLA DE ASIGNACIONES) SIEMPRE ESTÁN MARCADAS COMO OCUPADAS.
 *		ṔOR LO QUE ESTA FUNCION NOS DEVUELVE UN NUMERO DE BLOQUE ABSOLUTO PERTENECIENTE AL BLOQUE DE DATOS.
 *		RECUERDE LA CORRESPONDENCIA ENTRE EL NUMERO RELATIVO DE BLOQUE EN EL CAMPO BLOQUE DE DATOS CON LA TABLA DE ASIGNACIONES.
 */
int osada_ocupa_bit_libre_de(t_disco_osada *disco);

/*
 * @NAME:int osada_b_check_is_bitarray_full(t_disco_osada *disco);
 * @DESC: Verifa si el disco está completo o todavia hay algún espacio libre. Devuelve:
 * 		- 0 --> HAY ESPACIO LIBRE
 * 		- 1 --> NO HAY MÁS ESPACIO LIBRE
 */
int osada_b_check_is_bitarray_full(t_disco_osada *disco);
int calcular_posicion_relativa_en_bloque_de_datos(int posicion_absoluta);

/*
 * @NAME:osada_desocupa_bit(t_disco_osada *disco, int num_block);
 * @DESC:Dado un disco osada, y un número de bloque ABSOLUTO perteneciente al BLOQUE DE DATOS, lo libera.
 *
 * WARNING: LOS CAMBIOS NO SON IMPACTADOS DIRECTAMENTE.
 *
 * NOTA: PARA SABER COMO CALCULAR EL NUM DE BLOQUE ABSOLUTO DEL CAMPO BLOQUE DE DATOS DADO UN NUM DE BLOQUE RELATIVO,
 * 		REVISE LA IMPLEMENTACION DE LA FUNCION "OSADA_DESOCUPA_N_BITS"
 */
void osada_desocupa_bit(t_disco_osada *disco, int num_block);


/*
 * @NAME:osada_desocupa_n_bits(t_list *bloques_a_liberar);
 * @DESC:Dado un bloque que contiene los números de bloques RELATIVOS pertenencientes al campo BLOQUE DE DATOS, libera
 * 		cada uno de ellos.
 *
 * 	WARNING: LOS CAMBIOS SON IMPACTADOS DIRECTAMENTE EN EL DISCO LUEGO DE TERMINAR CON LA EJECUCIÓN DE LA FUNCIÓN.
 */
void osada_desocupa_n_bits(t_list *bloques_a_liberar);



/*---------------------------------------------AUXILIARES----------------------------------------------------------------*/
int array_size(char **array); //<-- Se entiende solo con leer el nombre
void array_free_all(char **array);//<-- Se entiende solo con leer el nombre
char* array_last_element(char* path);
char* crear_ruta(char* hijo, char* path_padre);

/*---------------------------------------------DESTROYERS----------------------------------------------------------------*/
void t_file_osada_destroy(t_file_osada *file);
void file_listado_eliminate(t_file_listado* file);
#endif /* OSADA_GENERALES_H_ */
