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
void* osada_b_read_file(osada_file *file, t_disco_osada *disco, t_to_be_read *to_read)
{
	t_list *bloques_por_recuperar = osada_get_blocks_nums_of_this_file(file, disco);

	int cantidad_bloques = list_size(bloques_por_recuperar);

	int bloque_inicial =  to_read->offset/OSADA_BLOCK_SIZE;
	int desplazamiento_en_bloque_init = to_read->offset - (bloque_inicial * OSADA_BLOCK_SIZE);

	int bloque_final = calcular_bloque_final_por_leer(bloque_inicial,to_read);

	void *data = malloc(to_read->size);
	if(bloque_inicial == bloque_final)
	{
		int *block_num = list_get(bloques_por_recuperar,bloque_inicial);
		void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
		memcpy(data,data_recv+to_read->offset,to_read->size);
		return( (void*) data);
	}
	else
	{
		int i = bloque_inicial;
			int iteracion = 0;


			while(i<=bloque_final)
			{
				int *block_num = list_get(bloques_por_recuperar,i);
				void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
				if(i == bloque_final)
				{
					int cantidad_bloques_enteros_leidos = to_read->size / OSADA_BLOCK_SIZE;
					int bytes_a_leer_del_ultimo_bloque = to_read->size - (cantidad_bloques_enteros_leidos*OSADA_BLOCK_SIZE);
					memcpy(data + (iteracion*OSADA_BLOCK_SIZE),data_recv,bytes_a_leer_del_ultimo_bloque);
				}
				else
				{
					if(i == bloque_inicial)
					{
						memcpy(data + (desplazamiento_en_bloque_init),data_recv,OSADA_BLOCK_SIZE);
					}
					else
					{
						memcpy(data + (iteracion*OSADA_BLOCK_SIZE),data_recv,OSADA_BLOCK_SIZE);
					}
				}
				free(data_recv);
				iteracion++;
				i++;
			}
			list_destroy(bloques_por_recuperar);
			return data;
	}

}

int calcular_bloque_final_por_leer(int bloque_inicial,t_to_be_read *to_read)
{

	int bloque_final;
	if(to_read->size<= OSADA_BLOCK_SIZE -1)
	{
		bloque_final = bloque_inicial;
		return bloque_final;
	}
	else
	{
		int cantidad_bloques_enteros_leidos = to_read->size / OSADA_BLOCK_SIZE;
		return (bloque_inicial +cantidad_bloques_enteros_leidos );
	}
}

void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco)
{
	t_list *bloques_por_recuperar = osada_get_blocks_nums_of_this_file(file, disco);
	int size = list_size(bloques_por_recuperar);

	int last_block = size -1;
	void *data = malloc(file->file_size); //char *data = malloc(file->file_size +1);
	int i;
	for(i=0; i<(size); i++)
	{
		int *block_num = list_get(bloques_por_recuperar,i);
		void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);

		if(i == last_block)
		{
			int ultimo_byte_a_recibir = calcular_byte_final_a_recuperar_de_file(file->file_size);
			if(ultimo_byte_a_recibir == OSADA_BLOCK_SIZE)
			{
				memcpy(data + (i*OSADA_BLOCK_SIZE),data_recv,ultimo_byte_a_recibir);
				free(data_recv);
			}
			else
			{
				memcpy(data + (i*OSADA_BLOCK_SIZE),data_recv,ultimo_byte_a_recibir);
				free(data_recv);
			}
		}
		else
		{
			memcpy(data + (i*OSADA_BLOCK_SIZE),data_recv,OSADA_BLOCK_SIZE);
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
		return file_size;
	}
	else
	{
		//int bloques_completos = (int) floor(file_size / OSADA_BLOCK_SIZE);
		int bloques_completos =  file_size / OSADA_BLOCK_SIZE;
		if(bloques_completos * OSADA_BLOCK_SIZE == file_size)
		{
			return (OSADA_BLOCK_SIZE);
		}
		else
		{
			int byte_final = file_size -  OSADA_BLOCK_SIZE * bloques_completos;
			return byte_final;
		}

	}
}
