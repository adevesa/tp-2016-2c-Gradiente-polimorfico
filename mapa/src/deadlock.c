/*
 * deadlock.c
 *
 *  Created on: 3/11/2016
 *      Author: utnso
 */
#include "deadlock.h"
extern char** vector_auxiliar_identificadores_pokenest;

//t_list* lista_auxiliar_entrenadores;
t_dictionary* procesos;
t_dictionary* procesos_identificados_por_indice;
int cantidad_columnas_actuales = 0;
int cantidad_columnas_ocupadas = 0;
int indice_posicion_proceso = 0;

int* vector_procesos_sin_recursos_asignados;
int* vector_T;

/*----------------------------------------------INICIALIZACION----------------------------------------------------------*/
t_estructuras_deteccion* deadlock_inicializate()
{
	t_estructuras_deteccion *new_deadlock = malloc(sizeof(t_estructuras_deteccion));
	new_deadlock->vector_recursos_totales = inicializar_recursos_totales();
	new_deadlock->vector_recursos_disponibles = inicializar_recursos_disponibles();
	new_deadlock->matriz_asignacion=inicializar_matriz_asignacion();
	new_deadlock->matriz_solicitud = inicializar_matriz_solicitud();
	procesos = dictionary_create();
	procesos_identificados_por_indice = dictionary_create();

	vector_procesos_sin_recursos_asignados = malloc(sizeof(int)+1);
	vector_procesos_sin_recursos_asignados[0] = -1;

	cantidad_columnas_actuales = 1;
	return new_deadlock;
}

int* inicializar_recursos_totales()
{
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	int* new_vector = malloc(cantidad_pokenest * sizeof(int));

	int i;
	for(i=0;i<cantidad_pokenest;i++)
	{
		t_pokeNest *pokenest = dictionary_get(mapa->pokeNests,vector_auxiliar_identificadores_pokenest[i]);
		new_vector[i] = pokenest->cantidad_pokemones_disponibles;
	}
	return new_vector;
}

int* inicializar_recursos_disponibles()
{
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	int* new_vector = malloc(cantidad_pokenest * sizeof(int));

	int i;
	for(i=0;i<cantidad_pokenest;i++)
	{
		t_pokeNest *pokenest = dictionary_get(mapa->pokeNests,vector_auxiliar_identificadores_pokenest[i]);
		new_vector[i] = pokenest->cantidad_pokemones_disponibles;
	}
	return new_vector;
}

int** inicializar_matriz_asignacion()
{
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	int** new_matriz = malloc(cantidad_pokenest*sizeof(int));

	/* ASIGNO ESPACIO PARA UNA SOLA COLUMNA A TODAS LAS FILAS */
	int i;
	for(i=0;i<cantidad_pokenest;i++)
	{
		new_matriz[i] = malloc(sizeof(int));
		new_matriz[i][0] = 0;
	}
	return new_matriz;
}

int** inicializar_matriz_solicitud()
{
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	int** new_matriz = malloc(cantidad_pokenest*sizeof(int));
	/* ASIGNO ESPACIO PARA UNA SOLA COLUMNA A TODAS LAS FILAS */
	int i;
	for(i=0;i<cantidad_pokenest;i++)
	{
		new_matriz[i] = malloc(sizeof(int));
		new_matriz[i][0] = 0;
	}
	return new_matriz;
}

void limpiar_filas(int matriz)
{
	switch(matriz)
	{
		case(MATRIZ_ASIGNACION):
		{
			int i;
			int cantidad_pokenest = dictionary_size(mapa->pokeNests);

			for(i=0;i<cantidad_pokenest;i++)
			{
				deadlock->matriz_asignacion[i][0]=0;
			}

		};break;
		case(MATRIZ_SOLICITUD):
		{
			int i;
			int cantidad_pokenest = dictionary_size(mapa->pokeNests);

			for(i=0;i<cantidad_pokenest;i++)
			{
				deadlock->matriz_solicitud[i][0]=0;
			}
		};break;
	}
}

void iniciar_vectorT()
{
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	vector_T= malloc((cantidad_pokenest +1)* sizeof(int));
	memcpy(vector_T,deadlock->vector_recursos_disponibles,cantidad_pokenest * sizeof(int));
	vector_T[cantidad_pokenest] = -1;
}

/*----------------------------------------------MANIPULACION ESTRUCTURAS-------------------------------------------------*/
int identificar_numero_fila(char* id_recurso_solicitado)
{
	int i=0;
	int encontrado = 0;
	int cantidad_pokenest = dictionary_size(mapa->pokeNests);
	while(i<cantidad_pokenest && !encontrado)
	{
		char* id = vector_auxiliar_identificadores_pokenest[i];
		if(string_equals_ignore_case(id,id_recurso_solicitado))
		{
			encontrado=1;
		}
		else
		{
			i++;
		}
	}

	return i;
}

void deadlock_agregar_nuevo_proceso_a_matrices(char* id_proceso)
{
	t_proceso *new_proceso = malloc(sizeof(t_proceso));
	new_proceso->id = id_proceso;
	new_proceso->posicion = indice_posicion_proceso;

	dictionary_put(procesos, id_proceso, new_proceso);
	char* indice_string = string_itoa(indice_posicion_proceso);
	dictionary_put(procesos_identificados_por_indice,indice_string,new_proceso);

	indice_posicion_proceso++;

	if(cantidad_columnas_actuales == cantidad_columnas_ocupadas)
	{
		cantidad_columnas_actuales++;
		cantidad_columnas_ocupadas++;
		asignar_nueva_columna_a_matriz(MATRIZ_ASIGNACION);
		asignar_nueva_columna_a_matriz(MATRIZ_SOLICITUD);
	}
	else
	{
		cantidad_columnas_ocupadas++;
		// YA HAY ESPACIO PARA LA COLUMNA Y SE CORRESPONDERÁ CON EL INDICE EN LA LISTA
	}
}

void asignar_nueva_columna_a_matriz(int matriz)
{
	/* ASIGNO ESPACIO PARA UNA SOLA COLUMNA A TODAS LAS FILAS A PARTIR DE LA ULTIMA COLUMNA*/
	switch(matriz)
	{
		case(MATRIZ_ASIGNACION):
		{
			int i;
			int cantidad_pokenest = dictionary_size(mapa->pokeNests);
			for(i=0;i<cantidad_pokenest;i++)
			{
				deadlock->matriz_asignacion[i] = realloc(deadlock->matriz_asignacion[i],cantidad_columnas_actuales*sizeof(int));
				deadlock->matriz_asignacion[i][cantidad_columnas_actuales-1] = 0;
			}
		};break;
		case(MATRIZ_SOLICITUD):
		{
			int i;
			int cantidad_pokenest = dictionary_size(mapa->pokeNests);
			for(i=0;i<cantidad_pokenest;i++)
			{
				deadlock->matriz_solicitud[i] = realloc(deadlock->matriz_solicitud[i],cantidad_columnas_actuales*sizeof(int));
				deadlock->matriz_solicitud[i][cantidad_columnas_actuales-1]=0;
			}
		};break;
	}
}

void deadlock_actualizar_matriz(char* id_proceso, char* id_recurso, int matriz,int tipo_actualizacion)
{
	int numero_fila_del_recurso = identificar_numero_fila(id_recurso);
	t_proceso* proceso = dictionary_get(procesos,id_proceso);

	switch(matriz)
	{
		case(MATRIZ_ASIGNACION):
		{
			switch(tipo_actualizacion)
			{
				case(OTORGAR_RECURSO):
				{
					deadlock->matriz_asignacion[numero_fila_del_recurso][proceso->posicion] = deadlock->matriz_asignacion[numero_fila_del_recurso][proceso->posicion] -1;
					deadlock->vector_recursos_disponibles[numero_fila_del_recurso] = deadlock->vector_recursos_disponibles[numero_fila_del_recurso] -1;
				};break;
				case(QUITAR_RECURSO):
				{
					deadlock->matriz_asignacion[numero_fila_del_recurso][proceso->posicion] = deadlock->matriz_asignacion[numero_fila_del_recurso][proceso->posicion] +1;
					deadlock->vector_recursos_disponibles[numero_fila_del_recurso] = deadlock->vector_recursos_disponibles[numero_fila_del_recurso] +1;
				};break;
			}
		};break;

		case(MATRIZ_SOLICITUD):
		{
			switch(tipo_actualizacion)
			{
				case(OTORGAR_RECURSO):
				{
					deadlock->matriz_solicitud[numero_fila_del_recurso][proceso->posicion] = deadlock->matriz_solicitud[numero_fila_del_recurso][proceso->posicion] +1;
				};break;
				case(QUITAR_RECURSO):
				{
					deadlock->matriz_solicitud[numero_fila_del_recurso][proceso->posicion] = deadlock->matriz_solicitud[numero_fila_del_recurso][proceso->posicion] -1;
				};break;
			}
		};break;
	}
}

void deadlock_elimina_proceso_de_matrices(char* id_proceso)
{
	/* POR CUESTIONES DE SIMPLICIDAD, NO SE ELIMINARA LA COLUMNA, SINO QUE SOLO SE MARCARÁ TODAS SUS CELDAS CON UN -1 */
	t_proceso* proceso = dictionary_get(procesos, id_proceso);

	int cantidad_pokenest = dictionary_size(mapa->pokeNests);

	/* SE SUMAN LOS RECURSOS QUE POSEÍA AL VECTOR DE DISPONIBLES */
	int recorrido_recursos_asignados;
	for(recorrido_recursos_asignados=0;recorrido_recursos_asignados<cantidad_pokenest;recorrido_recursos_asignados++)
	{
		deadlock->vector_recursos_disponibles[recorrido_recursos_asignados] = deadlock->matriz_solicitud[recorrido_recursos_asignados][proceso->posicion] + deadlock->vector_recursos_disponibles[recorrido_recursos_asignados];
	}

	/* SE LIMPIA LA MATRIZ DE SOLICITUD */
	int i;
	for(i=0;i<cantidad_pokenest;i++)
	{
	deadlock->matriz_solicitud[i][proceso->posicion] = -1;
	}

	/* SE LIMPIA LA MATRIZ DE ASIGNACION */
	int i2;
	for(i2=0;i2<cantidad_pokenest;i2++)
	{
		deadlock->matriz_asignacion[i2][proceso->posicion] = -1;
	}


}

void quitar_proceso_de_vectores(int numero_proceso)
{
	if(proceso_esta_marcado(numero_proceso))
	{

	}
	if(proceso_esta_en_vectorT(numero_proceso))
	{

	}
}

t_list* obtener_las_victimas()
{
	t_list *new_list = list_create();

	int i;
	for(i=0;i<cantidad_columnas_ocupadas;i++)
	{
		if(!proceso_esta_borrado(i) && !proceso_esta_marcado(i))
		{
			char* index_string = string_itoa(i);
			t_proceso *proceso = (t_proceso*) dictionary_get(procesos_identificados_por_indice,index_string);
			list_add(new_list,proceso->id);
		}
	}

	return new_list;
}

/*----------------------------------------------EJECUCION------------------------------------------------------------*/
void ejecutar_deadlock(void* arg)
{
	deadlock=deadlock_inicializate();
	deadlock_revisa();
}

void deadlock_revisa()
{
	while(1)
	{
		usleep(mapa->tiempo_chequeo_deadlock);

		marcar_procesos_que_no_tienen_recursos_asignados();
		iniciar_vectorT();

		int se_sigue_ejecutando = 1;

		while(se_sigue_ejecutando)
		{
			int se_encontro_proceso = marcar_proceso_si_se_puede_satisfacer();
			if(!se_encontro_proceso)
			{
				resolver_deadlock();
				se_sigue_ejecutando = 0;
			}
		}
		free(vector_T);
	}
}

void marcar_procesos_que_no_tienen_recursos_asignados()
{
	int i;
	/* SE RECORRE POR COLUMNA -> UNA COLUMNA = UN PROCESO */
	for(i=0;i<cantidad_columnas_ocupadas;i++)
	{
		if(!proceso_esta_borrado(i))
		{
			if(!proceso_tiene_recursos_asignados(i) && !proceso_esta_marcado(i) && proceso_tiene_solicitudes(i))
			{
				marcar_proceso(i);
			}
		}
	}
}

void marcar_proceso(int proceso)
{
	int tamanio_del_vector = tamanio_vector(vector_procesos_sin_recursos_asignados);
	vector_procesos_sin_recursos_asignados = realloc(vector_procesos_sin_recursos_asignados, (tamanio_del_vector+2)*sizeof(int));
	vector_procesos_sin_recursos_asignados[tamanio_del_vector] = proceso;
	vector_procesos_sin_recursos_asignados[tamanio_del_vector+1] = -1;
}

int marcar_proceso_si_se_puede_satisfacer()
{
	int i;
	int proceso_candidato_encontrado = 0;

	while(i<cantidad_columnas_ocupadas && !proceso_candidato_encontrado)
	{
		if(!proceso_esta_borrado(i) && !proceso_esta_marcado(i) )
		{
			if(proceso_puede_satisfacerce(i))
			{
				proceso_candidato_encontrado = 1;
			}
		}
		i++;
	}

	if(proceso_candidato_encontrado)
	{
		marcar_proceso(i-1);
		int *vector = recuperar_vector_proceso(i-1,MATRIZ_ASIGNACION);
		sumar_vector_a_vectorT(vector);
		free(vector);
		return 1;
	}
	else
	{
		return 0;
	}
}

void resolver_deadlock()
{
	t_list* victimas = obtener_las_victimas();
	resolver_deadlock_pokemon(victimas);
}

/*----------------------------------------------AUXILIARES-----------------------------------------------------------*/
int proceso_tiene_solicitudes(int numero_proceso)
{
	int i=0;
	int cantidad_elementos = dictionary_size(mapa->pokeNests);

	/* SE RECORRE POR COLUMNA -> UNA COLUMNA = UN PROCESO */
	int tiene_recursos = 0;
	while(i<cantidad_elementos && tiene_recursos<=0)
	{
		tiene_recursos = tiene_recursos + deadlock->matriz_solicitud[i][numero_proceso];
		i++;
	}
	if(tiene_recursos > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int proceso_puede_satisfacerce(int numero_proceso)
{
	int* vector_del_proceso = recuperar_vector_proceso(numero_proceso,MATRIZ_SOLICITUD);

	int resultado = vector_es_menor_igual_a_vectorT(vector_del_proceso);
	free(vector_del_proceso);

	return resultado;
}

int* recuperar_vector_proceso(int num_proceso, int matriz)
{
	int cantidad_elementos = dictionary_size(procesos);
	int* aux = malloc(sizeof(int) * (cantidad_elementos+1));
	aux[cantidad_elementos] = -1;

	switch(matriz)
	{
		case(MATRIZ_ASIGNACION):
		{
			int i;
			for(i=0;i<cantidad_elementos;i++)
			{
				aux[i]= deadlock->matriz_asignacion[i][num_proceso];
			}
		};break;
		case(MATRIZ_SOLICITUD):
		{
			int i;
			for(i=0;i<cantidad_elementos;i++)
			{
				aux[i]= deadlock->matriz_asignacion[i][num_proceso];
			}
		};break;
	}

	return aux;

}

int proceso_esta_borrado(int numero_proceso)
{
	int i=0;
	int cantidad_elementos = dictionary_size(mapa->pokeNests);

	/* SE RECORRE POR COLUMNA -> UNA COLUMNA = UN PROCESO */
	int esta_borrado = 0;
	while(i<cantidad_elementos && esta_borrado>=0)
	{
		esta_borrado = deadlock->matriz_asignacion[i][numero_proceso];
		i++;
	}
	if(esta_borrado<0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int proceso_tiene_recursos_asignados(int numero_proceso)
{
	int i=0;
	int cantidad_elementos = dictionary_size(mapa->pokeNests);

	/* SE RECORRE POR COLUMNA -> UNA COLUMNA = UN PROCESO */
	int tiene_recursos = 0;
	while(i<cantidad_elementos && tiene_recursos<=0)
	{
		tiene_recursos = tiene_recursos + deadlock->matriz_asignacion[i][numero_proceso];
		i++;
	}
	if(tiene_recursos > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int proceso_esta_marcado(int numero_proceso)
{
	int tamanio = tamanio_vector(vector_procesos_sin_recursos_asignados);
	int i=0;
	int encontrado = 0;

	while(i<tamanio && !encontrado)
	{
		if(vector_procesos_sin_recursos_asignados[i] == numero_proceso)
		{
			encontrado = 1;
		}
		i++;
	}
	return encontrado;
}

int proceso_esta_en_vectorT(int numero_proceso)
{
	int tamanio = tamanio_vector(vector_T);
	int i=0;
	int encontrado = 0;

	while(i<tamanio && !encontrado)
	{
		if(vector_T[i] == numero_proceso)
		{
				encontrado = 1;
		}
		i++;
	}
	return encontrado;
}

int tamanio_vector(int* vector)
{
	int i=0;
	int elementos = 0;
	while(vector[i] != -1)
	{
		elementos++;
		i++;
	}
	return elementos;
}

int vector_es_menor_igual_a_vectorT(int* vector)
{
	int tamanio_vector_t = tamanio_vector(vector_T);

	int i=0;
	int rompe_condicion = 0;

	while(i<tamanio_vector_t && !rompe_condicion)
	{
		if(vector[i] > vector_T[i])
		{
			rompe_condicion = 1;
		}
		i++;
	}
	return rompe_condicion;
}

void sumar_vector_a_vectorT(int* vector)
{
	int size = tamanio_vector(vector_T);
	int i;
	for(i=0;i<size;i++)
	{
		vector_T[i] = vector_T[i] + vector[i];
	}
}

