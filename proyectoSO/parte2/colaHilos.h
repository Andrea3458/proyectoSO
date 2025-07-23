#ifndef COLAHILOS_H
#define COLAHILOS_H

#include "procesos.h"

typedef struct {
    pthread_t *hilos;
    int capacidad_max;
    int tamano_actual;
    int inicio;
    int final;
} ColaHilos;

// Prototipos de funciones
ColaHilos* crear_Cola_Hilos(int capacidad);
void destruir_Cola_Hilos(ColaHilos *cola);
int is_emptyH(ColaHilos *cola);
int is_fullH(ColaHilos *cola);
void agregar_hilo(ColaHilos *cola, pthread_t p);
pthread_t eliminar_hilo(ColaHilos *cola);

typedef 