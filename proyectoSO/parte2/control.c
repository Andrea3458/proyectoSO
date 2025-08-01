#include "control.h"

// Función para adquirir recursos
int adquirir_recursos(Proceso* proc) {
    
    // Intentar adquirir impresoras
    numImpresoras -= proc->num_impresoras;
    
    // Intentar adquirir scanner
    numScanner -= proc->num_scanners;
    
    // Intentar adquirir modem
    numModem -= proc->num_modems;
    
    // Intentar adquirir lectores DVD
    numLectoresDVD -= proc->num_DVDs;

    //printf("PEDIDO ID: %d, nI: %d, nS: %d, nM: %d, nL: %d\n",proc->id,numImpresoras,numScanner,numModem,numLectoresDVD);
    
    return numImpresoras >= 0 && numScanner >= 0 && numModem >= 0 && numLectoresDVD >= 0;
}

// Función para liberar recursos
void liberar_recursos(Proceso* proc) {
    
    // Liberar impresoras
    numImpresoras += proc->num_impresoras;
    
    // Intentar adquirir scanner
    numScanner += proc->num_scanners;
    
    // Intentar adquirir modem
    numModem += proc->num_modems;
    
    // Intentar adquirir lectores DVD
    numLectoresDVD += proc->num_DVDs;

    //printf("LIBERADO ID: %d, nI: %d, nS: %d, nM: %d, nL: %d\n",proc->id,numImpresoras,numScanner,numModem,numLectoresDVD);
}

int esta_en_cola_de_usuarios(Proceso p){
    return esta_proceso_en_la_cola(&usuario, p);
}

void borrar_proceso_de_acuerdo_a_cola(Proceso p){
    
    if(esta_proceso_en_la_cola(&tiempo_real, p)){
        eliminar_de_cola_proceso_especifico(&tiempo_real, p);
        return;
    }

    if(esta_proceso_en_la_cola(&usuario, p)){
        eliminar_de_cola_proceso_especifico(&usuario, p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[0], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[0], p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[1], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[1], p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[2], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[2], p);
        return;
    }
}

void control_semaforos(){
    sem_post(&sem_mutex2);
    //Espera a que los hilos entren al for
    if(proc_first.tiempo_llegada <= segundo_actual){
        sem_wait(&sem_hilos_terminaron);
    }

    cont_hilos_ejecucion = 0;

    sem_wait(&sem_mutex);
    for(int i = 0; i < max_hilos_ejecucion; i++){
        sem_post(&sem_ejecucion);
    }

    sem_wait(&sem_mutex2);
    if(proc_first.tiempo_llegada <= segundo_actual){
        //Registrar mensaje
        printf("Segundo %d: ", segundo_actual);
        sem_post(&sem_mutex);
        sem_wait(&sem_hilos_terminaron);
    } else {
        sem_post(&sem_mutex);
    }

    sem_wait(&sem_mutex);
    cont_hilos_ejecucion = 0;

    if(proc_first.tiempo_llegada <= segundo_actual){
        printf("\n");
    }
    segundo_actual++;
    sem_post(&sem_mutex);
}