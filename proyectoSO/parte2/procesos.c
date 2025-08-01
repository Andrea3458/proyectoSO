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
    
    int empezo = 0, suspendido = 0, termino = 0, seg_temp = -1;

    // CALCULAR TIEMPO INICIAL
    int tiempo_restante = proc->tiempo_procesador;

    for (int i = 0; i < 20; i++) {

        //Contandor hilos que esperan su tiempo
        sem_wait(sem_mutex2);
        cont_hilos_ejecucion++;

        // Evitar que el mismo proceso acapare el momento de otro proceso
        if(cont_hilos_ejecucion == max_hilos_ejecucion){
            sem_post(&sem_hilos_terminaron);
        }

        //while(seg_temp == segundo_actual){} //POSIBLE ESPERA ACTIVA
        sem_post(&sem_mutex2);

        sem_wait(&sem_ejecucion);

        sem_wait(&sem_mutex);
        
        printf("Hola soy %d ",proc->id);

        //Si el proceso está ejecutanto, no es su primera vez en ejecucion y todavia le queda tiempo en CPU entonces el proceso muestra
        if (id_actual == proc->id) { // Es mi turno de CPU

            if (tiempo_restante == 0) {
                printf("#%d END ", proc->id);
                //registrar_mensajes();
                termino = 1;
                hay_proceso_en_ejecucion = 0; // Solo si este era el proceso en ejecución
            } else if (!empezo) {
                printf("#%d BEGIN ", proc->id);
                //registrar_mensajes();
                empezo = 1;
            } else {
                printf("#%d EXECUTION ", proc->id);
                //registrar_mensajes();
            }
            tiempo_restante--;
            suspendido = 1; // Está en ejecución o acaba de ejecutar
        } else if (suspendido == 1) { // No es mi turno, y estuve ejecutando
            if (tiempo_restante == 0) {
                printf("#%d END ", proc->id);
                //registrar_mensajes();
                termino = 1;
                hay_proceso_en_ejecucion = 0; // Solo si este era el proceso en ejecución
            } else {
                printf("#%d SUSPENDED ", proc->id);
                //registrar_mensajes();
                suspendido = 0; // Ya no se encuentra en estado de "ejecución actual"
            }
        } 

        cont_hilos_ejecucion++;

        if(termino){
            max_hilos_ejecucion--;
            cont_hilos_ejecucion--;

            //Si no esta en cola de usuarios
            if(!estaEnColaDeUsuarios(*proc)){
                liberar_recursos(proc);
            }
        } else if(i == 19){
            max_hilos_ejecucion--;
            cont_hilos_ejecucion--;

            borrarProcesoDeAcuerdoACola(*proc);

            printf("#%d END ", proc->id);

            if(suspendido){
                hay_proceso_en_ejecucion = 0;
            }
        }

        seg_temp = segundo_actual;

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