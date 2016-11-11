/*
 ============================================================================
 Name        : testeandoFilesytem.c
 Author      : Agustin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <string.h>

void main()
{
	//crear_N_archivos_vacios("/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/pokedex-cliente/Debug/tmp/carpet", 559);
	//copiar_archivo_N_veces("/home/utnso/Escritorio/asd.lol", "/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/pokedex-cliente/Debug/tmp", 50);
	crear_N_carpetas_vacias("/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/pokedex-cliente/Debug/tmp", 1024);
	//crear_n_carpetas_con_m_archivos_adentro("/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/pokedex-cliente/Debug/tmp", 24,41);
	//crear_n_carpetas_con_m_archivos_adentro2("/home/utnso/workspace/tp-2016-2c-Gradiente-polimorfico/pokedex-cliente/Debug/tmp", 15,41);
}

void crear_N_carpetas_vacias(char* ruta, int n)
{
	int i;

	for(i=0; i<n ; i++)
	{
		char* ruta_new = string_new();
		string_append(&ruta_new, ruta);
		string_append(&ruta_new, "/");
		string_append(&ruta_new, "carp");
		char* numero = string_itoa(i);
		string_append(&ruta_new, numero);
		char* orden = string_new();
		string_append(&orden, "mkdir ");
		string_append(&orden, ruta_new);
		system(orden);
		free(numero);
		free(orden);
		free(ruta_new);
	}
}

void crear_n_carpetas_con_m_archivos_adentro(char* ruta, int n, int m)
{
	int i, j;
	for(i=0; i<n ; i++)
		{
			char* ruta_new = string_new();
			string_append(&ruta_new, ruta);
			string_append(&ruta_new, "/");
			string_append(&ruta_new, "carp");
			char* numero = string_itoa(i);
			string_append(&ruta_new, numero);
			char* orden = string_new();
			string_append(&orden, "mkdir ");
			string_append(&orden, ruta_new);
			system(orden);
			for(j=0; j<m; j++)
			{
				crear_N_archivos_vacios(ruta_new, m);
			}

			free(numero);
			free(orden);
			free(ruta_new);
		}

}


void crear_n_carpetas_con_m_archivos_adentro2(char* ruta, int n, int m)
{
	int i, j;
	for(i=0; i<n ; i++)
		{
			char* ruta_new = string_new();
			string_append(&ruta_new, ruta);
			string_append(&ruta_new, "/");
			string_append(&ruta_new, "fd");
			char* numero = string_itoa(i);
			string_append(&ruta_new, numero);
			char* orden = string_new();
			string_append(&orden, "mkdir ");
			string_append(&orden, ruta_new);
			system(orden);
			for(j=0; j<m; j++)
			{
				crear_N_archivos_vacios(ruta_new, m);
			}

			free(numero);
			free(orden);
			free(ruta_new);
		}

}



void crear_N_archivos_vacios(char* ruta, int n)
{
	int i;

	for(i=0; i<n ; i++)
	{
		char* ruta_new = string_new();
		string_append(&ruta_new, ruta);
		string_append(&ruta_new, "/");
		string_append(&ruta_new, "arch");
		char* numero = string_itoa(i);
		string_append(&ruta_new, numero);
		char* orden = string_new();
		string_append(&orden, "touch ");
		string_append(&orden, ruta_new);
		system(orden);
		free(numero);
		free(orden);
		free(ruta_new);
	}

}

void copiar_archivo_N_veces(char* archivo_origen, char* destino, int n)
{
	int i;

	for(i=0; i<n ; i++)
	{
		char* ruta_new = string_new();
		string_append(&ruta_new, destino);
		string_append(&ruta_new, "/");
		string_append(&ruta_new, "archCOPY");
		char* numero = string_itoa(i);
		string_append(&ruta_new, numero);
		char* orden = string_new();
		string_append(&orden, "cp ");
		string_append(&orden, archivo_origen);
		string_append(&orden, " ");
		string_append(&orden, ruta_new);
		system(orden);
		free(numero);
		free(orden);
		free(ruta_new);
	}

}
