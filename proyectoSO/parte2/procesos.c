#include "procesos.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "controlrecursos.h"
#include "cola.h"

extern Cola prioridad[3];

void registrar_mensajes (int segundo, int id_proceso, const char *estado) {

    FILE *archivo = fopen("30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        perror("Error al abrir archivo de log");
        return;
    }

    char mensaje[256];
    snprintf(mensaje, sizeof(mensaje), "Segundo %d: #%d %s\n", segundo, id_proceso, estado);

    printf("%s", mensaje);

    fprintf(archivo, "%s", mensaje);
    fclose(archivo);
}

void* ejecutar_proceso(void* arg) {

    Proceso *proc = (Proceso*)arg;
    
    int empezo = 0, suspendido = 0;

    // CALCULAR TIEMPO INICIAL
    int tiempo_restante = proc->tiempo_procesador;
    int quantum;

    // Determinar quantum según prioridad
    switch(proc->prioridad) {
        case 1: quantum = 3; break;
        case 2: quantum = 2; break;
        case 3: quantum = 1; break;
        default: quantum = proc->tiempo_procesador; // Tiempo Real ejecuta completo
    }
    
    int tiempo_ejecucion = (tiempo_restante < quantum) ? tiempo_restante : quantum;

    for (int i = segundo_actual; i <= segundo_actual+20; i++) {

        // REGION CRITICA
        sem_wait(&sem_ejecucion);

        if (id_actual == proc->id && !empezo) {
            // RECUERDA ACTUALIZAR EL SEGFUNDO ACTUAL EN ALGUN MOMENTO

            printf("Segundo %d: #%d BEGIN\n", segundo_actual, proc->id);
            // registrar mensaje();
            empezo = 1;
            segundo_actual++;
            tiempo_restante -= tiempo_ejecucion;
            continue; // VERIFICAR

        } else if (id_actual == proc->id && empezo && tiempo_restante > 0 && suspendido == 1) {
            // VERIFICAR SI EL PROCESOS SE SUSPENDIO
            

            printf("Segundo %d: #%d EXECUTION\n", proc->tiempo_llegada + proc->tiempo_ejecutado, proc->id);

            proc->tiempo_ejecutado++;
            tiempo_restante -= tiempo_ejecucion;
            segundo_actual++;
            continue;
        
        } else if (id_actual != proc->id && tiempo_restante > 0 && proc->prioridad > 0) {

            // Si no ha terminado y es proceso de usuario (no tiempo real)
            printf("#%d SUSPENDED ", proc->id); 
                
            // Bajar prioridad si es posible
            // if(proc->prioridad < 3 && is_full(&prioridad[proc->prioridad-1])) {
            //     // Volver a encolar
            //     proc->prioridad++;
            // }
            agregar_proceso(&prioridad[proc->prioridad-1], *proc);

            // Liberar recursos temporalmente
            liberar_recursos(proc);
            //pthread_mutex_unlock(&mutex_contador);
            continue;

        } else if (tiempo_restante == 0) {
            // Mensaje de fin
            printf("Segundo %d: #%d END\n", proc->tiempo_llegada + proc->tiempo_ejecutado, proc->id);
            pthread_exit(NULL);
            break;
        }
        
        //Contandor hilos que esperan su tiempo
        cont_hilos_ejecucion++;

        if(cont_hilos_ejecucion == max_hilos_ejecucion){
            sem_post(&sem_hilos_terminaron);
        }
    }

    max_hilos_ejecucion--;
        
    // Liberar recursos definitivamente
    liberar_recursos(proc);
    free(proc);
}