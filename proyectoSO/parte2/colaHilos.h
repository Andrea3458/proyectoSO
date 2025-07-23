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

typedef 