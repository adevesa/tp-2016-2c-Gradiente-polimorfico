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
#define RAIZ  65535

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

typedef struct
{
	osada_file *file;
	char *path;
	int block_relative;
	int position_in_block;
}t_osada_file_free;

typedef t_osada_file_free t_file_osada;

typedef struct{
	t_file_osada* file;
	char* path;
	int tipo;
}t_file_listado;

typedef struct
{
	int size;
	int offset;
	char* path;
}t_to_be_read;

typedef struct
{
	int size;
	int offset;
	char* path;
	char* text;
}t_to_be_write;

typedef struct
{
	char* old_path;
	char* new_path;
}t_to_be_rename;

typedef struct
{
	int size;
	int tipo;
}t_attributes_file;

#endif __OSADA_H__
