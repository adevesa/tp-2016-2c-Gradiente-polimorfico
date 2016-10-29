/*
 ============================================================================
 Name        : pokedex-server.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "osada.h"
#include "file_manipuling.h"
#include "ls_and_atributes.h"
#include "osada_generales.h"
#include "borrados.h"
#include "comunication.h"

extern t_disco_osada* disco;

int main(int argc, char* argv[])
{
	disco = osada_disco_abrite("/home/utnso/workspace/challenge.bin");

	//"/Pallet Town/Pokemons/Desafios/special.mp4"
	/*t_file_osada *file = osada_get_file_called("/README.txt",disco);
	t_attributes_file *atributos = osada_b_get_attributes_of_this_file("/README.txt");
	printf("%d\n", atributos->size);

	char* contenido = osada_get_data_of_this_file(file->file,disco);
	printf("%d\n", string_length(contenido));*/


	//t_attributes_file *atributos = osada_b_get_attributes_of_this_file("/README.TXT");
	//printf("%d\n", atributos->size);


	/*t_to_be_read *to_read = malloc(sizeof(t_to_be_read));
	to_read->offset = 0;
	to_read->size = 10;
	to_read->path = "/Pallet Town/Pokemons/Desafios/special.mp4";

	char *path = string_new();
	string_append(&path,"/Pallet Town/Pokemons/Desafios/special.mp4");
	t_file_osada *file = osada_get_file_called(path,disco);
	char *contenido_1 = osada_get_data_of_this_file(file->file,disco);
	printf("a %d \n", string_length(contenido_1));
	read_content *contenido = osada_a_read_file(to_read);
	printf("%s :: %d\n",contenido->contenido,string_length(contenido->contenido));*/
	pokedex_server_conectate();
	servidor_acepta_clientes();


	return EXIT_SUCCESS;
}
