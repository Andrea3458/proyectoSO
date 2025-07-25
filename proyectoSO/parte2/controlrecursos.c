#include "controlrecursos.h"

// Función para adquirir recursos
int adquirir_recursos(Proceso* proc) {
    
    if(proc->num_impresoras > 2 || proc->num_modems > 1 || proc->num_scanners > 1 || proc->num_DVDs > 2){
        return 0;
    }
    // Intentar adquirir impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        numImpresoras--;
    }
    
    // Intentar adquirir scanner
    if(proc->num_scanners > 0) {
        numScanner--;
    }
    
    // Intentar adquirir modem
    if(proc->num_modems > 0) {
        numModem--;
    }
    
    // Intentar adquirir lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        numLectoresDVD--;
    }
    
    return numImpresoras >= 0 && numScanner >= 0 && numModem >= 0 && numLectoresDVD >= 0;
}

// Función para liberar recursos
void liberar_recursos(Proceso* proc) {
    
    // Liberar impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        numImpresoras++;
    }
    
    // Intentar adquirir scanner
    if(proc->num_scanners > 0) {
        numScanner++;
    }
    
    // Intentar adquirir modem
    if(proc->num_modems > 0) {
        numModem++;
    }
    
    // Intentar adquirir lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        numLectoresDVD++;
    }
}