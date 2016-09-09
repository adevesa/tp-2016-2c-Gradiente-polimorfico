/*
 * cunitdeclarations.c
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#include "cunitdeclarations.h"


// *COMIENZO* DEFINICION DE INICILIZADOR Y LIMPIADOR//
int init_suite()
{
	return 0;
}

int clean_suite()
{
	return 0;
}

// *FIN* DEFINICION DE INICIALIZADOR Y LIMPIADOR//

void agregarTests()
{
	CU_pSuite suite = CU_add_suite("Suite mapa", init_suite, clean_suite);

	CU_add_test(suite, "mapa_reconoce su nombre",mapa_reconoce_nombre);
	CU_add_test(suite, "mapa reconoce su ruta pokedex", mapa_reconoce_rutaPokedex);
	CU_add_test(suite, "mapa reconoce su algoritmo completo", mapa_reconoce_algoritmo_completo);
	CU_add_test(suite, "mapa reconoce su ip", mapa_reconoce_ip);
	CU_add_test(suite, "mapa reconoce su puerto", mapa_reconoce_puerto);
	CU_add_test(suite, "mapa reconoce su batalla", mapa_reconoce_tiempo_batalla);
	CU_add_test(suite, "mapa reconoce su tiempo de deadlock", mapa_reconoce_tiempo_deadlock);


}

