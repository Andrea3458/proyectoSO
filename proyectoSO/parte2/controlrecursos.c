#include "controlrecursos.h"

// Función para adquirir recursos
int adquirir_recursos(Proceso* proc) {
    // Para procesos de tiempo real (no necesitan recursos)
    if(proc->prioridad == 0) return 1;
    
    // Intentar adquirir impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        sem_wait(&impresora);
    }
    
    // Intentar adquirir scanner
    if(proc->num_scanners > 0) {
        sem_wait(&scanner);
    }
    
    // Intentar adquirir modem
    if(proc->num_modems > 0) {
        sem_wait(&modem);
    }
    
    // Intentar adquirir lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        sem_wait(&lectoresDVD);
    }
    
    return 1;
}

// Función para liberar recursos
void liberar_recursos(Proceso* proc) {
    // Para procesos de tiempo real (no usan recursos)
    if(proc->prioridad == 0) return;
    
    // Liberar impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        sem_post(&impresora);
    }
    
    // Liberar scanner
    if(proc->num_scanners > 0) {
        sem_post(&scanner);
    }
    
    // Liberar modem
    if(proc->num_modems > 0) {
        sem_post(&modem);
    }
    
    // Liberar lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        sem_post(&lectoresDVD);
    }
}