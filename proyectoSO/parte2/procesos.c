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
        
    int tiempo_restante = proc->tiempo_procesador;
    int quantum;

    // Determinar quantum según prioridad
    switch(proc->prioridad) {
        case 1: quantum = 3; break;
        case 2: quantum = 2; break;
        case 3: quantum = 1; break;
        default: quantum = proc->tiempo_procesador; // Tiempo Real ejecuta completo
    }

    printf("Segundo %d: #%d BEGIN\n", proc->tiempo_llegada, proc->id);
    // registrar mensaje();
    
    while(tiempo_restante > 0) {
        int tiempo_ejecucion = (tiempo_restante < quantum) ? tiempo_restante : quantum;
        
        // Ejecutar por el tiempo determinado
        for(int i = 0; i < tiempo_ejecucion; i++) {
            printf("Segundo %d: #%d EXECUTION\n", proc->tiempo_llegada + proc->tiempo_ejecutado, proc->id);
            proc->tiempo_ejecutado++;
            sleep(1); // Simula 1 segundo de ejecución
        }
        
        tiempo_restante -= tiempo_ejecucion;
        
        // Si no ha terminado y es proceso de usuario (no tiempo real)
        if(tiempo_restante > 0 && proc->prioridad > 0) {
            printf("Segundo %d: #%d SUSPENDED\n", proc->tiempo_llegada + proc->tiempo_ejecutado, proc->id);
            
            // Bajar prioridad si es posible
            if(proc->prioridad < 3) {
                proc->prioridad++;
            }
            
            // Volver a encolar
            agregar_proceso(&prioridad[proc->prioridad-1], *proc);
            
            // Liberar recursos temporalmente
            liberar_recursos(proc);
            
            pthread_exit(NULL);
        }
    }
    
    // Mensaje de fin
    printf("Segundo %d: #%d END\n", proc->tiempo_llegada + proc->tiempo_ejecutado, proc->id);
    
    // Liberar recursos definitivamente
    liberar_recursos(proc);
    free(proc);

}