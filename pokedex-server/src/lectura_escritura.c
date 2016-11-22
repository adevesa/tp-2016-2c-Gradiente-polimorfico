/*
 * lectura_escritura.c
 *
 *  Created on: 12/10/2016
 *      Author: utnso
 */
#include "lectura_escritura.h"
//extern pthread_mutex_t mutex_operaciones;
/*----------------------------------------------TRUNCATE----------------------------------------------------------*/
void osada_b_truncate_file_full(t_to_be_truncate *to_truncate,osada_file *file,t_info_file *info)
{
	if(to_truncate->new_size >= file->file_size)
	{
		osada_b_aumentar_tamanio_full(file,to_truncate->new_size,info);
	}
	else
	{
		osada_b_disminuir_tamanio_full(file,to_truncate->new_size,info);
	}
}

void osada_b_aumentar_tamanio_full(osada_file *file, int new_size,t_info_file *info)
{
	int tamanio_a_aumentar;
	if(new_size == file->file_size)
	{
		tamanio_a_aumentar=new_size;
		}
	else
	{
			tamanio_a_aumentar = new_size - file->file_size;
	}

	int bloques_necesarios = tamanio_a_aumentar / OSADA_BLOCK_SIZE;

	if(bloques_necesarios == 0)
	{
			asignar_un_unico_bloque_si_es_necesario_full(file,info,tamanio_a_aumentar);
	}
	else
	{
			if(file->first_block== FEOF)
			{
				asignar_un_unico_bloque_si_es_necesario_full(file,info,OSADA_BLOCK_SIZE);
				if(tamanio_a_aumentar > (OSADA_BLOCK_SIZE*bloques_necesarios))
				{
					asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios);
				}
				else
				{
					asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios-1);
				}
			}
			else
			{

				if(tamanio_a_aumentar > (OSADA_BLOCK_SIZE*bloques_necesarios))
				{
					int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file);
					if(espacio_disponible != 0)
					{
						int a_escribir_en_bloque_incompleto = espacio_disponible - (tamanio_a_aumentar -(OSADA_BLOCK_SIZE*bloques_necesarios));
						if(a_escribir_en_bloque_incompleto <= espacio_disponible && a_escribir_en_bloque_incompleto>0)
						{
							asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios);
						}
						else {asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios +1);}
					}
					else
					{
						asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios+1);
					}

				}
				else { asignar_nuevo_bloque_datos_full(file,info,bloques_necesarios);}
			}
		}
}

void asignar_un_unico_bloque_si_es_necesario_full(osada_file *file, t_info_file *info,int tamanio_a_aumentar)
{
	if(file->first_block == FEOF)
	{
			osada_block_pointer byte_inicial_tabla_asignaciones = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);

			int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
			int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
			limpiar_bloque_de_datos(bloque_Asignado);

			int offset_del_recien_asignado = byte_inicial_tabla_asignaciones + 4*bloque_Asignado;
			int *feof = malloc(4);
			*feof = FEOF;

			//pthread_mutex_lock(&mutex_operaciones);
			memcpy(disco->map + offset_del_recien_asignado,feof,4);
			//pthread_mutex_unlock(&mutex_operaciones);

			free(feof);

			file->first_block = bloque_Asignado;
			file->file_size=OSADA_BLOCK_SIZE;
			info->last_block_asigned = bloque_Asignado;
			info->cantidad_bloques_asignados = info->cantidad_bloques_asignados +1;
			osada_impactar_un_archivo(info->posicion_en_tabla_de_archivos,file);
		}
		else
		{
			int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file);
			int capacidad_satisfaccion = espacio_disponible - tamanio_a_aumentar;
			if(capacidad_satisfaccion<0)
			{
				asignar_nuevo_bloque_datos_full(file,info,1);
			}
		}
}

void asignar_nuevo_bloque_datos_full(osada_file * file, t_info_file *info,int n)
{
	int *ultimo_elemento = malloc(sizeof(int));
	*ultimo_elemento = info->last_block_asigned;

	int i=0;
	while(i<n)
	{
		int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
		int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
		limpiar_bloque_de_datos(bloque_Asignado);
		impactar_en_tabla_de_asignaciones(*ultimo_elemento,bloque_Asignado);
		*ultimo_elemento = bloque_Asignado;
		i++;
	}
	file->file_size = (file->file_size+ (n*OSADA_BLOCK_SIZE));
	info->last_block_asigned = *ultimo_elemento;
	info->cantidad_bloques_asignados = info->cantidad_bloques_asignados + n;
	osada_impactar_un_archivo(info->posicion_en_tabla_de_archivos,file);
	free(ultimo_elemento);
}

void impactar_en_tabla_de_asignaciones(int posicion, int valor)
{
	//pthread_mutex_lock(&mutex_operaciones);

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

	//pthread_mutex_unlock(&mutex_operaciones);

}

int osada_check_space_to_truncate_full(osada_file *file,t_info_file *info,int size)
{
	if(size< file->file_size)
	{
			return 1;
	}
	else
	{
			int blOques_necesarios_para_satisfacer = size / OSADA_BLOCK_SIZE;
			if(blOques_necesarios_para_satisfacer==0)
			{
				blOques_necesarios_para_satisfacer = 1;
			}

			if(file->file_size==0)
			{
				if(size > blOques_necesarios_para_satisfacer* OSADA_BLOCK_SIZE)
				{
						return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer+1,disco);
				}
				else
				{
					return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
				}
			}
			else
			{
				int cantidad_bloques_actuales = info->cantidad_bloques_asignados;
				if(size >= (blOques_necesarios_para_satisfacer - cantidad_bloques_actuales)* OSADA_BLOCK_SIZE)
				{
					if(blOques_necesarios_para_satisfacer==cantidad_bloques_actuales)
					{
					return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
					}
					else
					{
						return osada_b_check_is_bitarray_have_n_blocks_free((blOques_necesarios_para_satisfacer-cantidad_bloques_actuales),disco);
					}

				}
				else
				{
					return osada_b_check_is_bitarray_have_n_blocks_free((blOques_necesarios_para_satisfacer-cantidad_bloques_actuales),disco);
				}
			}
	}
}

void osada_b_disminuir_tamanio_full(osada_file *file, int new_size,t_info_file *info)
{
	int bloques_a_liberar = cantidad_de_bloques_a_desasignar(file->file_size,new_size);
	liberar_n_bloques_full(file,bloques_a_liberar,info);
	osada_b_change_size_file_full(file,new_size,info);
	info->cantidad_bloques_asignados = (info->cantidad_bloques_asignados) - bloques_a_liberar;
}

void liberar_n_bloques_full(osada_file *file, int bloques_a_liberar,t_info_file *info)
{
	t_list *bloques_actuales = osada_get_blocks_nums_of_this_file(file,disco);
	int cantidad_bloques_actuales = list_size(bloques_actuales);

	int i = cantidad_bloques_actuales -1;
	int stop = cantidad_bloques_actuales - bloques_a_liberar;
	if(stop!=0)
	{
		int cantidad_bloques_admin = calcular_cantidad_bloques_admin();
		while(i>=stop)
		{
			int *num_block = list_get(bloques_actuales,i);
			bitarray_clean_bit(disco->bitmap,cantidad_bloques_admin + (*num_block));
			i--;
		}
		osada_aumenta_cantidad_bloques_libres(bloques_a_liberar);
		impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);

		int *last_new_block = list_get(bloques_actuales,i);
		establecer_nuevo_feof_en_tabla_de_asignaciones(*last_new_block);
		info->last_block_asigned = *last_new_block;

	}
	else
	{
		osada_desocupa_n_bits(bloques_actuales);
		file->first_block = FEOF;
		osada_impactar_un_archivo(info->posicion_en_tabla_de_archivos,file);
		info->last_block_asigned = FEOF;
		info->last_block_write = FEOF;
	}
	list_destroy_and_destroy_elements(bloques_actuales,free_list_blocks);
}




void osada_b_truncate_file(t_to_be_truncate *to_truncate)
{

	//int new_size = to_truncate->new_size + to_truncate->file->file->file_size;
	if(to_truncate->new_size >= to_truncate->file->file->file_size)
	{
		osada_b_aumentar_tamanio(to_truncate->file,to_truncate->new_size);
	}
	else
	{
		osada_b_disminuir_tamanio(to_truncate->file,to_truncate->new_size);
	}
}

void osada_b_aumentar_tamanio(t_file_osada *file, int new_size)
{
	int tamanio_a_aumentar;
	if(new_size == file->file->file_size)
	{
		tamanio_a_aumentar=new_size;
	}
	else
	{
		tamanio_a_aumentar = new_size - file->file->file_size;
	}
	//int tamanio_a_aumentar = new_size-file->file->file_size;

	int bloques_necesarios = tamanio_a_aumentar / OSADA_BLOCK_SIZE;

	if(bloques_necesarios == 0)
	{
		asignar_un_unico_bloque_si_es_necesario(file,tamanio_a_aumentar);
	}
	else
	{
		if(file->file->first_block== FEOF)
		{
			//asignar_nuevo_bloque_datos(file);
			//file->file->first_block=osada_b_get_a_new_block_init();
			//int offset = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
			//osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative,offset,file->file,disco);
			asignar_un_unico_bloque_si_es_necesario(file,OSADA_BLOCK_SIZE);
			if(tamanio_a_aumentar > (OSADA_BLOCK_SIZE*bloques_necesarios))
			{
				asignar_n_bloques(file,bloques_necesarios);
			}
			else
			{
				asignar_n_bloques(file,bloques_necesarios-1);
			}
		}
		else
		{

			if(tamanio_a_aumentar > (OSADA_BLOCK_SIZE*bloques_necesarios))
			{
				int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file->file);
				if(espacio_disponible != 0)
				{
					int a_escribir_en_bloque_incompleto = espacio_disponible - (tamanio_a_aumentar -(OSADA_BLOCK_SIZE*bloques_necesarios));
					if(a_escribir_en_bloque_incompleto <= espacio_disponible && a_escribir_en_bloque_incompleto>0)
					{
						asignar_n_bloques(file,bloques_necesarios);
					}
					else {asignar_n_bloques(file,bloques_necesarios +1);}
				}
				else
				{
					asignar_n_bloques(file,bloques_necesarios+1);
				}

			}
			else { asignar_n_bloques(file,bloques_necesarios);}
		}
	}
}

void asignar_n_bloques(t_file_osada *file, int n)
{
	asignar_nuevo_bloque_datos_2(file,n);
}

void asignar_nuevo_bloque_datos_2(t_file_osada* file, int n)
{
	t_list *bloques = osada_get_blocks_nums_of_this_file(file->file,disco);
	int posicion_ultimo_elemento = list_size(bloques) -1;
	int *ultimo_elemento = list_get(bloques,posicion_ultimo_elemento);

	int i=0;
	while(i<n)
	{
		int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
		int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
		limpiar_bloque_de_datos(bloque_Asignado);
		impactar_en_tabla_de_asignaciones(*ultimo_elemento,bloque_Asignado);
		*ultimo_elemento = bloque_Asignado;
		i++;
	}
	osada_b_change_size_file(file,file->file->file_size+(n*OSADA_BLOCK_SIZE));
	list_destroy_and_destroy_elements(bloques, free_list_blocks);
}

void asignar_n_bloques_aux(t_file_osada*file, int n)
{
	void* vieja_tabla_de_asignacion = osada_get_table_asig();
	//int tamanio_tabla_asignaciones = calcular_tamanio_tabla_de_asignaciones(disco->header);
	int i;
	t_list* bloques = osada_get_blocks_asig(file->file);
	int cantidad_bloques = list_size(bloques);

	int* ultimo_elemento = list_get(bloques, cantidad_bloques-1);
	int byte_inicial_tabla_asig = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);
	int desplazamiento = byte_inicial_tabla_asig + *ultimo_elemento;

	for(i=0;i<n;i++)
	{
		int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
		int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
		limpiar_bloque_de_datos(bloque_Asignado);

		int* block_aux = malloc(sizeof(int));
		*block_aux = bloque_Asignado;

		//memcpy(disco->map+desplazamiento, block_aux, 4);
		memcpy(vieja_tabla_de_asignacion+(4*(*ultimo_elemento)),block_aux,sizeof(int));
		//impactar_en_tabla_de_asignaciones(*ultimo_elemento,bloque_Asignado);

		free(block_aux);
		*ultimo_elemento = bloque_Asignado;
		desplazamiento = byte_inicial_tabla_asig + *ultimo_elemento;
	}
	int *feof = malloc(4);
	*feof = FEOF;
	//memcpy(disco->map+desplazamiento,feof, 4);
	memcpy(vieja_tabla_de_asignacion+(4*(*ultimo_elemento)),feof,sizeof(int));
	free(feof);

	impactar_en_disco_tabla_asignaciones(vieja_tabla_de_asignacion);
	//free(vieja_tabla_de_asignacion);
	list_destroy_and_destroy_elements(bloques,free_list_blocks);
	osada_b_change_size_file(file,file->file->file_size+(n*OSADA_BLOCK_SIZE));//ERROR ACA
}

void asignar_un_unico_bloque_si_es_necesario(t_file_osada *file, int tamanio_a_aumentar)
{
	if(file->file->first_block == FEOF)
	{

		osada_block_pointer byte_inicial_tabla_asignaciones = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);

		int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
		int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
		limpiar_bloque_de_datos(bloque_Asignado);

		int offset_del_recien_asignado = byte_inicial_tabla_asignaciones + 4*bloque_Asignado;
		int *feof = malloc(4);
		*feof = FEOF;

		//pthread_mutex_lock(&mutex_operaciones);
		memcpy(disco->map + offset_del_recien_asignado,feof,4);
		//pthread_mutex_unlock(&mutex_operaciones);

		free(feof);

		file->file->first_block = bloque_Asignado;
		int offset = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
		osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative,offset,file->file,disco);

		osada_b_change_size_file(file, OSADA_BLOCK_SIZE);

	}
	else
	{

		int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file->file);
		int capacidad_satisfaccion = espacio_disponible - tamanio_a_aumentar;
		if(capacidad_satisfaccion<0)
		{
			asignar_nuevo_bloque_datos_2(file,1);
		}
	}
}

void asignar_nuevo_bloque_datos(t_file_osada *archivo)
{
	int num_bloque_absoluto = osada_ocupa_bit_libre_de(disco);
	int bloque_Asignado = calcular_bloque_relativo_datos_dado_absoluto(num_bloque_absoluto);
	limpiar_bloque_de_datos(bloque_Asignado);

	osada_b_change_size_file(archivo,archivo->file->file_size+OSADA_BLOCK_SIZE);

	t_list *bloques = osada_get_blocks_nums_of_this_file(archivo->file,disco);
	if(list_size(bloques) == 0)
	{
		impactar_en_tabla_de_asignaciones(archivo->file->first_block,bloque_Asignado);
		list_destroy(bloques);
	}
	else
	{
		int *ultimo_elemento = list_get(bloques,(list_size(bloques)-1));
		impactar_en_tabla_de_asignaciones(*ultimo_elemento,bloque_Asignado);
		list_destroy_and_destroy_elements(bloques,free_list_blocks);
	}
}

int calcular_bloque_relativo_datos_dado_absoluto(int numero_bloque_absoluto)
{
	int bloque_incial = osada_get_start_block_absolut_of(BLOQUE_DE_DATOS,disco);
	int bloque_relativo = numero_bloque_absoluto - bloque_incial;
	return bloque_relativo;
}

void osada_b_disminuir_tamanio(t_file_osada *file, int new_size)
{
	int bloques_a_liberar = cantidad_de_bloques_a_desasignar(file->file->file_size,new_size);
	liberar_n_bloques(file,bloques_a_liberar);
	osada_b_change_size_file(file,new_size);
}

void liberar_n_bloques(t_file_osada *file, int bloques_a_liberar)
{
	t_list *bloques_actuales = osada_get_blocks_nums_of_this_file(file->file,disco);
	int cantidad_bloques_actuales = list_size(bloques_actuales);

	int i = cantidad_bloques_actuales -1;
	int stop = cantidad_bloques_actuales - bloques_a_liberar;
	if(stop!=0)
	{
		int cantidad_bloques_admin = calcular_cantidad_bloques_admin();
		while(i>=stop)
		{
		int *num_block = list_get(bloques_actuales,i);
		bitarray_clean_bit(disco->bitmap,cantidad_bloques_admin + (*num_block));
		i--;
		}
		impactar_en_disco_n_bloques(OSADA_BLOCK_SIZE,disco->header->bitmap_blocks,disco->bitmap->bitarray,disco->map);
		int *last_new_block = list_get(bloques_actuales,i);
		establecer_nuevo_feof_en_tabla_de_asignaciones(*last_new_block);
		//file->file->file_size = stop;
		//int offset = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
		//osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative,offset,file->file,disco);
	}
	else
	{
		osada_desocupa_n_bits(bloques_actuales);
		file->file->file_size=0;
		file->file->first_block = FEOF;
		int offset = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
		osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative,offset,file->file,disco);
	}
	list_destroy_and_destroy_elements(bloques_actuales,free_list_blocks);
}

void establecer_nuevo_feof_en_tabla_de_asignaciones(int posicion)
{
	osada_block_pointer byte_inicial_tabla_asignaciones = calcular_byte_inicial_absolut(disco->header->allocations_table_offset);
	int desplazamiento = byte_inicial_tabla_asignaciones + 4*posicion;
	int *value = malloc(4);
	*value = FEOF;
	memcpy(disco->map + desplazamiento,value,4);
	free(value);
}

int cantidad_de_bloques_a_desasignar(uint32_t tamanio_actual, int new_size)
{
		int bloques_actuales = tamanio_actual / OSADA_BLOCK_SIZE;
		if(tamanio_actual>bloques_actuales*OSADA_BLOCK_SIZE)
		{
			bloques_actuales =bloques_actuales + 1;
		}
		else
		{
			if(tamanio_actual<OSADA_BLOCK_SIZE)
			{
				bloques_actuales=1;
			}
		}

		int nuevos_bloques_requeridos;
		if(new_size<OSADA_BLOCK_SIZE)
		{
			if(new_size==0)
			{
				nuevos_bloques_requeridos=0;
			}
			else
			{
				nuevos_bloques_requeridos = 1;
			}
		}
		else
		{
			int cantidad_entera_requerida = new_size / OSADA_BLOCK_SIZE;
			if(new_size>cantidad_entera_requerida*OSADA_BLOCK_SIZE)
			{
				nuevos_bloques_requeridos = cantidad_entera_requerida +1;
			}
			else
			{
				nuevos_bloques_requeridos = cantidad_entera_requerida;
			}
		}

	return (bloques_actuales - nuevos_bloques_requeridos);
}

int osada_check_space_to_truncate(char* path,t_file_osada* file,int size)
{
	if(path!=NULL)
	{
		file = osada_get_file_called(path,disco);
	}

	if(size< file->file->file_size)
	{
		return 1;
	}
	else
	{
		int blOques_necesarios_para_satisfacer = size / OSADA_BLOCK_SIZE;
		if(blOques_necesarios_para_satisfacer==0)
		{
			blOques_necesarios_para_satisfacer = 1;
		}

		if(file->file->file_size==0)
		{
			if(size > blOques_necesarios_para_satisfacer* OSADA_BLOCK_SIZE)
			{
					return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer+1,disco);
			}
			else
			{
				return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
			}
		}
		else
		{
			t_list* bloques_asignados = osada_get_blocks_nums_of_this_file(file->file,disco);
			int cantidad_bloques_actuales = list_size(bloques_asignados);
			if(cantidad_bloques_actuales!=0)
			{
			list_destroy_and_destroy_elements(bloques_asignados,free_list_blocks);
			}
			else
			{
			list_destroy(bloques_asignados);
			}

			if(size >= (blOques_necesarios_para_satisfacer - cantidad_bloques_actuales)* OSADA_BLOCK_SIZE)
			{
				if(blOques_necesarios_para_satisfacer==cantidad_bloques_actuales)
				{
				return osada_b_check_is_bitarray_have_n_blocks_free(blOques_necesarios_para_satisfacer,disco);
				}
				else
				{
					return osada_b_check_is_bitarray_have_n_blocks_free((blOques_necesarios_para_satisfacer-cantidad_bloques_actuales),disco);
				}

			}
			else
			{
				return osada_b_check_is_bitarray_have_n_blocks_free((blOques_necesarios_para_satisfacer-cantidad_bloques_actuales),disco);
			}
		}
	}
}


/*----------------------------------------------ESCRITURA----------------------------------------------------------*/
int osada_check_space_to_write(t_to_be_write *to_write)
{
	t_file_osada *file = osada_get_file_called(to_write->path,disco);
	if(to_write->size < file->file->file_size)
	{
		return 1;
	}
	else
	{
		int blOques_necesarios_para_satisfacer = (to_write->size - file->file->file_size) / OSADA_BLOCK_SIZE;

			if(blOques_necesarios_para_satisfacer == 0)
			{
				int espacio_disponible = calcular_espacio_disponible_ultimo_bloque(file->file);
				int capacidad_satisfaccion = espacio_disponible - (to_write->size - file->file->file_size);
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
						int a_escribir_en_bloque_incompleto = espacio_disponible - (to_write->size - file->file->file_size -(OSADA_BLOCK_SIZE*blOques_necesarios_para_satisfacer));
						if(a_escribir_en_bloque_incompleto <= espacio_disponible && a_escribir_en_bloque_incompleto >0)
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
		if(file->file_size  == (OSADA_BLOCK_SIZE*bloques_enteros))
		{
			return 0;
		}
		else
		{
			espacio_disponible_ultimo_bloque = OSADA_BLOCK_SIZE - (file->file_size - (bloques_enteros * OSADA_BLOCK_SIZE));
			return espacio_disponible_ultimo_bloque;
		}
	}
}

void osada_write_aux_file(t_to_be_write* to_write)
{
	int tamanio_del_archivo = to_write->size_inmediatamente_anterior;
	t_list *bloques = osada_get_blocks_nums_of_this_file(to_write->file->file,disco);
	int cantidad_bloques = list_size(bloques);
	int ultimo_bloque_escrito = (to_write->size_inmediatamente_anterior / OSADA_BLOCK_SIZE);

	int index = ultimo_bloque_escrito;
	int i=0;
	//pthread_mutex_lock(&mutex_operaciones);
	while(index<cantidad_bloques)
	{
		int *num_block = list_get(bloques,index);
		void *data_aux = malloc(OSADA_BLOCK_SIZE);
		memcpy(data_aux,to_write->text + (i*OSADA_BLOCK_SIZE),OSADA_BLOCK_SIZE);
		osada_push_block(BLOQUE_DE_DATOS,*num_block,data_aux,disco);
		free(data_aux);
		i++;
		index++;
	}
	osada_b_change_size_file(to_write->file,to_write->size_inmediatamente_anterior + to_write->size);
	//pthread_mutex_unlock(&mutex_operaciones);
	list_destroy_and_destroy_elements(bloques, free_list_blocks);
}

int es_multiplo_de(int numero_1, int numero_2)
{
	int division = numero_1 / numero_2;
	if(numero_1 == (division*numero_2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}







void osada_write_little_file_full_(t_to_be_write* file, osada_file *archivo, t_info_file *info)
{
	int tamanio_del_archivo = file->size_inmediatamente_anterior;
	t_list *bloques = osada_get_blocks_nums_of_this_file(archivo,disco);
	int cantidad_bloques = list_size(bloques);
	void *data = osada_get_data_of_this_file(archivo,disco);
	void *aux = malloc(OSADA_BLOCK_SIZE * cantidad_bloques);
	if(file->offset == 0)
	{
		memcpy(aux, file->text, file->size);
		osada_b_alter_data_blocks_full(aux,bloques);
		osada_b_change_size_file_full(archivo, file->size,info);
	}
	else
	{
		if(file->offset < tamanio_del_archivo)
		{
				memcpy(aux,data,file->offset);
				memcpy(aux + file->offset, file->text, file->size);
				memcpy(aux + (file->offset + file->size),data + file->offset, (tamanio_del_archivo - file->offset));
				osada_b_alter_data_blocks_full(aux,bloques);
				osada_b_change_size_file_full(archivo, file->size + tamanio_del_archivo,info);
		}
		else
		{
				memcpy(aux,data,archivo->file_size);
				memcpy(aux + file->offset, file->text, file->size);
				osada_b_alter_data_blocks_full(aux,bloques);
				osada_b_change_size_file_full(archivo, file->size + tamanio_del_archivo,info);
		}
	}

		free(aux);
		free(data);
		int* ultimo_bloque = list_get(bloques,cantidad_bloques -1);
		info->last_block_write = *ultimo_bloque;
		list_destroy_and_destroy_elements(bloques,free_list_blocks);
}

void osada_b_alter_data_blocks_full(void *data_new, t_list *bloques)
{
	int cantidad_bloques = list_size(bloques);

	int i = 0;
	while(i<cantidad_bloques)
	{
		int *num_block = list_get(bloques,i);
		void *data_aux = malloc(OSADA_BLOCK_SIZE);
		memcpy(data_aux, data_new + (i*OSADA_BLOCK_SIZE),OSADA_BLOCK_SIZE);
		osada_push_block(BLOQUE_DE_DATOS,*num_block,data_aux,disco);
		free(data_aux);
		i++;
	}
}

void osada_b_change_size_file_full(osada_file *file,int new_size, t_info_file *info)
{
	uint32_t size = (uint32_t) new_size;
	file->file_size = size;
	osada_impactar_un_archivo(info->posicion_en_tabla_de_archivos,file);
}

void osada_write_big_file_full_(t_to_be_write* to_write, osada_file *archivo, t_info_file *info)
{
	t_list *bloques = osada_get_blocks_nums_of_this_file_since(info->last_block_write);
	//t_list *bloques = osada_get_blocks_nums_of_this_file(to_write->file->file,disco);
	int cantidad_bloques = list_size(bloques);
	//int ultimo_bloque_escrito = (to_write->size_inmediatamente_anterior / OSADA_BLOCK_SIZE);

	int index = 1;
	int i=0;
	while(index<cantidad_bloques)
	{
		int *num_block = list_get(bloques,index);
		void *data_aux = malloc(OSADA_BLOCK_SIZE);
		memcpy(data_aux,to_write->text + (i*OSADA_BLOCK_SIZE),OSADA_BLOCK_SIZE);
		osada_push_block(BLOQUE_DE_DATOS,*num_block,data_aux,disco);
		free(data_aux);
		i++;
		index++;
	}
	osada_b_change_size_file_full(archivo,to_write->size_inmediatamente_anterior + to_write->size,info);
	info->last_block_write = info->last_block_asigned;
	list_destroy_and_destroy_elements(bloques, free_list_blocks);
}




void osada_write_file(t_to_be_write* file)
{
	//int tamanio_del_archivo = file->file->file->file_size;

	int tamanio_del_archivo = file->size_inmediatamente_anterior;
	/*if(tamanio_del_archivo<= file->offset  || tamanio_del_archivo==0)
	{
		int ultimo_bloque_escrito_es = ultimo_bloque_escrito(file->file);
		t_list *bloques = osada_get_blocks_nums_of_this_file(file->file->file,disco);
		int espacio_disponible_ultimo_bloque = calcular_espacio_disponible_ultimo_bloque(file->file->file);
		switch(espacio_disponible_ultimo_bloque)
		{
			case(0):
			{
				osada_b_alter_data_blocks_since_ultil(file->file,file->text,bloques,ultimo_bloque_escrito_es+1,-3,NULL);
				osada_b_change_size_file(file->file,file->file->file->file_size+file->size);
			};break;
			case(OSADA_BLOCK_SIZE):
			{
				osada_b_alter_data_blocks_since_ultil(file->file,file->text,bloques,ultimo_bloque_escrito_es,-3,NULL);
				osada_b_change_size_file(file->file,file->file->file->file_size+file->size);

			};break;
			default:
			{
				void* aux = malloc(espacio_disponible_ultimo_bloque+1);
				memcpy(aux,file->text,espacio_disponible_ultimo_bloque);
				int byte_donde_empiezo = calcular_byte_final_a_recuperar_de_file(tamanio_del_archivo);
				osada_b_alter_data_blocks_since_ultil(file->file,aux,bloques,ultimo_bloque_escrito_es,15,byte_donde_empiezo+1);
				free(aux);

				int desplazamiento = espacio_disponible_ultimo_bloque;
				osada_b_alter_data_blocks_since_ultil(file->file,file->text+desplazamiento,bloques,ultimo_bloque_escrito_es+1,-3,NULL);
				osada_b_change_size_file(file->file,file->file->file->file_size+file->size);
			}
		}

		list_destroy_and_destroy_elements(bloques,free_list_blocks);
	}

	else
	{*/

	//pthread_mutex_lock(&mutex_operaciones);
	t_list *bloques = osada_get_blocks_nums_of_this_file(file->file->file,disco);
	//t_list* bloques = osada_get_blocks_asig(file->file->file);
	int cantidad_bloques = list_size(bloques);
	void *data = osada_get_data_of_this_file(file->file->file,disco);
	//pthread_mutex_unlock(&mutex_operaciones);

	void *aux = malloc(OSADA_BLOCK_SIZE * cantidad_bloques);
	if(file->offset == 0)
	{
		memcpy(aux, file->text, file->size);
		osada_b_alter_data_blocks(file->file,aux,bloques);
		osada_b_change_size_file(file->file, file->size);
	}
	else
	{
		if(file->offset < tamanio_del_archivo)
		{
			memcpy(aux,data,file->offset);
			memcpy(aux + file->offset, file->text, file->size);
			memcpy(aux + (file->offset + file->size),data + file->offset, (tamanio_del_archivo - file->offset));
			osada_b_alter_data_blocks(file->file,aux,bloques);
			osada_b_change_size_file(file->file, file->size + tamanio_del_archivo);
		}
		else
		{
			memcpy(aux,data,file->file->file->file_size);
			memcpy(aux + file->offset, file->text, file->size);
			osada_b_alter_data_blocks(file->file,aux,bloques);
			osada_b_change_size_file(file->file, file->size + tamanio_del_archivo);
		}
	}
	free(aux);
	free(data);
	list_destroy_and_destroy_elements(bloques,free_list_blocks);
	//}
}

int ultimo_bloque_escrito(t_file_osada* file)
{
	int ultimo_bloque_escrito = file->file->file_size / OSADA_BLOCK_SIZE;
	if(file->file->file_size > (ultimo_bloque_escrito*OSADA_BLOCK_SIZE) )
	{
		ultimo_bloque_escrito = ultimo_bloque_escrito+1;
	}
	return ultimo_bloque_escrito;
}

void osada_b_alter_data_blocks(t_file_osada *file, void *data_new, t_list *bloques)
{
	int cantidad_bloques = list_size(bloques);

	int i = 0;
	while(i<cantidad_bloques)
	{
		int *num_block = list_get(bloques,i);
		void *data_aux = malloc(OSADA_BLOCK_SIZE);
		memcpy(data_aux, data_new + (i*OSADA_BLOCK_SIZE),OSADA_BLOCK_SIZE);
		osada_push_block(BLOQUE_DE_DATOS,*num_block,data_aux,disco);
		free(data_aux);
		i++;
	}
}

void osada_b_alter_data_blocks_since_ultil(t_file_osada *file, void* data_new, t_list* bloques, int start,int end,int byte_init)
{
	int size = list_size(bloques);
	int i = start;
	if(end==-3)
	{
		int iteracion = 0;
		while(i<size)
		{
			int *num_block = list_get(bloques,i);
			//void *data_aux = malloc(OSADA_BLOCK_SIZE);
			//memcpy(data_aux, data_new + (iteracion*OSADA_BLOCK_SIZE),OSADA_BLOCK_SIZE);
			osada_push_block(BLOQUE_DE_DATOS,*num_block,data_new+(iteracion*OSADA_BLOCK_SIZE),disco);
			//free(data_aux);
			iteracion++;
			i++;
		}
	}
	else
	{
		int *num_block = list_get(bloques,i);
		int byte_inicial = calcular_byte_inicial_relative(BLOQUE_DE_DATOS,*num_block,disco->header);
		int i2;
		int byte = byte_inicial+byte_init;
		int byte_final = OSADA_BLOCK_SIZE-1;
		char* mapping = (char*) disco->map;
		char* date = (char*) data_new;
		for(i2=byte_inicial; i2<=byte_final;i2++)
		{
			mapping[i2] = date[byte];
			byte++;
		}
	}
}

void osada_b_change_size_file(t_file_osada *file,int new_size)
{
	uint32_t size = (uint32_t) new_size;
	file->file->file_size = size;
	int desplazamiento = calcular_desplazamiento_tabla_de_archivos(file->position_in_block);
	//pthread_mutex_lock(&mutex_operaciones);
	osada_push_middle_block(TABLA_DE_ARCHIVOS,file->block_relative,desplazamiento,file->file,disco);
	//pthread_mutex_unlock(&mutex_operaciones);
}

/*----------------------------------------------LECTURA------------------------------------------------------------*/
void* osada_b_read_file(osada_file *file, t_disco_osada *disco, t_to_be_read *to_read, t_list* bloques_por_recuperar )
{
	if(file->first_block==FEOF)
	{
		return NO_EXISTE;
	}
	else
	{
		int bloque_inicial =  (to_read->offset/OSADA_BLOCK_SIZE);
		int desplazamiento_en_bloque_init = to_read->offset - (bloque_inicial * OSADA_BLOCK_SIZE);

		int bloque_final = calcular_bloque_final_por_leer(bloque_inicial,to_read);

		void *data = malloc(to_read->size);
		if(bloque_inicial == bloque_final)
		{
			int *block_num = list_get(bloques_por_recuperar,bloque_inicial);
			void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
			memcpy(data,data_recv+to_read->offset,to_read->size);
			free(data_recv);
			return( (void*) data);
		}
		else
		{
			int i = bloque_inicial;
			int iteracion = 0;

			if(to_read->size <= OSADA_BLOCK_SIZE*(bloque_final-bloque_inicial))
			{
				bloque_final = bloque_final -1;
			}
			while(i<=bloque_final)
			{
				int *block_num = list_get(bloques_por_recuperar,i);
				void *data_recv = osada_get_blocks_relative_since(BLOQUE_DE_DATOS,*block_num,1,disco);
				if(i == bloque_final)
				{
					int cantidad_bloques_enteros_leidos = to_read->size / OSADA_BLOCK_SIZE;
					int bytes_a_leer_del_ultimo_bloque = to_read->size - (cantidad_bloques_enteros_leidos*OSADA_BLOCK_SIZE);
					if(bytes_a_leer_del_ultimo_bloque==0)
					{
						memcpy(data + (iteracion*OSADA_BLOCK_SIZE),data_recv,OSADA_BLOCK_SIZE);
					}
					else
					{
						memcpy(data + (iteracion*OSADA_BLOCK_SIZE),data_recv,bytes_a_leer_del_ultimo_bloque);
					}

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

			return data;
		}
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
		if(to_read->size> cantidad_bloques_enteros_leidos*OSADA_BLOCK_SIZE)
		{
			return (bloque_inicial +cantidad_bloques_enteros_leidos +1 );
		}
		else
		{
			return (bloque_inicial +cantidad_bloques_enteros_leidos );
		}
	}
}


void* osada_get_data_of_this_file(osada_file *file, t_disco_osada *disco)
{
	t_list *bloques_por_recuperar = osada_get_blocks_nums_of_this_file(file, disco);
	int size = list_size(bloques_por_recuperar);

	int last_block = size -1;
	void *data = malloc(size*OSADA_BLOCK_SIZE);
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
	list_destroy_and_destroy_elements(bloques_por_recuperar,free_list_blocks);
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
