#ifndef RECURSOS_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define RECURSOS_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "procesos.h"
#include "cola.h"

extern sem_t sem_mutex, sem_mutex2, sem_hilos_terminaron, sem_ejecucion; 
extern int numImpresoras, numScanner, numModem, numLectoresDVD, cont_hilos_ejecucion, max_hilos_ejecucion, segundo_actual, hay_proceso_en_ejecucion;
extern Cola tiempo_real, usuario, prioridad[3];
extern Proceso proc_first;

int adquirir_recursos(Proceso* proc);
void liberar_recursos(Proceso* proc);
int esta_en_cola_de_usuarios(Proceso p);
void borrar_proceso_de_acuerdo_a_cola(Proceso p);
void control_semaforos();


#endif