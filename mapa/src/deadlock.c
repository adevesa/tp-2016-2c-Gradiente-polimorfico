/*
 * deadlock.c
 *
 *  Created on: 3/11/2016
 *      Author: utnso
 */



/*
 * ---------------------------------------------------------------------------BATALLAZA ANDROID VS IOS
 */

t_pokemon* conseguir_mejor_pokemon(t_entrenador* entrenador)
{
	t_pokemon* pokemon_a_tope = malloc (sizeof(t_pokemon*)); //Pokemon Bandera
	pokemon_a_tope->level=-1;

	int i;
	for(i=0;i<list_size(entrenador->pokemones_capturados);i++){
		t_pokemon* pokemon = malloc(sizeof(t_pokemon));
		pokemon = list_get(entrenador->pokemones_capturados,i);

		if (pokemon->nivel > pokemon_a_tope->nivel)	pokemon_a_tope = pokemon;
		else (free(pokemon));
		}

	return(pokemon_a_tope);
	free(pokemon_a_tope);
}

t_pokemon* obtener_pokemon_de_entrenador(t_list* pokemones_del_entrenador, int index)
{
	char* path_pokemon = list_get(pokemones_del_entrenador, index);
	t_config* config_greco = config_create(path_pokemon);
	int nivel = config_get_int_value(config_greco,"Nivel");
	t_pokemon* new_pokemon = malloc(sizeof(t_pokemon));
	new_pokemon->level = nivel;
	new_pokemon->species =

}

char* array_get_last_element(char* path)
{
	char **file_for_file = string_split(path, "/");
	int size = array_size(file_for_file);
	char *nombre = string_new();
	string_append(&nombre,file_for_file[size - 1]);
	array_free_all(file_for_file);
 	return nombre;

}

char* adaptar_nombre_pokemon(char* nombre_sucio)
{
	char* new_pokemon_adapter = malloc(string_length(nombre_sucio) -2);
	memcpy(new_pokemon_adapter, nombre_sucio, string_length(nombre_sucio) -3);
	new_pokemon_adapter = string_
}

 int son_iguales(char* a, char* b){
 	if(strcmp(a,b)==0)return 1;
 	else return 0;
 }

t_entrenador* ganador_batalla_pokemon(t_entrenador* atacante, t_entrenador* contricante){
	t_log* informe_batalla = log_create("Informe batallas","ganador_batalla_pokemon",0,LOG_LEVEL_INFO);

	log_info(loger,"Se van a enfrentar %s contra %s", atacante->nombre, contricante->nombre);


	t_pokemonx* el_mejorcito_del_atacante = conseguir_mejor_pokemon(atacante);
	t_pokemonx* el_mejorcito_del_contricante = conseguir_mejor_pokemon(contrincante);

	log_info(loger,"El mejor pokemon de %s es: %s", atacante->nombre, el_mejorcito_del_atacante->nombre);
	log_info(loger,"El mejor pokemon de %s es: %s", contricante->nombre, el_mejorcito_del_contrincante->nombre);

	t_pkmn_factory* pokemon_factory = create_pkmn_factory();

	t_pokemon* pokemon1 = create_pokemon(pokemon_factory, el_mejorcito_del_atacante->nombre, el_mejorcito_del_atacante->nivel);
	t_pokemon* pokemon2 = create_pokemon(pokemon_factory, el_mejorcito_del_contrincante->nombre, el_mejorcito_del_contrincante->nombre);

	log_info(loger,"El Pokemon de %s tiene un nivel de %d", atacante->nombre, el_mejorcito_del_atacante->nivel);
	log_info(loger,"Mientras que %s, su mejor pokemon es de nivel %d", contrincante->nombre, el_mejorcito_del_contrincante->nivel);

	log_info(loger,"B!B!B!Que empiece la batalla!!!");

	t_pokemon * loser = pkmn_battle(pokemon1, pokemon2);

	t_pokemonx pokemon_ganador = (son_iguales(loser->species, el_mejorcito_del_atacante->nombre))? el_mejorcito_del_contrincante->nombre : el_mejorcito_del_atacante->nombre);

	log_info(loger,"EL GANADOR ES: %s", pokemon_ganador) ;

	if(strcmp(loser->species, el_mejorcito_del_atacante->nombre) == 0) return contricante;
	else return atacante;

	free(pokemon1);
	free(pokemon2);
	destroy_pkmn_factory(pokemon_factory);
}
