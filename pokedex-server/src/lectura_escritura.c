/*
 * lectura_escritura.c
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */
#include "lectura_escritura.h"

/*----------------------------------------------TRUNCATE----------------------------------------------------------*/

/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
int osada_check_space_to_write(t_to_be_write *to_write)
{
	t_file_osada *file = osada_get_file_called(to_write->path,disco);
	int blOques_necesarios_para_satisfacer = to_write->size / OSADA_BLOCK_SIZE;

	if(blOques_necesarios_para_satisfacer == 0)
	{
		int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file->file);
		int capacidad_satisfaccion = espacio_disponible - to_write->size;
		if(capacidad_satisfaccion<0)
		{
			int disponibilidad_de_bloques = osada_b_check_is_bitarray_have_n_blocks_free(1,disco);
			t_file_osada_destroy(file);
			return disponibilidad_de_bloques;
		}
		else
		{
			t_file_osada_destroy(file);
			return HAY_ESPACIO_SUFICIENTE;
		}
	}
	else
	{
		int disponibilidad_de_bloques = osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
		if(disponibilidad_de_bloques == NO_HAY_ESPACIO_SUFICIENTE)
		{
			return NO_HAY_ESPACIO_SUFICIENTE;
		}
		else
		{
			if(to_write->size > (OSADA_BLOCK_SIZE*blOques_necesarios_para_satisfacer))
			{
				int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file->file);
				int a_escribir_en_bloque_incompleto = espacio_disponible - (to_write->size -(OSADA_BLOCK_SIZE*blOques_necesarios_para_satisfacer));
				if(a_escribir_en_bloque_incompleto <= espacio_disponible)
				{
							return HAY_ESPACIO_SUFICIENTE;
				}
				else
				{
					int disponibilidad_de_bloques = osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer+1,disco);
					t_file_osada_destroy(file);
					return disponibilidad_de_bloques;
				}
			}
			else
			{
				int disponibilidad_de_bloques = osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
				t_file_osada_destroy(file);
				return disponibilidad_de_bloques;
			}
		}

	}
}

int calcular_espacio_disponible_ultimo_bloque(osada_file *file)
{
	int bloques_enteros = file->file_size / OSADA_BLOCK_SIZE;
	int espacio_disponible_ultimo_bloque;
	if(bloques_enteros == 0)
	{
		espacio_disponible_ultimo_bloque = OSADA_BLOCK_SIZE - file->file_size;
		return espacio_disponible_ultimo_bloque;
	}
	else
	{
		espacio_disponible_ultimo_bloque = OSADA_BLOCK_SIZE - (file->file_size - (bloques_enteros * OSADA_BLOCK_SIZE));
		return espacio_disponible_ultimo_bloque;
	}
}

void osada_write_file(t_to_be_write* file)
{
	t_file_osada* archivo = osada_get_file_called(file->path,disco);
	void *data = osada_get_data_of_this_file(archivo,disco);

}

void asignar_bloques_de_datos_necesarios(t_to_be_write *to_write, t_file_osada *file)
{
	int blOques_necesarios_para_satisfacer = to_write->size / OSADA_BLOCK_SIZE;
	if(blOques_necesarios_para_satisfacer != 0)
	{

	}

}

void asignar_nuevo_bloque_datos(t_file_osada *archivo)
{
	int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
	int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);

	t_list *bloques = osada_get_blocks_nums_of_this_file(archivo->file,disco);
	if(list_size(bloques) == 0)
	{
		impactar_en_tabla_de_asignaciones(archivo->file->first_block,bloque_Asignado);
		osada_push_middle_block(TABLA_DE_ARCHIVOS,archivo->block_relative,archivo->position_in_block,archivo->file,disco);
	}
	else
	{
		int *ultimo_elemento = list_get(bloques,(list_size(bloques)-1));
		impactar_en_tabla_de_asignaciones(*ultimo_elemento,bloque_Asignado);
		osada_push_middle_block(TABLA_DE_ARCHIVOS,archivo->block_relative,archivo->position_in_block,archivo->file,disco);
	}
}

void impactar_en_tabla_de_asignaciones(int posicion, int valor)
{
	osada_block_pointer byte_inicial_tabla_asignaciones = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);
	int desplazamiento = byte_inicial_tabla_asignaciones + 4*posicion;
	int *value = malloc(4);
	*value = valor;
	memcpy(disco->map + desplazamiento,value,4);
	free(value);

	int offset_del_recien_asignado = byte_inicial_tabla_asignaciones + 4*valor;
	int *feof = malloc(4);
	*feof = FEOF;
	memcpy(disco->map + offset_del_recien_asignado,feof,4);
	free(feof);
}

int calcular_bloque_relativo_datos_dado_absoluto(int numero_bloque_absoluto)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int bloque_relativo = numero_bloque_absoluto - bloque_incial;
	return bloque_relativo;
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
