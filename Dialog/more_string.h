/*
 * more_string.h
 *
 *  Created on: 24/9/2016
 *      Author: utnso
 */

#ifndef MORE_STRING_H_
#define MORE_STRING_H_

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <commons/string.h>

void free_string_array(char **path);
int string_contains(char *palabra, char *conteiner);
void string_replace(char *palabra, char *este_caracter,char *por_este);
void string_path_replace_spaces(char *path, char *este_caracter, char *por_este);
char* armar_coordenada(int x, int y, int MAX_BYTES_COORDENADA);
#endif /* MORE_STRING_H_ */
