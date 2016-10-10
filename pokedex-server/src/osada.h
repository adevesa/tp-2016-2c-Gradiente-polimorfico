#ifndef __OSADA_H__
#define __OSADA_H__

#define OSADA_BLOCK_SIZE 64
#define OSADA_FILENAME_LENGTH 17

#include <stdint.h>
#include "so-commons/bitarray.h"
#include "so-commons/string.h"
#include "so-commons/collections/list.h"
#include "so-commons/string.h"
#include "math.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "global-vars.h"

#define LSB_FIRST 5
#define FEOF -1

/*--------------------------------------------------ENUMS-------------------------------------------------------*/

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

/*------------------------------------------------ OSADA STRUCTS-----------------------------------------------*/
typedef unsigned char osada_block[OSADA_BLOCK_SIZE];
typedef uint32_t osada_block_pointer;

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

/*----------------------------------------------OBTENCION DE DATOS DE UN ARCHIVO---------------------------------------*/

/*
 * @NAME:void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
 * @DESC: Dado un archivo osada,y un disco, devuelve TODA la información completa de dicho archivo
 */
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco);
int calcular_byte_final_a_recuperar_de_file(int file_size); //<-- La usa la función anteriror
void nada (void *nada); //<-- Una estupidez

/*---------------------------------------------BUSQUEDA DE UN ARCHIVO DISPONIBLE Y VACIO---------------------------------*/
/*
 * STRUCT T_OSADA_FILE_FREE
 * @CAMPOS:
 * 		-Un archivo vacío y listo para ser manipulado
 * 		-El número de bloque RELATIVO dentro de la TABLA DE ARCHIVOS (servirá para poder IMPACTARLO en disco)
 * 		-El número de posición en el bloque.
 * 			-position_in_block = 0 si es el primer File dentro del bloque
 * 			-position_in_block = 1 si es el segundo File dentro del bloque
 *
 * 	*NOTA: RECUERDE QUE LA UNA ESTRUCTURA DE TIPO OSADA_FILE PESA 32 BYTES, Y COMO LOS BLOQUES SON DE 64 BYTES,
 * 			EN UN BLOQUE ENTRAN DOS ARCHIVOS. DE AQUÍ LA JUSTIFICACIÓN DEL CAMPO POSITION_IN_BLOCK
 */
typedef struct
{
	osada_file *file;
	int block_relative;
	int position_in_block;
}t_osada_file_free;


/*
 * @NAME:t_osada_file_free*  osada_file_table_get_space_free(t_disco_osada *disco);
 * @DESC: Dado un disco osada, devuelve un puntero a una estructura T_OSADA_FILE_FREE
 *
 */
t_osada_file_free* osada_file_table_get_space_free(t_disco_osada *disco);

/*
 * @NAME:int verify_file_state(int state,osada_file *file);
 * @DESC: Dado un ESTADO posible para el archivo (DELETED, REGULAR, DIRECTORY) y un archivo,
 * 		devuelve 1 si el estado coincide, 0 caso default.
 */
int verify_file_state(int state,osada_file *file);


/*
 * @NAME:void osada_change_file_state(osada_file *file, osada_file_state new_state);
 * @DESC: Dado un ARCHIVO y un estado DISTINTO al actual (DELETED, REGULAR, DIRECTORY), lo setea en el archivo.
 *
 *  WARNING: LOS CAMBIOS NO SON IMPACTADOS. AL FINALIZAR DE MANIPULAR EL ARCHIVO POR COMPLETO, EL PROGRAMADOR DEBERÁ
 *  		LLAMAR A ALGUNA DE LAS FUNCIONES DE IMPACTO EN DISCO
 */
void osada_change_file_state(osada_file *file, osada_file_state new_state);


/*----------------------------------------------OBTENCION DE UN ARCHIVO ESPECIFICO---------------------------------------*/
typedef t_osada_file_free t_file_osada;

/*
 * @NAME:void* osada_get_file_called(char *file_name, t_disco_osada *disco);
 * @DESC: Dado un POSIBLE nombre de archivo,y un disco osada, devuelve "NO_EXISTE" (string) si el nombre de archivo dado
 * 		no se corresponde con ninguno del disco, o el archivo en caso de que lo encuentre.
 *
 * 	WARNING: NOTESE QUE ESTA FUNCION SOLO COMPARA SI EXISTE EL NOMBRE DEL ARCHIVO EN LA TABLA DE ARCHIVOS,
 * 			POR LO QUE NO COMPRUEBA SI EL ARCHIVO ESTÁ BORRADO.
 * 			PARA COMPROBAR ESTO ULTIMO, DEBE USARSE OSADA_CHECH_EXIST (VEASE MÁS ABAJO)
 */
void* osada_get_file_called(char *file_name, t_disco_osada *disco);
int verificar_si_es_archivo_buscado(char *file_name, osada_file *file); //<-- Simple delegación de la función anterior

/*---------------------------------------------VERIFICACION EXISTENCIA DE UN PATH-----------------------------------------*/
/*
 * @NAME: int osada_check_exist(char *path);
 * @DESC: Dado un PATH verifica su TOTAL existencia. Devuelve 1 caso positivo, 0 caso contrario.
 *
 * *NOTA: Un ejemplo de path puede ser: "/home/utnso/workspace/pepito.txt"
 * 			La función corrobora que existan cada uno de los directorios y que haya una correlatividad entre ellos,
 * 			además de revisar que NINGUNO de ellos esté borrado.
 * 			En el ejemplo mecionado, corrobora que: la carpeta "home" exista, que "utnso" exista y que su padre sea "home", etc.
 */
int osada_check_exist(char *path);
int verificar_existencia(char *file_or_directory, uint16_t dad_block); //<-- La usa la función anterior
int revisar_resultado(int result);//<-- La usa la función anterior


/*
 * @NAME: int calcular_posicion_en_tabla_de_archivos(int num_block, int position);
 * @DESC: Dado un número de bloque RELATIVO, y una posición (0 o 1), devolverá la posición exacta que tendrá en la TABLA DE ARCHIVOS
 *
 *  WARNING: RECUERDE QUE LA TABLA DE ARCHIVOS ES UN ARRAY, POR LO QUE LA POSICION DEVUELTA REPRESENTA LA POSICION EN EL ARRAY Y NO
 *  		EL NUM DE BYTE ABSOLUTO.
 *
 *  		EJEMPLO: calcular_posicion_en_tabla_de_archivos(1, 1); NOS DEVOLVERÁ LA POSICIÓN 3 DEL ARRAY.
 *  		NOTESE QUE LOS ARRAYS COMIENZAN EN 0. EL PRIMER BLOQUE (O) CONTIENE DOS POSICIONES (O,1),
 *  		EL SEGUNDO BLOQUE (1) CONTIENE DOS POSICIONES MAS (2,3), ....
 */
int calcular_posicion_en_tabla_de_archivos(int num_block, int position);

/*---------------------------------------------BORRADO DE ARCHIVOS---------------------------------------------------------*/
/*
 * @NAME:void osada_delete_this_file(char *path);
 * @DESC: Dado un PATH que lleva a un ARCHIVO, ELIMINA el mismo del disco.
 *
 * WARNING: AL LLAMAR A ESTA FUNCIÓN, SE PRESUME QUE EL PROGRAMADOR YA SE CHEQUEÓ LA EXISTENCIA DE DICHO PATH
 * 			(NO SE PUEDE BORRAR ALGO QUE NO ESTA EN DISCO). ADEMÁS, IMPACTA EN DISCO LOS CAMBIOS, POR LO QUE NO SE PODRÁ
 * 			VOLVER A RECUPERAR DICHO ARCHIVO.
 */
void osada_delete_this_file(char *path);
void delete_file(char *archivo); //<-- Simple delegación de la función anterior
int calcular_desplazamiento_tabla_de_archivos(int posicion_relativa); //<-- Simple delegación de la función anterior
int calcular_cantidad_bloques_admin(); //<-- Simple delegación


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

/*--------------------------------------DELETE DIR---------------------------------------*/

/*
 * @NAME: osada_delete_this_dir(char* path);
 * @DESC: Dado un path, chequea la existencia de ese directorio, borra a todos sus hijos, y se borra a si mismo,
 * 		  y cambia el tamaño del dir padre
 */
void osada_delete_this_dir(char* path);

/*
 * @NAME: borrar_archivo(char* path, char* nombre_archivo);
 * @DESC: Dado un nombre de archivo, y el path del padre, borra el archivo que esta dentro del dir padre.
 */
void borrar_archivo(char* path_padre, char*nombre_archivo_hijo);

/*
 * @NAME: borrar_directorio(char* path, char* nombre_archivo);
 * @DESC: Dado un directorio hijo, y la ruta del directorio padre  borra el directorio hijo(funcion recursiva con osada_delete_this_dir)
 */
void borrar_directorio(char* path, char* nombre_directorio);


/*
 * @NAME: osada_borrar_hijos(char* path);
 * @DESC: Dado un path, borra todos sus hijos, sean directorios o archivos.
 */
void osada_borrar_hijos(char* path);

/*
 * @NAME: osada_delete_dir_void(char* path);
 * @DESC: Dado un path de un directorio VACIO, setea en su tabla de archivos como borrado.
 */
void osada_delete_dir_void(char* path);

/*
 * @NAME: nombre_en_el_path(char* path)
 * @DESC: Dado un path, devuelve el string del ultimo archivo.
 * @EJEM: nombre_en_el_path("/utn/so/grecoso");
 * @RETN: "grecoso"
 */
char* nombre_en_el_path(char* path);

/*
 * @NAME: recalcular_tamanio_del_padre(char* path);
 * @DESC: Dado un path a borrar, recalcula el size del padre.
 */
void recalcular_tamanio_del_padre(char* path);

/*
 * @NAME: nuevo_tamanio_padre(int posicion, char* path)
 * @DESC: Dado la posicion del array de archivos del padre, y el path del hijo, hace la resta entre
 * su hijo(directorio) y el padre y lo pushea.
 */
void nuevo_tamanio_padre(int posicion, char* path);

/*
 * @NAME: tamanio_del_dir(char* path);
 * @DESC: Dado un path a un dir/archivo calcula su size y al usar nuevo_tamanio_padre queda pusheado.
 */

int tamanio_del_file(char* path);
/*
 * @NAME: posicion_del_padre(char* path)
 * @DESC: Dado un path devuelve la posicion en el array del padre.
 */
uint16_t posicion_del_padre(char* path);

/*
 * @NAME: es_el_padre(osada_file file,char* path);
 * @DESC: Dado un file y un path del supuesto padre, devuelve 1 si es el padre, 0 si no lo es.
 */
int es_el_padre(osada_file* file_hijo,char* path_padre);

/*
 * @NAME: es_el_hijo(osada_file* file_padre, char* path_hijo);
 * @DESC: Dado un file del padre y un path del hijo, determina si es el hijo, devuelve 1 si es el hijo , 0 sino lo es.
 *
 */

int es_el_hijo(osada_file* file_padre, char* path_hijo);

//#pragma pack(pop)


//----------------------------------------DEVOLVER LISTADO------------------------------//
typedef struct{
	char* name;
	osada_file_state state;
}t_file_show;

/*
 * @NAME:osada_devolve_listado(char* path_dir);
 * @DESC: Dado una ruta a un directorio, checkea su existencia y luego devuelve una lista de
 * t_file_show con todos los archivos y dir que tiene de hijos.
 */
t_list* osada_devolve_listado(char* path_dir);


/*
 * @NAME:filtrar_hijos(char* path_padre);
 * @DESC: Lo mismo que "osada_devolve_listado" pero sin checkear existencia.
 */
t_list* filtrar_hijos(char* path_padre);



#endif __OSADA_H__
