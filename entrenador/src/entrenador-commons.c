/*
 * entrenador-commons.c
 *
 *  Created on: 13/9/2016
 *      Author: utnso
 */
#include "entrenador-commons.h"


sem_t turno_entrenador;
int matan_al_entrenador = 0;
//sem_t bloqueado;
//sem_t desbloqueado;

void ejecutar_entrenador(char *nombre_entrenador, char *ruta_pokedex)
{
	iniciar_log(nombre_entrenador);
	entrenador_registra_hora(INICIO);
	sem_init(&turno_entrenador,0,0);
	entrenador = entrenador_create(nombre_entrenador, ruta_pokedex);
	entrenador_iniciar_seniales();
	log_info(info_entrenador, "Entrenador creado con EXITO");
	entrenador_comenza_a_explorar();
	entrenador_registra_hora(FIN);
	mostrar_por_pantalla_resultados();
}

void mostrar_por_pantalla_resultados()
{
	printf("LOGRE TERMINAR MI AVENTURA \n");
	printf("-TIEMPO TOTAL AVENTURA %d \n", entrenador->tiempo_total_aventura);
	printf("-TIEMPO BLOQUEADO EN POKENEST: %d \n", entrenador->tiempo_bloqueado_pokenest);
	printf("-CANTIDAD DE DEADLOCKS: %d \n", entrenador->cantidad_deadlocks);
	printf("-CANTIDAD DE MUERTES: %d \n", entrenador->muertes);
}

/*void entrenador_comenza_aventura()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_t hilo_hoja_viaje;
	pthread_create(&hilo_hoja_viaje,&attr,entrenador_recorre_hoja_de_viaje,NULL);

	pthread_attr_destroy(&attr);
}*/

void entrenador_iniciar_seniales()
{
	signal(SIGUSR1, subirvida);
	signal(SIGTERM, bajarvida);
	signal(SIGKILL, matar_entrenador);
}

void subirvida(int n)
{
	entrenador->vidas++;
	log_info(info_entrenador,"RECIBI MAS VIDA POR SEÑAL EXTERNA");

}

void bajarvida(int n)
{
	if(n != 259)
	{
		log_info(info_entrenador,"PIERDO VIDA POR SEÑAL EXTERNA");
		if(entrenador->vidas == 0)
		{
			log_info(info_entrenador,"TENIA 0 VIDAS, POR LO QUE NO DISMINUYO");
		}
	}
	entrenador->vidas--;
}

void entrenador_finalizo_muriendo()
{
	printf("Intentos totales: %d", entrenador->reintentos);
	printf("GAME OVER. ¿Desea reintentar? Y/N \n");
	tratar_respuesta();
}

void tratar_respuesta()
{
	char resp;
	scanf("%c", &resp);
	if(resp == 'Y')
	{
		log_info(info_entrenador,"Se reintentara jugar");
		entrenador->reintentos = entrenador->reintentos +1;
		entrenador_borra_medallas();
		entrenador_borra_pokemons();
		entrenador_comenza_a_explorar();
	}
	else if(resp == 'N')
	{
		log_info(info_entrenador,"Se decidio no jugar mas.");
	}
	else{
		printf("Respuesta invalida\n");
		tratar_respuesta();
	}
}

void matar_entrenador(int n)
{
	matan_al_entrenador = 1;
	close(entrenador->mapa_actual->server);
	entrenador_borra_medallas();
	entrenador_borra_pokemons();
	exit(1);
}

void entrenador_borra_pokemons()
{
	char* directorio_de_pokemons = string_new();
	string_append(&directorio_de_pokemons, entrenador->ruta_pokedex);
	string_append(&directorio_de_pokemons,"/");
	string_append(&directorio_de_pokemons, "Entrenadores/");
	string_append(&directorio_de_pokemons, entrenador->nombre);
	string_append(&directorio_de_pokemons, "/Dir de Bill");
	borrar_todos_los_archivos_del_directorio(directorio_de_pokemons);
	free(directorio_de_pokemons);
}

void entrenador_borra_medallas()
{
	char* directorio_de_medallas = string_new();
	string_append(&directorio_de_medallas, entrenador->ruta_pokedex);
	string_append(&directorio_de_medallas,"/");
	string_append(&directorio_de_medallas, "Entrenadores/");
	string_append(&directorio_de_medallas, entrenador->nombre);
	string_append(&directorio_de_medallas, "/medallas");
	borrar_todos_los_archivos_del_directorio(directorio_de_medallas);
	free(directorio_de_medallas);
}

void iniciar_log(char *nombre_del_entrenador)
{
	char *nombre_log = string_new();
	string_append(&nombre_log, "Log ");
	string_append(&nombre_log, nombre_del_entrenador);
	info_entrenador =log_create(nombre_log, "Proceso entrenador", 1, LOG_LEVEL_INFO);

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
void entrenador_comenza_a_explorar()
{
	entrenador_recorre_hoja_de_viaje(NULL);
}

void entrenador_recorre_hoja_de_viaje(void* arg)
{
	int comienzo = (int) arg;
	int i;
	int cantidad = list_size(entrenador->hoja_de_viaje);
	int estoy_muerto = 0;

	if(comienzo == NULL) {i =0;log_info(info_entrenador, "COMIENZO del recorrido de la HOJA DE VIAJE");}
	else {i = comienzo; log_info(info_entrenador, "NUEVO INTENTO del recorrido de la HOJA DE VIAJE");}

	while(i<cantidad && !estoy_muerto)
	{
		entrenador_busca_mapa(i);
		conectar_a_mapa(entrenador->mapa_actual);
		enviar_mensaje_a_mapa(entrenador->mapa_actual,OTORGAR_SIMBOLO_ENTRENADOR, entrenador->simbolo);
		estoy_muerto=entrenador_cumpli_objetivos_del_mapa(i);
		i++;
	}

	switch(estoy_muerto)
	{
		case(MUERTO):
		{
			log_info(info_entrenador, "FIN por MUERTE");
			printf("No me quedaron vidas disponibles y morí en el mapa: %s", entrenador->mapa_actual->nombre);
			mapa_destruite(entrenador->mapa_actual);
			entrenador_finalizo_muriendo();
		};break;
		case(EXITO):
		{
			log_info(info_entrenador, "FIN del recorrido de la HOJA DE VIAJE");
			//entrenador_borra_medallas();
			//entrenador_borra_pokemons();
			//mapa_destruite(entrenador->mapa_actual);
		};break;
	}
}

/*void entrenador_recorre_este_mapa()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_t hilo_objetivos;
	pthread_create(&hilo_objetivos,&attr,entrenador_cumpli_objetivos_del_mapa,NULL);
	pthread_attr_destroy(&attr);
}*/

void entrenador_busca_mapa(int index)
{
	char *nombre_mapa = list_get(entrenador->hoja_de_viaje, index);
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
int entrenador_cumpli_objetivos_del_mapa(int index)
{
	//INICIO log
	char *mensaje = string_new();
	string_trim(&mensaje);
	string_append(&mensaje, "COMIENZO AVENTURA en ");
	string_append(&mensaje, entrenador->mapa_actual->nombre);
	log_info(info_entrenador,mensaje);
	free(mensaje);
	//FIN log

	int i=0;
	int cantidad_objetivos = list_size(entrenador->mapa_actual->objetivos);
	int estoy_muerto = 0;
	while(i<cantidad_objetivos  && !estoy_muerto)
	{
		entrenador_pedi_ubicacion_pokenest(i);
		estoy_muerto = entrenador_cumpli_objetivo(i);
		i++;
	}

	if(!estoy_muerto)
	{
		entrenador_termina_en_el_mapa();
		mapa_destruite(entrenador->mapa_actual);
		return 0;
	}
	else
	{
		if(me_quedan_vidas())
		{
			bajarvida(259);
			return entrenador_volve_a_empezar_en_este_mapa(index);
		}
		else
		{
			close(entrenador->mapa_actual->server);
			return 1;
		}
	}
}

int entrenador_volve_a_empezar_en_este_mapa(int index)
{
	borrar_todos_los_archivos_del_directorio(entrenador->directorio_de_bill); // <- solo debo borrar los pokemons de este mapa
	mapa_destruite(entrenador->mapa_actual);
	entrenador_busca_mapa(index);
	conectar_a_mapa(entrenador->mapa_actual);
	return entrenador_cumpli_objetivos_del_mapa(index);
}

void entrenador_borra_pokemos_del_mapa()
{
	int cantidad_pokemons = list_size(entrenador->mapa_actual->pokemons_capturados);
	int i=0;
	while(i<cantidad_pokemons)
	{
		char *element = list_get(entrenador->mapa_actual->pokemons_capturados,i);
		char *last_element = array_last_element(element);
		char *path = obtener_ruta_especifica(entrenador->directorio_de_bill,last_element,NULL);

		eliminar(path);
		free(last_element);
		free(path);
		i++;
	}
	list_clean(entrenador->mapa_actual->pokemons_capturados);
}

int me_quedan_vidas()
{
	if(entrenador->vidas > 0) return 1;
	else return 0;
}

void entrenador_termina_en_el_mapa()
{
	entrenador_copia_medalla_del_mapa();
	close(entrenador->mapa_actual->server);
	//entrenador_borra_pokemons();

		//INICIO log
		char *mensaje_fin = string_new();
		string_trim(&mensaje_fin);
		string_append(&mensaje_fin, "FIN AVENTURA EN ");
		string_append(&mensaje_fin, entrenador->mapa_actual->nombre);
		log_info(info_entrenador,mensaje_fin);
		free(mensaje_fin);
		//FIN log
}

void entrenador_espera_turno()
{
	char *turno =escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_DA_TURNO);
	if(string_equals_ignore_case(turno, "tr;"))
	{
		sem_post(&turno_entrenador);
	}
}

void entrenador_pedi_ubicacion_pokenest(int indice_objetivo)
{
	char *identificador_pokenest =list_get(entrenador->mapa_actual->objetivos,indice_objetivo);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,SOLICITAR_COORDENADAS_POKENEST ,identificador_pokenest);
	char *mapa_me_dice_que_me_envia = escuchar_mensaje_mapa(entrenador->mapa_actual,MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	int lo_que_me_da_mapa =  mapa_me_dice(mapa_me_dice_que_me_envia);
	free(mapa_me_dice_que_me_envia);
	if(lo_que_me_da_mapa == MAPA_ME_DA_TURNO)
	{
		char *mapa_me_dice_que_me_envia_2 = escuchar_mensaje_mapa(entrenador->mapa_actual,MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
		int lo_que_me_da_mapa_2 =  mapa_me_dice(mapa_me_dice_que_me_envia_2);
		free(mapa_me_dice_que_me_envia_2);
		switch(lo_que_me_da_mapa_2)
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

}

/*--------------------------------------------LOGICA DE CAMINAR HACIA POKENEST--------------------------------*/
int entrenador_cumpli_objetivo(int indice_obejtivo)
{
	while(!entrenador_llego_a_destino())
	{
		entrenador_espera_turno();
		sem_wait(&turno_entrenador);
		entrenador_ubicate_para_donde_caminar();
		log_info(info_entrenador, "Me moví");
		entrenador_informa_movimiento();
	}
	entrenador_espera_turno();
	sem_wait(&turno_entrenador);
	int resultado=entrenador_captura_pokemon(indice_obejtivo);
	return resultado;
}

void entrenador_informa_movimiento()
{
	char *posicion_actual = armar_coordenada(entrenador->ubicacion->x, entrenador->ubicacion->y, MAX_BYTES_COORDENADA);
	enviar_mensaje_a_mapa(entrenador->mapa_actual,REPORTAR_MOVIMIENTO, posicion_actual);
}

/*--------------------------------------------LOGICA DE ATRAPAR POKEMON-------------------------------------*/
int entrenador_captura_pokemon(int indice_objetivo)
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

	int respuesta_mapa = mapa_me_dice(solicitud);
	free(solicitud);
	switch(respuesta_mapa)
	{
		case(MAPA_ME_DA_POKEMON):
		{
			entrenador_recibi_y_copia_pokemon();
			char *hora_fin_desbloqueado = temporal_get_string_time();
			entrenador_registra_tiempo_bloqueo(hora_inicio_bloqueado, hora_fin_desbloqueado);
			return EXITO;
		};break;
		case(MAPA_ME_AVISA_DEADLOCK):
		{
			entrenador->cantidad_deadlocks = entrenador->cantidad_deadlocks +1;
			int resultado_deadlock = entrenador_trata_deadlock();
			if(resultado_deadlock == EXITO)
			{
				entrenador_recibi_y_copia_pokemon();
				char *hora_fin_desbloqueado = temporal_get_string_time();
				entrenador_registra_tiempo_bloqueo(hora_inicio_bloqueado, hora_fin_desbloqueado);
				return EXITO;
			}
			else return MUERTO;
		};break;
	}
}

void entrenador_recibi_y_copia_pokemon()
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
		list_add(entrenador->mapa_actual->pokemons_capturados, pokemon);

		//INICIO log
		char *mensaje_2 = string_new();
		string_trim(&mensaje_2);
		string_append(&mensaje_2, "Pokemon copiado a Dir de Bill");
		log_info(info_entrenador,mensaje_2);
		free(mensaje_2);
		//FIN log

}

void entrenador_registra_tiempo_bloqueo(char *hora_inicio, char *hora_fin)
{
	int tiempo = diferencia_de_tiempos(hora_inicio, hora_fin);
	entrenador->tiempo_bloqueado_pokenest = entrenador->tiempo_bloqueado_pokenest + tiempo;
}

/*void entrenador_espera_a_que_mapa_te_bloquee()
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
}*/

/*------------------------------------------LOGICA DE CAER EN DEADLOCK--------------------------------------------*/
int entrenador_trata_deadlock()
{
	entrenador_otorga_mejor_pokemon_a_mapa(entrenador->mapa_actual);
	char* respuesta = escuchar_mensaje_mapa(entrenador->mapa_actual, MAPA_ME_AVISA_QUE_ME_VA_A_ENVIAR);
	int caso_respuesta = mapa_me_dice(respuesta);
	free(respuesta);
	switch(caso_respuesta)
	{
		case(MAPA_ME_DICE_QUE_GANE):
		{
			return EXITO;
		}break;
		case(MAPA_ME_DICE_QUE_PERDI):
		{
			entrenador->muertes = entrenador->muertes +1;
			return MUERTO;
		}break;
	}
}

void entrenador_otorga_mejor_pokemon_a_mapa(t_mapa *mapa)
{
	t_pokemon *mejor_pokemon = entrenador_busca_mejor_pokemon();
	char *mejor_pokemon_serialized = armar_mejor_pokemon_string(mejor_pokemon);
	enviar_mensaje_a_mapa(mapa,ENTREGAR_MEJOR_POKEMON, mejor_pokemon_serialized);
	free(mejor_pokemon_serialized);
	free(mejor_pokemon->species);
	free(mejor_pokemon);
}

/*------------------------------------------LOGICA DE TERMINAR EN EL MAPA----------------------------------------*/
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

	char *ruta_medallas_destino = obtener_ruta_especifica(entrenador->ruta_pokedex, "Entrenadores", entrenador->nombre);
	ruta_medallas_destino = obtener_ruta_especifica(ruta_medallas_destino,"medallas",NULL);
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

/*------------------------------------------BUSCAR MEJOR POKEMON-------------------------------------------------*/
t_pokemon* entrenador_busca_mejor_pokemon()
{
	t_list *pokemons_names = nombre_de_archivos_del_directorio(entrenador->directorio_de_bill);
	t_list *pokemos = recuperar_pokemons(pokemons_names);
	list_destroy_and_destroy_elements(pokemons_names,destroy_path);

	t_pokemon* pokemon_a_tope = malloc (sizeof(t_pokemon*)); //Pokemon Bandera
	pokemon_a_tope->level=-1;

	int i;
	int size = list_size(pokemos);
	for(i=0;i<size;i++)
	{
		t_pokemon *pokemon = list_get(pokemos,i);
		if (pokemon->level > pokemon_a_tope->level)	pokemon_a_tope = pokemon;
	}
	list_destroy_and_destroy_elements(pokemos,pokemon_destroy);
	return pokemon_a_tope;
}

void destroy_path(void *arg)
{
	char *path = (char*) arg;
	free(path);
}

void pokemon_destroy(void *arg)
{
	t_pokemon *pokemon = (t_pokemon*) arg;
	free(pokemon->species);
	free(pokemon);
}

t_list* recuperar_pokemons(t_list *lista_nombres_pokemons)
{
	int size = list_size(lista_nombres_pokemons);
	t_list *list_pokemons = list_create();

	int i;
	for(i=0; i<size; i++)
	{
		char *name_file = list_get(lista_nombres_pokemons, i);
		t_pokemon *new_pokemon = recuperar_pokemon(name_file);
		list_add(list_pokemons, new_pokemon);
	}
	return list_pokemons;
}

t_pokemon* recuperar_pokemon(char *nombre_file_pokemon)
{
	char *aux = obtener_ruta_especifica(entrenador->directorio_de_bill, nombre_file_pokemon,NULL);
	t_config *config_aux = config_create(aux);
	int nivel = config_get_int_value(config_aux,"Nivel");
	t_pokemon* new_pokemon = malloc(sizeof(t_pokemon));
	new_pokemon->level = nivel;
	new_pokemon->species = adaptar_nombre_pokemon(nombre_file_pokemon);
	config_destroy(config_aux);
	free(aux);
	return new_pokemon;
}

char* adaptar_nombre_pokemon(char* nombre_sucio)
{
	char* new_pokemon_adapter = malloc(string_length(nombre_sucio) -2);
	memcpy(new_pokemon_adapter, nombre_sucio, string_length(nombre_sucio) -3);
	toupper(new_pokemon_adapter[0]);
	return new_pokemon_adapter;
}

/*------------------------------------------AUX --> LS -----------------------------------------------------------*/
t_list* nombre_de_archivos_del_directorio(char *ruta)
{

	/* Variables */
		DIR *dirp;
		struct dirent *direntp;

	/* Abrimos el directorio */
		dirp = opendir(ruta);
		if (dirp == NULL){
			printf("Error: No se puede abrir el directorio\n");
			exit(1);
		}
		t_list *lista = list_create();
		while ((direntp = readdir(dirp)) != NULL) {
			 if(!string_equals_ignore_case(direntp->d_name, ".")){
				  if(!string_equals_ignore_case(direntp->d_name, "..")){
					  if(!string_equals_ignore_case(direntp->d_name, "metadata")){
						 list_add(lista, direntp->d_name);
					  	 }
				  }
			 }
		}
		 	 return lista;
		 	 /* Cerramos el directorio */
		 	 closedir(dirp);
	}
