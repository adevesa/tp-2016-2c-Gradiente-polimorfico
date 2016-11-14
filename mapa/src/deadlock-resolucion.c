/*
 * deadlock-resolucion.c
 *
 *  Created on: 11/11/2016
 *      Author: utnso
 */
#include "deadlock-resolucion.h"

extern t_log* logger;
extern int semaforo_rr_cambiado_por_deadlock;
extern int semaforo_srdf_cambiado_por_deadlock;

void resolver_deadlock_pokemon(t_list* involucrados)
{

	int index_entrenador = 0;

	if(mapa->batalla)
	{
		log_info(logger, "SE COMIENZA A RESOLVER EL DEADLOCK");
		t_list *entrenadores_involucrados = recuperar_entrenadores_involucrados(involucrados);
		list_destroy(involucrados);
		ordenar_entrenadores_segun_tiempo_de_ingreso(entrenadores_involucrados);

		t_entrenador* entrenador_1 = (t_entrenador*) list_get(entrenadores_involucrados,index_entrenador);
				index_entrenador++;

		t_entrenador* entrenador_2 = (t_entrenador*) list_get(entrenadores_involucrados,index_entrenador);
		index_entrenador++;

		char* mensaje_a_log = string_new();
		string_append(&mensaje_a_log,"LOS ENTRENADORES ELEGIDOS PARA LA BATALLA SON: ");
		string_append(&mensaje_a_log,entrenador_1->simbolo_identificador);
		string_append(&mensaje_a_log," Y: ");
		string_append(&mensaje_a_log,entrenador_2->simbolo_identificador);
		log_info(logger,mensaje_a_log);
		free(mensaje_a_log);


		t_perdedor *perdedor;
		perdedor = efectuar_batalla_pokemon_entre(entrenador_1,entrenador_2);

		char* mensaje_a_log_2 = string_new();
		string_append(&mensaje_a_log_2,"EL PERDEDOR FUE: ");
		string_append(&mensaje_a_log_2,perdedor->victima->simbolo_identificador);
		log_info(logger, mensaje_a_log_2);
		free(mensaje_a_log_2);

		if(perdedor->caso == POR_DESCONEXION)
		{
			planificador_desbloqueame_a(perdedor->victima);
			planificador_aborta_entrenador(perdedor->victima);
			cambiar_semaforos_si_es_necesario();
		}
		else
		{
			int hay_victima_por_desconexion = 0;
			int size = list_size(entrenadores_involucrados);

			/* SE COMIENZA LA BATALLA ANIDADA HASTA QUE ALGUIEN SE DESCONECTE O HASTA QUE ALGUNO PIERDA DEFINITIVAMENTE */
			while(index_entrenador<size && !hay_victima_por_desconexion)
			{
				t_entrenador* siguiente_entrenador = (t_entrenador*) list_get(entrenadores_involucrados,index_entrenador);
				index_entrenador++;

				char* mensaje_a_log = string_new();
				string_append(&mensaje_a_log,"SE HARÁ OTRA BATALLA Y LOS LOS ENTRENADORES ELEGIDOS SON: ");
				string_append(&mensaje_a_log,siguiente_entrenador->simbolo_identificador);
				string_append(&mensaje_a_log," Y ");
				string_append(&mensaje_a_log,perdedor->victima->simbolo_identificador);
				log_info(logger,mensaje_a_log);
				free(mensaje_a_log);

				perdedor = efectuar_batalla_pokemon_con_el_anterior_perdedor(perdedor,siguiente_entrenador);
				if(perdedor->caso == POR_DESCONEXION)
				{
					hay_victima_por_desconexion = 1;
				}

				char* mensaje_a_log_2 = string_new();
				string_append(&mensaje_a_log_2,"EL PERDEDOR FUE: ");
				string_append(&mensaje_a_log_2,perdedor->victima->simbolo_identificador);
				log_info(logger, mensaje_a_log_2);
				free(mensaje_a_log_2);
			}
			log_info(logger,perdedor->victima->simbolo_identificador);
			enviar_mensaje_a_entrenador(perdedor->victima,AVISAR_QUE_PERDIO,NULL);
			planificador_desbloqueame_a(perdedor->victima);
			planificador_aborta_entrenador(perdedor->victima);
			cambiar_semaforos_si_es_necesario();
		}
	}
}

void cambiar_semaforos_si_es_necesario()
{
	if(mapa_decime_si_planificador_es(PLANIFICADOR_RR))
	{
		semaforo_rr_cambiado_por_deadlock = 1;
		planificador_rr_cambia_semaforo_si_es_necesario();
	}
	else
	{
		semaforo_srdf_cambiado_por_deadlock = 1;
		planificador_srdf_cambia_semaforo_si_es_necesario();
	}
}

t_perdedor* efectuar_batalla_pokemon_con_el_anterior_perdedor(t_perdedor* anterior_perdedor, t_entrenador *entrenador)
{
	enviar_mensaje_a_entrenador(entrenador,AVISAR_DEADLOCK,NULL);
	void* respuesta_entrenador = escuchar_mejor_pokemon(entrenador->socket_entrenador);

	t_perdedor* new_perdedor = malloc(sizeof(t_perdedor));

	if(string_equals_ignore_case((char*) respuesta_entrenador, "DESCONECTADO"))
	{
		enviar_mensaje_a_entrenador(anterior_perdedor->victima,AVISAR_QUE_GANO,NULL);
		new_perdedor->caso = POR_DESCONEXION;
		new_perdedor->victima = anterior_perdedor->victima;
		new_perdedor->pokemon_de_la_victima =anterior_perdedor->pokemon_de_la_victima;
		//free(anterior_perdedor);
		return new_perdedor;
	}
	else
	{
		t_pokemon *pokemon_entrenador = (t_pokemon*) respuesta_entrenador;
		t_pokemon *pokemon_perdedor = pkmn_battle(anterior_perdedor->pokemon_de_la_victima,pokemon_entrenador);
		int resultado = retornar_perdedor(pokemon_perdedor,anterior_perdedor->pokemon_de_la_victima, pokemon_entrenador);
		if(resultado == VICTIMA_ENTRENADOR_1)
		{
			enviar_mensaje_a_entrenador(entrenador,AVISAR_QUE_GANO,NULL);
			new_perdedor->caso = POR_BATALLA;
			new_perdedor->victima = anterior_perdedor->victima;
			new_perdedor->pokemon_de_la_victima = anterior_perdedor->pokemon_de_la_victima;
			//free(anterior_perdedor);
			destroy_pokemon(pokemon_entrenador);
			return new_perdedor;
		}
		else
		{
			enviar_mensaje_a_entrenador(anterior_perdedor->victima,AVISAR_QUE_GANO,NULL);
			//destroy_pokemon(anterior_perdedor->pokemon_de_la_victima);
			new_perdedor->caso = POR_BATALLA;
			new_perdedor->victima= entrenador;
			new_perdedor->pokemon_de_la_victima = pokemon_entrenador;
			//free(anterior_perdedor);
			return new_perdedor;
		}
	}
}

t_perdedor* efectuar_batalla_pokemon_entre(t_entrenador *entrenador_1, t_entrenador *entrenador_2)
{
	enviar_mensaje_a_entrenador(entrenador_1,AVISAR_DEADLOCK,NULL);
	void* respuesta_entrenador_1 = escuchar_mejor_pokemon(entrenador_1->socket_entrenador);

	enviar_mensaje_a_entrenador(entrenador_2,AVISAR_DEADLOCK,NULL);
	void* respuesta_entrenador_2 = escuchar_mejor_pokemon(entrenador_2->socket_entrenador);

	t_perdedor* new_perdedor = malloc(sizeof(t_perdedor));

	if(string_equals_ignore_case((char*) respuesta_entrenador_1, "DESCONECTADO"))
	{
		enviar_mensaje_a_entrenador(entrenador_2,AVISAR_QUE_GANO,NULL);
		new_perdedor->caso = POR_DESCONEXION;
		new_perdedor->victima = entrenador_1;
		return new_perdedor;
	}
	else
	{
		if(string_equals_ignore_case((char*) respuesta_entrenador_2, "DESCONECTADO"))
		{
			enviar_mensaje_a_entrenador(entrenador_1,AVISAR_QUE_GANO,NULL);
			new_perdedor->caso = POR_DESCONEXION;
			new_perdedor->victima = entrenador_2;
			return new_perdedor;
		}
		else
		{
			/* NINGNO SE DESCONECTO! HACEMOS LA BATALLA */
			t_pokemon *pokemon_entrenador_1 = (t_pokemon*) respuesta_entrenador_1;
			t_pokemon *pokemon_entrenador_2 = (t_pokemon*) respuesta_entrenador_1;

			t_pokemon *pokemon_perdedor = pkmn_battle(pokemon_entrenador_1,pokemon_entrenador_2);

			int perdedor = retornar_perdedor(pokemon_perdedor,pokemon_entrenador_1,pokemon_entrenador_2);

			if(perdedor == VICTIMA_ENTRENADOR_1)
			{
				enviar_mensaje_a_entrenador(entrenador_2,AVISAR_QUE_GANO,NULL);
				new_perdedor->caso = POR_BATALLA;
				new_perdedor->victima = entrenador_1;
				new_perdedor->pokemon_de_la_victima = pokemon_entrenador_1;
				destroy_pokemon(pokemon_entrenador_2);
			}
			else
			{
				enviar_mensaje_a_entrenador(entrenador_1,AVISAR_QUE_GANO,NULL);
				new_perdedor->caso = POR_BATALLA;
				new_perdedor->victima = entrenador_2;
				new_perdedor->pokemon_de_la_victima = pokemon_entrenador_2;
				destroy_pokemon(pokemon_entrenador_1);
			}
			return new_perdedor;
		}
	}
}

void destroy_pokemon(t_pokemon *pokemon)
{
	free(pokemon->species);
	free(pokemon);
}

int retornar_perdedor(t_pokemon *perdedor, t_pokemon *pokemon_entrenador_1, t_pokemon *pokemon_entrenador_2)
{
	if(pokemones_son_iguales(perdedor,pokemon_entrenador_1))
	{
		return VICTIMA_ENTRENADOR_1;
	}
	else
	{
		return VICTIMA_ENTRENADOR_2;
	}

}

int pokemones_son_iguales(t_pokemon *pok_1, t_pokemon *pok_2)
{
	if(pok_1->level == pok_2->level && pok_1->second_type== pok_2->second_type && string_equals_ignore_case(pok_1->species,pok_2->species) && pok_1->type == pok_2->type)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

t_list* recuperar_entrenadores_involucrados(t_list* identificadores_involucrados)
{
	int size = list_size(identificadores_involucrados);
	t_list* new_list = list_create();
	int i;
	for(i=0;i<size;i++)
	{
		char* id = list_get(identificadores_involucrados,i);
		t_entrenador *entrenador = (t_entrenador*) dictionary_get(mapa->diccionario_de_entrenadores,id);
		list_add(new_list, entrenador);

		char* mensaje_a_log = string_new();
		string_append(&mensaje_a_log,"EL ENTRENADOR ");
		string_append(&mensaje_a_log,entrenador->simbolo_identificador);
		string_append(&mensaje_a_log," SE ENCUENTRA EN DEADLOCK");
		log_info(logger,mensaje_a_log);
		free(mensaje_a_log);
	}
	return new_list;
}

void ordenar_entrenadores_segun_tiempo_de_ingreso(t_list* entrenadores)
{
	list_sort(entrenadores,comparador_para_batalla);
}

int comparador_para_batalla(void* elem1, void* elem2)
{
	t_entrenador *entrenador_1 = (t_entrenador*) elem1;
	t_entrenador *entrenador_2=(t_entrenador*) elem2;

	if(entrenador_1->numero_de_ingreso > entrenador_2->numero_de_ingreso)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
