// queue.h
#ifndef COLA_H
#define COLA_H

#include "procesos.h"

typedef struct {
    Proceso *procesos;
    int capacidad_max;
    int tamano_actual;
    int inicio;
    int final;
} Cola;

// Prototipos de funciones
void crear_Cola(Cola *cola, int capacidad);
void destruir_Cola(Cola *cola);
int is_empty(Cola *cola);
int is_full(Cola *cola);
void agregar_proceso(Cola *cola, Proceso p);
Proceso eliminar_proceso(Cola *cola);
Proceso ver_frente(Cola *q);
int esta_proceso_en_la_cola(Cola *q, Proceso p);

#endif