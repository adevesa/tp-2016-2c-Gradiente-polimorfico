/*
 * lectura_escritura.c
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */
#include "lectura_escritura.h"

/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
void osada_write_file(t_to_be_write* file)
{
	t_file_osada* archivo = osada_get_file_called(file->path,disco);
	if(archivo_vacio(archivo))
	{
		//osada_B_write_file_void(file);
	}
	else
	{
	t_list* bloque_de_datos_ocupados_por_el_archivo = osada_get_blocks_nums_of_this_file(archivo->file, disco);
	}
}

void osada_b_write_file_void(t_to_be_write* archivo)
{
	//asignar_nuevo_bloque_datos(archivo->file);
}

int archivo_vacio(t_file_osada* arch)
{
	if(arch->file->file_size==0) return 1;
	else return 0;
}

void asignar_nuevo_bloque_datos(osada_file* archivo)
{
	int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco); //impaktar
	//completar


}


/*----------------------------------------------LECTURA------------------------------------------------------------*/
void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco)
{
	t_list *bloques_por_recuperar = osada_get_blocks_nums_of_this_file(file, disco);
	int size = list_size(bloques_por_recuperar);

	int last_block = size -1;
	char *data = string_new();
	int i;
	for(i=0; i<(size); i++)
	{
		int *block_num = list_get(bloques_por_recuperar,i);
		void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
		if(i == last_block)
		{
			int ultimo_byte_a_recibir = calcular_byte_final_a_recuperar_de_file(file->file_size);
			if(ultimo_byte_a_recibir == 63)
			{
				string_append(&data, (char*) data_recv);
				free(data_recv);
			}
			else
			{
				void *data_last = osada_get_bytes_start_in(0,ultimo_byte_a_recibir,data_recv);
				string_append(&data, (char*) data_last);
				free(data_last);
			}
		}
		else
		{
			string_append(&data, (char*) data_recv);
			free(data_recv);

		}
	}
	list_destroy(bloques_por_recuperar);
	return( (void*) data);
}

int calcular_byte_final_a_recuperar_de_file(int file_size)
{
	if(file_size < OSADA_BLOCK_SIZE)
	{
		return file_size -1;
	}
	else
	{
		int bloques_completos = (int) floor(file_size / OSADA_BLOCK_SIZE);
		if(bloques_completos * OSADA_BLOCK_SIZE == file_size)
		{
			return (OSADA_BLOCK_SIZE-1);
		}
		else
		{
			int byte_final = file_size -  OSADA_BLOCK_SIZE * bloques_completos;
			return byte_final;
		}

	}
}
