/*
 * time.c
 *
 *  Created on: 14/9/2016
 *      Author: utnso
 */
#include "time.h"

long de_hora_a_ms(int hora){
	long aux = hora * 3.6 * 10 * 10 * 10 * 10 * 10 * 10;
	return(aux);
}

long de_min_a_ms(int minutos){
	long aux = minutos * 6 * 10 * 10 * 10;
	return(aux);
}

long de_seg_a_ms(int segundos){
	long aux = segundos * 1000;
	return aux;
}

long hora(char* tiempo){
	char* hora = string_new();
	hora = string_substring(tiempo,0,2);
	int horaint = atoi(hora);

	return ((long) horaint);
	free(hora);
}

long minutos(char* tiempo){
	char* minutos = string_new();
	minutos = string_substring(tiempo,3,2);
	int minutosint = atoi(minutos);
	return ((long) minutosint);
	free(minutos);
}

long segundos(char* tiempo){
	char* segundos = string_new();
	segundos = string_substring(tiempo,6,2);
	int segundosint = atoi(segundos);
	return ((long) segundosint);
	free(segundos);
}


long milisegundos(char* tiempo){
	char* milisegundos = string_new();
	milisegundos = string_substring(tiempo,9,3);
	int milisegundosint = atoi(milisegundos);
	return ((long) milisegundosint);
	free(milisegundos);
}

long tiempo_a_milisegundos(char* tiempo){
	long horax = hora(tiempo);
	long mshora = de_hora_a_ms(horax);
	long minutosx = minutos(tiempo);
	long msminutos = de_min_a_ms(minutosx);
	long segundosx = segundos(tiempo);
	long mssegundos = de_seg_a_ms(segundosx);
	long milisegundosx = milisegundos(tiempo);
	return (mshora+msminutos+mssegundos+milisegundosx);
}

long diferencia_de_tiempos(char* tiempoOrigen, char* tiempoFinal){
	long to= tiempo_a_milisegundos(tiempoOrigen);
	long tf=tiempo_a_milisegundos(tiempoFinal);
	return(tf-to);
}


