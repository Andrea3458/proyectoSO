#include "colaHilos.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

ColaHilos* crear_Cola_Hilos(int capacidad) {
    ColaHilos *q = (ColaHilos*)malloc(sizeof(ColaHilos));
    if (!q) {
        perror("Error al crear la cola");
        exit(EXIT_FAILURE);
    }
    
    q->hilos = (pthread_t*)malloc(capacidad * sizeof(pthread_t));
    if (!q->hilos) {
        free(q);
        perror("Error al asignar memoria para elementos de la cola");
        exit(EXIT_FAILURE);
    }
    
    q->capacidad_max = capacidad;
    q->tamano_actual = 0;
    q->inicio = 0;
    q->final = -1;
    
    return q;
}

// Liberar memoria de la cola
void destruir_Cola_Hilos(ColaHilos *q) {
    if (q) {
        free(q->hilos);
        free(q);
    }
}

// Verificar si la cola está vacía
int is_emptyH(ColaHilos *q) {
    return q->tamano_actual == 0;
}

// Verificar si la cola está llena
int is_fullH(ColaHilos *q) {
    return q->tamano_actual == q->capacidad_max;
}

// Añadir un proceso al final de la cola
void agregar_hilo(ColaHilos *q, pthread_t h) {
    if (is_fullH(q)) {
        fprintf(stderr, "Error: Cola llena\n");
        return;
    }
    
    q->final = (q->final + 1) % q->capacidad_max;
    q->hilos[q->final] = h;
    q->tamano_actual++;
}

// Eliminar y devolver el proceso al frente de la cola
pthread_t eliminar_hilo(ColaHilos *q) {
    if (is_emptyH(q)) {
        fprintf(stderr, "Error: Cola vacía\n");
        pthread_t empty = {0};
        return empty;
    }
    
    pthread_t h = q->hilos[q->inicio];
    q->inicio = (q->inicio + 1) % q->capacidad_max;
    q->tamano_actual--;

    pthread
    
    return h;
}