/*
 * osada-structs.h
 *
 *  Created on: 27/9/2016
 *      Author: utnso
 */

#ifndef OSADA_STRUCTS_H_
#define OSADA_STRUCTS_H_

typedef struct {
	osada_file_state state;
	unsigned char fname[OSADA_FILENAME_LENGTH];
	uint16_t parent_directory;
	uint32_t file_size;
	uint32_t lastmod;
	osada_block_pointer first_block;
} osada_file;

typedef struct {
	unsigned char magic_number[7]; // OSADAFS
	uint8_t version;
	uint32_t fs_blocks; // Tamaño del FS [bloques]
	uint32_t bitmap_blocks; // Tamaño del Bitmap [bloques]
	uint32_t allocations_table_offset; // Inicio Tabla Asignaciones [bloque]
	uint32_t data_blocks; // Tamaño Datos [bloques]
	unsigned char padding[40]; // useless bytes just to complete the block size (relleno)
} osada_header;



#endif /* OSADA_STRUCTS_H_ */
