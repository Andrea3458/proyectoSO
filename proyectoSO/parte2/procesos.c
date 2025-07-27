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

        if(termino){
            break;
        }

        // Evitar que el mismo acapare el momento de otro proceso
        while(seg_temp == segundo_actual) {} //SOLUCIONAR POSIBLE ESPERA ACTIVA
        sem_wait(&sem_ejecucion);
        //printf("Hola soy %d\n",proc->id);

        //Si el proceso está ejecutanto, no es su primera vez en ejecucion y todavia le queda tiempo en CPU entonces el proceso muestra
        if (id_actual == proc->id && empezo && tiempo_restante > 0 && tiempo_restante > 0) {
            
            printf("#%d EXECUTION ", proc->id);
            //registrar mensaje();
            tiempo_restante--;
            suspendido = 1;
        
        //Si el proceso se va a ejecutar y todavia no ha empezado entonces...
        } else if (id_actual == proc->id && !empezo) {

            printf("#%d BEGIN ", proc->id);
            // registrar mensaje();
            empezo = 1;
            tiempo_restante--;
            suspendido = 1;

        //Si ha cambiado el id y todavia le queda tiempo en cpu el proceso muestra que se suspende
        } else if (id_actual != proc->id && tiempo_restante > 0 && suspendido == 1) {

            printf("#%d SUSPENDED ", proc->id); 
            //registrar mensaje();
            tiempo_restante--;
            suspendido = 0;

        } else if (tiempo_restante == 0) {
            // Mensaje de fin
            if(id_actual == proc->id){
                hay_proceso_en_ejecucion = 0;
            }

            printf("#%d END ", proc->id);
            termino = 1;
        }
        
        //Contandor hilos que esperan su tiempo

        seg_temp = segundo_actual;
        cont_hilos_ejecucion++;
        if(cont_hilos_ejecucion == max_hilos_ejecucion){
            sem_post(&sem_hilos_terminaron);
        }
        sem_post(&sem_ejecucion);
    }

    max_hilos_ejecucion--;

    if(estaEnColaDeUsuarios(*proc)){
        liberar_recursos(proc);
    }
    
    free(proc);
        
    // Liberar recursos definitivamente
    pthread_exit(NULL);
}