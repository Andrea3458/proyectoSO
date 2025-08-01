#include "cola.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Crear una nueva cola
void crear_Cola(Cola *q, int capacidad) {
    q->procesos = (Proceso*)malloc(capacidad * sizeof(Proceso));
    if (!q->procesos) {
        free(q);
        perror("Error al asignar memoria para elementos de la cola");
        exit(EXIT_FAILURE);
    }
    
    q->capacidad_max = capacidad;
    q->tamano_actual = 0;
    q->inicio = 0;
    q->final = -1;
}

// Liberar memoria de la cola
void destruir_Cola(Cola *q) {
    if (q) {
        free(q->procesos);
        free(q);
    }
}

// Verificar si la cola está vacía
int is_empty(Cola *q) {
    return q->tamano_actual == 0;
}

// Verificar si la cola está llena
int is_full(Cola *q) {
    return q->tamano_actual == q->capacidad_max;
}

// Añadir un proceso al final de la cola
void agregar_proceso(Cola *q, Proceso p) {
    if (is_full(q)) {
        fprintf(stderr, "Error: Cola llena\n");
        return;
    }
    
    q->final = (q->final + 1) % q->capacidad_max;
    q->procesos[q->final] = p;
    q->tamano_actual++;
}

// Eliminar y devolver el proceso al frente de la cola
Proceso eliminar_proceso(Cola *q) {
    if (is_empty(q)) {
        //fprintf(stderr, "Error: Cola vacía\n");
        Proceso empty = {0};
        return empty;
    }
    
    Proceso p = q->procesos[q->inicio];
    q->inicio = (q->inicio + 1) % q->capacidad_max;
    q->tamano_actual--;
    
    return p;
}

// Verifica si un proceso esta cola de acuerdo al id
int esta_proceso_en_la_cola(Cola *q, Proceso p){
    
    if(is_empty(q)){
        return 0;
    }

    for(int i = 0; i < q->tamano_actual; i++){
        int auxP = (q->inicio + i) % q->capacidad_max;
        if(q->procesos[auxP].id == p.id){
            return 1;
        }
    }

    return 0;
}

// Elimina proceso de acuerdo a id
void eliminar_de_cola_proceso_especifico(Cola *q, Proceso p){

    if(is_empty(q)){
        return;
    }

    for(int i = 0; i < q->tamano_actual; i++){
        int auxP = (q->inicio + i) % q->capacidad_max;
        if(q->procesos[auxP].id == p.id){
            Proceso proceso_eliminado = q->procesos[auxP];

            for(int j = i; j < q->tamano_actual - 1; j++){
                int auxP_actual = (q->inicio + j) % q->capacidad_max;
                int auxP_sig = (q->inicio + j + 1) % q->capacidad_max;
                q->procesos[auxP_actual] = q->procesos[auxP_sig];
            }

            q->tamano_actual--;
            q->final = (q->final - 1 + q->capacidad_max) % q->capacidad_max;

            return;
        }
    }

    return;
}