#include <stdint.h>
#include <commons/bitarray.h>


#ifndef __OSADA_H__
#define __OSADA_H__

#define OSADA_BLOCK_SIZE 64
#define OSADA_FILENAME_LENGTH 17

typedef unsigned char osada_block[OSADA_BLOCK_SIZE];
typedef uint32_t osada_block_pointer;

// set __attribute__((packed)) for this whole section
// See http://stackoverflow.com/a/11772340/641451
#pragma pack(push, 1)

typedef struct {
	unsigned char magic_number[7]; // OSADAFS
	uint8_t version;
	uint32_t fs_blocks; // Tamaño del FS [bloques]
	uint32_t bitmap_blocks; // Tamaño del Bitmap [bloques]
	uint32_t allocations_table_offset; // Inicio Tabla Asignaciones [bloque]
	uint32_t data_blocks; // Tamaño Datos [bloques]
	unsigned char padding[40]; // useless bytes just to complete the block size (relleno)
} osada_header;


_Static_assert( sizeof(osada_header) == sizeof(osada_block), "osada_header size does not match osada_block size");

typedef enum __attribute__((packed)) {
    DELETED = '\0',
    REGULAR = '\1',
    DIRECTORY = '\2',
} osada_file_state;

_Static_assert( sizeof(osada_file_state) == 1, "osada_file_state is not a char type");

typedef struct {
	osada_file_state state;
	unsigned char fname[OSADA_FILENAME_LENGTH];
	uint16_t parent_directory;
	uint32_t file_size;
	uint32_t lastmod;
	osada_block_pointer first_block;
} osada_file;

typedef struct {
	uint32_t bloque_inicial;
	uint32_t bloque_final;
	uint32_t size;
	t_bitarray *datos;
} t_bitmap;

typedef struct {
	uint32_t bloque_inicial;
	uint32_t bloque_final;
	uint32_t size;
	int *datos;
} t_asignaciones;

typedef struct {
	uint32_t bloque_inicial;
	uint32_t bloque_final;
	uint32_t size;
	int *datos;
} t_bloques_de_datos;


_Static_assert( sizeof(osada_file) == (sizeof(osada_block) / 2.0), "osada_file size does not half osada_block size");

#pragma pack(pop)

#endif __OSADA_H__
