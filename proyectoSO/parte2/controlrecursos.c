#include "controlrecursos.h"

// Función para adquirir recursos
int adquirir_recursos(Proceso* proc) {
    
    if(proc->num_impresoras > 2 || proc->num_modems > 1 || proc->num_scanners > 1 || proc->num_DVDs > 2){
        return 0;
    }
    // Intentar adquirir impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        num_impresoras--;
    }
    
    // Intentar adquirir scanner
    if(proc->num_scanners > 0) {
        num_scanners--;
    }
    
    // Intentar adquirir modem
    if(proc->num_modems > 0) {
        num_modems--;
    }
    
    // Intentar adquirir lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        num_DVDs--;
    }
    
    return numImpresoras >= 0 && num_scanners >= 0 && num_modems >= 0 && num_DVDs >= 0;
}

// Función para liberar recursos
void liberar_recursos(Proceso* proc) {
    
    // Liberar impresoras
    for(int i = 0; i < proc->num_impresoras; i++) {
        num_impresoras++;
    }
    
    // Intentar adquirir scanner
    if(proc->num_scanners > 0) {
        num_scanners++;
    }
    
    // Intentar adquirir modem
    if(proc->num_modems > 0) {
        num_modems++;
    }
    
    // Intentar adquirir lectores DVD
    for(int i = 0; i < proc->num_DVDs; i++) {
        num_DVDs++;
    }
}