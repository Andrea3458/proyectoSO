#include "procesos.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "control.h"
#include "cola.h"

extern Cola prioridad[3];
int segundo_anterior = -1;

void registrar_mensajes (const char *mensaje) {
    char mensaje_log[256];
    FILE *archivo = fopen("30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        perror("Error al abrir archivo de log");
        return;
    }
    if(segundo_anterior < 0){
        fprintf(archivo, "Segundo %d: ", segundo_actual);
        fputs(mensaje, archivo);
    }
    else if(segundo_actual != segundo_anterior && segundo_anterior >= 0){
        fputs("\n", archivo);
        fprintf(archivo, "Segundo %d: ", segundo_actual);
        fputs(mensaje, archivo);
    }
    else fputs(mensaje, archivo);
    
    segundo_anterior = segundo_actual;
    
    fclose(archivo);
}

void* ejecutar_proceso(void* arg) {

    Proceso *proc = (Proceso*)arg;
    
    int empezo = 0, suspendido = 0, termino = 0;
    char mensaje_log[256];

    // CALCULAR TIEMPO INICIAL
    int tiempo_restante = proc->tiempo_procesador;

    for (int i = 0; i < 20; i++) {

        //Contandor hilos que esperan su tiempo
        sem_wait(&sem_mutex2);
        cont_hilos_ejecucion++;

        // Evitar que el mismo proceso acapare el momento de otro proceso
        if(cont_hilos_ejecucion == max_hilos_ejecucion){
            sem_post(&sem_hilos_terminaron);
        }

        sem_post(&sem_mutex2);

        sem_wait(&sem_ejecucion);

        sem_wait(&sem_mutex);
        
        //printf("Hola soy %d ",proc->id);

        //Si el proceso está ejecutanto, no es su primera vez en ejecucion y todavia le queda tiempo en CPU entonces el proceso muestra
        if (id_actual == proc->id) { // Es mi turno de CPU

            if (tiempo_restante == 0) {
                printf("#%d END ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d END ", proc->id);
                registrar_mensajes(mensaje_log);
                termino = 1;
                hay_proceso_en_ejecucion = 0; // Solo si este era el proceso en ejecución
            } else if (!empezo) {
                printf("#%d BEGIN ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d BEGIN ", proc->id);
                registrar_mensajes(mensaje_log);
                empezo = 1;
            } else {
                printf("#%d EXECUTION ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d EXECUTION ", proc->id);
                registrar_mensajes(mensaje_log);
            }
            tiempo_restante--;
            suspendido = 1; // Está en ejecución o acaba de ejecutar
        } else if (suspendido == 1) { // No es mi turno, y estuve ejecutando
            if (tiempo_restante == 0) {
                printf("#%d END ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d END ", proc->id);
                registrar_mensajes(mensaje_log);
                termino = 1;
                hay_proceso_en_ejecucion = 0; // Solo si este era el proceso en ejecución
            } else {
                printf("#%d SUSPENDED ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d SUSPENDED ", proc->id);
                registrar_mensajes(mensaje_log);
                suspendido = 0; // Ya no se encuentra en estado de "ejecución actual"
            }
        } 

        cont_hilos_ejecucion++;

        if(termino || i == 19){
            max_hilos_ejecucion--;
            cont_hilos_ejecucion--;

            //Si no esta en cola de usuarios
            if(!esta_en_cola_de_usuarios(*proc)){
                liberar_recursos(proc);
            }

            if(i == 19){
                borrar_proceso_de_acuerdo_a_cola(*proc);
                
                printf("#%d END ", proc->id);
                snprintf(mensaje_log, sizeof(mensaje_log), "#%d END ", proc->id);
                registrar_mensajes(mensaje_log);
                if(suspendido){
                    hay_proceso_en_ejecucion = 0;
                }
                termino = 1;
            }

        }

        if(cont_hilos_ejecucion == max_hilos_ejecucion){
            sem_post(&sem_hilos_terminaron);
        }
        if(termino){
            sem_post(&sem_mutex);
            break;
        }
        
        sem_post(&sem_mutex);
    }

    // Liberar recursos definitivamente
    free(proc);
    pthread_exit(NULL);
}