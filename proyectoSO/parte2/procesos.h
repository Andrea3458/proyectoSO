#ifndef PROCESOS_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define PROCESOS_H

#include <pthread.h>
#include <semaphore.h>

extern int segundo_actual, id_actual, cont_hilos_ejecucion, max_hilos_ejecucion, hay_proceso_en_ejecucion;
extern sem_t sem_ejecucion, sem_hilos_terminaron, sem_mutex;

typedef struct {
    int id;
    int tiempo_llegada;
    int prioridad;
    int tiempo_procesador; 
    int num_impresoras;
    int num_scanners;
    int num_modems;
    int num_DVDs;
    int tiempo_ejecutado;
} Proceso;

extern int estaEnColaDeUsuarios(Proceso p);
void registrar_mensajes(int segundo, int id_proceso, const char *estado);
void* ejecutar_proceso(void* arg);

#endif