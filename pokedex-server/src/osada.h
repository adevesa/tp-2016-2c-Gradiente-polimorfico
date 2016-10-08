#ifndef __OSADA_H__
#define __OSADA_H__

#define OSADA_BLOCK_SIZE 64
#define OSADA_FILENAME_LENGTH 17

#include <stdint.h>
#include "so-commons/bitarray.h"
#include "so-commons/string.h"
#include "so-commons/collections/list.h"
#include "math.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "global-vars.h"

#define LSB_FIRST 5
#define FEOF -1


enum
{
	EXITO = 1,
	NO_EXISTE = 2,
	EXISTE = 3,
	NO_HAY_ESPACIO = 4,
	ARGUMENTO_INVALIDO = 5
}t_results;

enum
{
	HEADER = 1,
	BITMAP,
	TABLA_DE_ARCHIVOS = 1024,
	TABLA_DE_ASIGNACIONES,
	BLOQUE_DE_DATOS
}t_bloque;


typedef unsigned char osada_block[OSADA_BLOCK_SIZE];
typedef uint32_t osada_block_pointer;

//#pragma pack(push, 1)

typedef struct {
	unsigned char magic_number[7]; // OSADAFS
	uint8_t version;
	uint32_t fs_blocks; // total amount of blocks
	uint32_t bitmap_blocks; // bitmap size in blocks
	uint32_t allocations_table_offset; // allocations table's first block number
	uint32_t data_blocks; // amount of data blocks
	unsigned char padding[40]; // useless bytes just to complete the block size
} osada_header;


typedef enum __attribute__((packed)) {
    DELETED = '\0',
    REGULAR = '\1',
    DIRECTORY = '\2',
} osada_file_state;

typedef struct {
	osada_file_state state;
	unsigned char fname[OSADA_FILENAME_LENGTH];
	uint16_t parent_directory;
	uint32_t file_size;
	uint32_t lastmod;
	osada_block_pointer first_block;
} osada_file;

typedef struct
{
	char *ruta;
	int file_descriptor;
	int size;
	void *map;
	osada_header *header;
	t_bitarray *bitmap;
}t_disco_osada;

t_disco_osada* disco; // <-- ES LA VARIABLE GLOBAL DEL DISCO

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
 * @NAME: void* osada_get_block_relative(int campo, int num_block, t_disco_osada *disco)
 * @DESC: dado un campo (HEADER, BITMAP, TABLA_DE_ARCHIVOS, TABLA_DE_ASIGNACIONES o BLOQUE_DE_DATOS) y un num de bloque relativo
 * 			dentro de DICHO campo, además de recibir un struct de tipo "t_disco_osada", devuelve el BLOQUE COMPLETO.
 *
 * WARNING: AL MODIFICAR ALGUN BYTE DEL BLOQUE QUE FUE DEVUELTO, LOS CAMBIOS NO SE IMPACTARÁN HASTA QUE SE LLAME A LA FUNCIÓN
 * 			"OSADA_PUSH_BLOCK()" CON SUS RESPECTIVOS PARAMETROS.
 */
void* osada_get_blocks_relative_since(int campo, int num_block_init, int num_blocks,t_disco_osada *disco);
void* osada_get_block_start_in(int byte_inicial, int num_blocks, void *map); // <--- FUNCION QUE ES USADA DENTRO DE "OSADA_GET_BLOCK_RELATIVE"
void* osada_get_bytes_start_in(int byte_inicial, int num_bytes_total, void *map);

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
void osada_push_part_of_block(int campo, int numero_block_relative, int offset, void *bloque, t_disco_osada *disco);
void impactar_en_disco_bloque_completo(int byte_inicial,void *bloque, void *map); //<---  FUNCION QUE ES USADA POR "OSADA_PUSH_BLOCK"
void impactar_en_disco_medio_bloque(int byte_inicial,void *bytes, void *map);
void impactar_en_disco_n_bloques(int byte_inicial, int cantidad_bloques,void *bloques, void *map);

/*----------------------------------------------OBTENCION DE NUM BLOQUE ARCHIVO-----------------------------------------*/
t_list* osada_get_blocks_nums_of_this_file(osada_file *file, t_disco_osada *disco);

/*----------------------------------------------OBTENCION DE DATOS DE UN ARCHIVO---------------------------------------*/
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
int calcular_byte_final_a_recuperar_de_file(int file_size);
void nada (void *nada);

/*---------------------------------------------BUSUQEDA DE TABLA DE ARCHIVOS DISPONIBLE---------------------------------*/
typedef struct
{
	osada_file *file;
	int block_relative;
	int position_in_block;
}t_osada_file_free;

t_osada_file_free* osada_file_table_get_space_free(t_disco_osada *disco);
int verificar_si_file_esta_libre(osada_file *file);

/*----------------------------------------------OBTENCION DE UN ARCHIVO ESPECIFICO---------------------------------------*/
typedef t_osada_file_free t_file_osada;

void* osada_get_file_called(char *file_name, t_disco_osada *disco);
int verificar_si_es_archivo_buscado(char *file_name, osada_file *file);

/*----------------------------------------------MANIPULACION BITARRAY-------------------------------------------------*/
int osada_ocupa_bit_libre_de(t_disco_osada *disco);
void osada_desocupa_bit(t_disco_osada *disco, int num_block);

osada_block_pointer osada_get_start_block_absolut_of(int campo, t_disco_osada *disco);

/*---------------------------------------------VERIFICACION EXISTENCIA DE UN PATH-----------------------------------------*/
int osada_check_exist(char *path);
int verificar_existencia(char *file_or_directory, uint16_t dad_block);
int revisar_resultado(int result);
int calcular_posicion_en_tabla_de_archivos(int num_block, int position);

/*---------------------------------------------BORRADO DE ARCHIVOS---------------------------------------------------------*/
void osada_delete_this_file(char *path);
void delete_file(char *archivo);
int calcular_desplazamiento_tabla_de_archivos(int posicion_relativa);

void osada_change_file_state(osada_file *file, osada_file_state new_state);

int calcular_cantidad_bloques_admin();
/*---------------------------------------------AUXILIARES----------------------------------------------------------------*/
int array_size(char **array);
void array_free_all(char **array);

//#pragma pack(pop)

#endif __OSADA_H__
