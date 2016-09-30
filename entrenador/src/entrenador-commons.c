/*
 * entrenador-commons.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "entrenador-commons.h"


//sem_t turno_entrenador;
//sem_t bloqueado;
//sem_t desbloqueado;


void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex)
{
	iniciar_log(nombre_entrenador);
	entrenador_registra_hora(INICIO);
	entrenador = entrenador_create(nombre_entrenador, ruta_pokedex);
	log_info(info_entrenador, "Entrenador creado con EXITO");
	entrenador_recorre_hoja_de_viaje();
	entrenador_registra_hora(FIN);
	//SI ESTA ACA, ES QUE YA TERMINO DE RECORRER
}


void iniciar_log(char *nombre_del_entrenador)
{
	char *nombre_log = string_new();
	string_append(&nombre_log, "Log ");
	string_append(&nombre_log, nombre_del_entrenador);
	info_entrenador =log_create(nombre_log, "Proceso entrenador", 0, LOG_LEVEL_INFO);

	free(nombre_log);
}

void entrenador_registra_hora(int rango)
{
	switch(rango)
	{
	case(INICIO):
			{
				hora_de_inicio = temporal_get_string_time();

				char *mensaje=string_new();
				string_append(&mensaje, "COMIENZO: ");
				string_append(&mensaje, hora_de_inicio);
				log_info(info_entrenador,mensaje);
				free(mensaje);

			}break;
	case(FIN):
			{
				hora_de_fin = temporal_get_string_time();

				char *mensaje=string_new();
				string_append(&mensaje, "FIN: ");
				string_append(&mensaje, hora_de_fin);
				log_info(info_entrenador,mensaje);
				free(mensaje);

				entrenador->tiempo_total_aventura = (int) diferencia_de_tiempos(hora_de_inicio,hora_de_fin);

				char *mensaje_2=string_new();
				string_append(&mensaje_2, "DURACION TOTAL: ");
				string_append(&mensaje_2, string_itoa(entrenador->tiempo_total_aventura));
				log_info(info_entrenador,mensaje_2);
				free(mensaje_2);

			}break;
	}

}

/*--------------------------------------------LOGICA DE CUMPLIR LOS OBJETIVOS DE TODOS LOS MAPAS---------------------------------*/
void entrenador_recorre_hoja_de_viaje()
{
	log_info(info_entrenador, "COMIENZO del recorrido de la HOJA DE VIAJE");

	int i;
	int cantidad = list_size(entrenador->hoja_de_viaje);
	for(i = 0; i<cantidad ; i++)
	{
		entrenador_busca_mapa(i);
		conectar_a_mapa(entrenador->mapa_actual);
		enviar_mensaje_a_mapa(entrenador->mapa_actual,OTORGAR_SIMBOLO_ENTRENADOR, entrenador->simbolo);
		entrenador_cumpli_objetivos_del_mapa();
	}

	log_info(info_entrenador, "FIN del recorrido de la HOJA DE VIAJE");
}

void entrenador_busca_mapa(int index)
{
	char *nombre_mapa = list_get(entrenador->hoja_de_viaje, index);
	string_trim_left(&nombre_mapa);
	entrenador->mapa_actual = mapa_create(nombre_mapa, entrenador->ruta_pokedex, entrenador);

	//INICIO log
	char *mensaje = string_new();
	string_append(&mensaje, "Mapa actual: ");
	string_append(&mensaje, entrenador->mapa_actual->nombre);
	log_info(info_entrenador,mensaje);
	free(mensaje);
	//FIN log
}

/*--------------------------------------------LOGICA DE CUMPLIR LOS OBJETIVOS DE UN MAPA---------------------------------*/
void entrenador_cumpli_objetivos_del_mapa()
{
	//INICIO log
	char *mensaje = string_new();
	string_trim(&mensaje);
	string_append(&mensaje, "COMIENZO AVENTURA en ");
	string_append(&mensaje, entrenador->mapa_actual->nombre);
	log_info(info_entrenador,mensaje);
	free(mensaje);
	//FIN log

	int i;
	int cantidad_objetivos = list_size(entrenador->mapa_actual->objetivos);
	for(i=0; i<cantidad_objetivos; i++)
	{
		//entrenador_espera_turno();
		//em_wait(&turno_entrenador);
		entrenador_pedi_ubicacion_pokenest(i);
		entrenador_cumpli_objetivo(i);
	}
	//entrenador_espera_turno();
	//sem_wait(&turno_entrenador);
	//entrenador_avisa_que_terminaste_en_este_mapa();

	entrenador_copia_medalla_del_mapa();
	close(entrenador->mapa_actual->server);

	//INICIO log
	char *mensaje_fin = string_new();
	string_trim(&mensaje_fin);
	string_append(&mensaje_fin, "FIN AVENTURA EN ");
	string_append(&mensaje_fin, entrenador->mapa_actual->nombre);
	log_info(info_entrenador,mensaje_fin);
	free(mensaje_fin);
	//FIN log

	mapa_destruite(entrenador->mapa_actual);
}

void entrenador_espera_turno()
{
	char *turno =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_TURNO);
	if(string_equals_ignore_case(turno, "tr;"))
	{
		//INICIO log
		char *mensaje = string_new();
		string_trim(&mensaje);
		string_append(&mensaje, entrenador->mapa_actual->nombre);
		string_append(&mensaje, " me concede TURNO");
		log_info(info_entrenador,mensaje);
		free(mensaje);
		//FIN log

		//sem_post(&turno_entrenador);
	}
}

void entrenador_pedi_ubicacion_pokenest(int indice_objetivo)
{
	char *identificador_pokenest =list_get(entrenador->mapa_actual->objetivos,indice_objetivo);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,SOLICITAR_COORDENADAS_POKENEST ,identificador_pokenest);
	char *mapa_me_dice_que_me_envia = escuchar_mensaje_mapa(entrenador->mapa_actual,MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	int lo_que_me_da_mapa =  mapa_me_dice(mapa_me_dice_que_me_envia);
	switch(lo_que_me_da_mapa)
	{
		case(MAPA_ME_DA_COORDENADAS_POKENEST):
			{
				char *coordenada = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_COORDENADAS_POKENEST);
				t_ubicacion *ubicacion_pokenest = desarmar_coordenada(coordenada);
				entrenador->pokenest = ubicacion_pokenest;
			};break;
	}


		//INICIO log
		char *mensaje = string_new();
		string_trim(&mensaje);
		string_append(&mensaje, entrenador->mapa_actual->nombre);
		string_append(&mensaje, " me da COORDENADAS de pokenest ");
		string_append(&mensaje, identificador_pokenest);
		log_info(info_entrenador,mensaje);
		free(mensaje);
		//FIN log

}

/*--------------------------------------------LOGICA DE CAMINAR HACIA POKENEST--------------------------------*/
void entrenador_cumpli_objetivo(int indice_obejtivo)
{
	while(!entrenador_llego_a_destino())
	{
		//entrenador_espera_turno();
		//sem_wait(&turno_entrenador);
		entrenador_ubicate_para_donde_caminar();
		log_info(info_entrenador, "Me moví");
		entrenador_informa_movimiento();
	}
	//entrenador_espera_turno();
	//sem_wait(&turno_entrenador);
	entrenador_captura_pokemon(indice_obejtivo);
}

void entrenador_informa_movimiento()
{
	char *posicion_actual = armar_coordenada(entrenador->ubicacion->x, entrenador->ubicacion->y, MAX_BYTES_COORDENADA);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,REPORTAR_MOVIMIENTO, posicion_actual);
}

/*--------------------------------------------LOGICA DE ATRAPAR POKEMON-------------------------------------*/
void entrenador_captura_pokemon(int indice_objetivo)
{
	//INICIO log
	char *mensaje = string_new();
	string_trim(&mensaje);
	string_append(&mensaje, "Llegué a destino. Pediré CAPTURAR POKEMON");
	log_info(info_entrenador,mensaje);
	free(mensaje);
	//FIN log

	enviar_mensaje_a_mapa(entrenador->mapa_actual,SOLICITAR_CAPTURA_POKEMON,NULL);
	char *hora_inicio_bloqueado = temporal_get_string_time();
	char *solicitud = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	entrenador_recibi_y_copia_pokemon(solicitud);
	char *hora_fin_desbloqueado = temporal_get_string_time();
	entrenador_registra_tiempo_bloqueo(hora_inicio_bloqueado, hora_fin_desbloqueado);


	//MAPA ME BLOQUEA//
	//entrenador_espera_a_que_mapa_te_bloquee();
	//sem_wait(&bloqueado);
	//entrenador->cantidad_de_veces_bloqueado++;


	//ESPERO QUE MAPA ME DESBLOQUEE//
	//entrenador_espera_a_que_mapa_te_desbloquee();
	//sem_wait(&desbloqueado);
}

void entrenador_recibi_y_copia_pokemon(char *solicitud)
{

	if(MAPA_ME_DA_POKEMON == mapa_me_dice(solicitud))
	{
		char *pokemon = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_POKEMON);

		//INICIO log
		char *mensaje = string_new();
		string_append(&mensaje, entrenador->mapa_actual->nombre);
		string_append(&mensaje, " me dice que el pokemon está en: ");
		string_append(&mensaje, pokemon);
		log_info(info_entrenador,mensaje);
		free(mensaje);
		//FIN log

		copiar(pokemon,entrenador->directorio_de_bill);

		//INICIO log
		char *mensaje_2 = string_new();
		string_trim(&mensaje_2);
		string_append(&mensaje_2, "Pokemon copiado a Dir de Bill");
		log_info(info_entrenador,mensaje_2);
		free(mensaje_2);
		//FIN log
	}
}

void entrenador_registra_tiempo_bloqueo(char *hora_inicio, char *hora_fin)
{
	int tiempo = diferencia_de_tiempos(hora_inicio, hora_fin);
	entrenador->tiempo_bloqueado_pokenest = entrenador->tiempo_bloqueado_pokenest + tiempo;
}

void entrenador_espera_a_que_mapa_te_bloquee()
{
	char *bloq =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_BLOQUEA);
	if(MAPA_ME_BLOQUEA == mapa_me_dice(bloq))
	{
		//INICIO log
		char *mensaje = string_new();
		string_trim(&mensaje);
		string_append(&mensaje, entrenador->mapa_actual->nombre);
		string_append(&mensaje, " me BLOQUEO");
		log_info(info_entrenador,mensaje);
		free(mensaje);
		//FIN log

		//sem_post(&bloqueado);
	}
}

void entrenador_espera_a_que_mapa_te_desbloquee()
{
	char *bloq =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_BLOQUEA);
	if(MAPA_ME_DESBLOQUEA == mapa_me_dice(bloq))
		{
			//INICIO log
			char *mensaje = string_new();
			string_trim(&mensaje);
			string_append(&mensaje, entrenador->mapa_actual->nombre);
			string_append(&mensaje, " me DESBLOQUEO");
			log_info(info_entrenador,mensaje);
			free(mensaje);
			//FIN log

			//sem_post(&desbloqueado);
		}
}
/*------------------------------------------LOGICA DE TERMINAR EN EL MAPA------------------------------------*/
void entrenador_avisa_que_terminaste_en_este_mapa()
{
	enviar_mensaje_a_mapa(entrenador->mapa_actual, REPORTAR_FIN_OBJETIVOS, NULL);
	char *esperar_confirmacion_de_medalla = escuchar_mensaje_mapa(entrenador->mapa_actual,MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	if(string_equals_ignore_case(esperar_confirmacion_de_medalla, "mr;"))
	{
		char *medalla_del_mapa = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_MEDALLA);
		char *ruta_medallas = obtener_ruta_especifica(entrenador->directorio_de_bill, "medallas", NULL);
		string_append(&ruta_medallas,"/");
		copiar(medalla_del_mapa, ruta_medallas);
		free(ruta_medallas);
	}
	else { perror("No se puede interpretar mensaje"); }
}

void entrenador_copia_medalla_del_mapa()
{
	char *ruta_medalla_origen = obtener_ruta_especifica(entrenador->ruta_pokedex, "Mapas", entrenador->mapa_actual->nombre);
	char *aux = string_new();
	string_append(&aux,"medalla-");
	string_append(&aux,entrenador->mapa_actual->nombre);
	string_append(&aux,".jpg");
	ruta_medalla_origen = obtener_ruta_especifica(ruta_medalla_origen,aux,NULL);
	string_path_replace_spaces(ruta_medalla_origen," ", "\\");

	char *ruta_medallas_destino = obtener_ruta_especifica(entrenador->directorio_de_bill, "medallas", NULL);
	string_append(&ruta_medallas_destino,"/");
	copiar(ruta_medalla_origen,ruta_medallas_destino);

	free(aux);
	free(ruta_medalla_origen);
	free(ruta_medallas_destino);

	//INICIO log
	char *mensaje = string_new();
	string_trim(&mensaje);
	string_append(&mensaje, "TERMINE objetivos en ");
	string_append(&mensaje, entrenador->mapa_actual->nombre);
	log_info(info_entrenador,mensaje);
	free(mensaje);
	//FIN log
}
