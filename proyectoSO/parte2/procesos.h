#ifndef PROCESOS_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define PROCESOS_H

#include <pthread.h>

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

void registrar_mensajes(int segundo, int id_proceso, const char *estado);
void* ejecutar_proceso(void* arg);

#endif