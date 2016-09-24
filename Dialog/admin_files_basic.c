/*
 * admin_files_basic.c
 *
 *  Created on: 24/9/2016
 *      Author: utnso
 */

#include "admin_files_basic.h"

void copiar(char* origen, char* destino)
{
	string_path_replace_spaces(origen, " ", "\\");
	string_path_replace_spaces(destino, " ","\\");
	char* mensaje = string_new();
	string_append(&mensaje, "cp ");
	string_append(&mensaje, origen);
	string_append(&mensaje, " ");
	string_append(&mensaje, destino);
	system(mensaje);
	free(mensaje);
}

void eliminar(char* elemento)
{
		char* mensaje = string_new();
		string_append(&mensaje, "rm ");
		string_append(&mensaje, elemento);
		system(mensaje);
		free(mensaje);
}
