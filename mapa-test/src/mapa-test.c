/*
 ============================================================================
 Name        : mapa-test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "CUnit/Basic.h"
#include <unistd.h>
#include "cunitdeclarations.h"

int main(void) {
	CU_initialize_registry();

	  agregarTests();

	  CU_basic_set_mode(CU_BRM_VERBOSE);
	  CU_basic_run_tests();
	  CU_cleanup_registry();

	  return CU_get_error();
}
