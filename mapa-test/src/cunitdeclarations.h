/*
 * cunitdeclarations.h
 *
 *  Created on: 9/9/2016
 *      Author: utnso
 */

#ifndef TEST_MAPA_SRC_CUNITDECLARATIONS_H_
#define TEST_MAPA_SRC_CUNITDECLARATIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include <unistd.h>
// *Acá se debe incluir el .h que contiene los tests creados por el programmer*
#include "mapa-testing.h"

int init_suite();

int clean_suite();

void agregarTests();


#endif /* TEST_MAPA_SRC_CUNITDECLARATIONS_H_ */
