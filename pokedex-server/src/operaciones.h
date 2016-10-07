/*
 * operaciones.h
 *
 *  Created on: 2/10/2016
 *      Author: utnso
 */

#ifndef OPERACIONES_H_
#define OPERACIONES_H_
#include "osada.h"
#include "comunication.h"

/*-------------------------------------------ATRIBUTOS------------------------------------------------------------------*/
void* osada_get_attributes(char *path);
void* osada_get_list_dir(char *path);
/*-------------------------------------------CREACION-------------------------------------------------------------------*/
void* osada_create_file(char *path);
void* osada_create_dir(char *path);
/*-------------------------------------------ELMINACION-----------------------------------------------------------------*/
void* osada_delete_file(char *path);
void* osada_delete_dir(char *path);
/*-------------------------------------------WRITE & READ---------------------------------------------------------------*/
void* osada_read_file(t_to_be_read *to_read);
void* osada_write_file(t_to_be_write *to_write);

/*-------------------------------------------RENAME---------------------------------------------------------------------*/
void* osada_rename_file(t_to_be_rename *to_rename);

/*-------------------------------------------OPENS & CLOSER-------------------------------------------------------------*/
void* osada_open_file(char *path);
#endif /* OPERACIONES_H_ */
