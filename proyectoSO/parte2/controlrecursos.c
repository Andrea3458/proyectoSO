#include "controlrecursos.h"

// Función para adquirir recursos
int adquirir_recursos(Proceso* proc) {
    
    /*if(proc->num_impresoras > 2 || proc->num_modems > 1 || proc->num_scanners > 1 || proc->num_DVDs > 2){
        return 0;
    }*/
    // Intentar adquirir impresoras
    numImpresoras -= proc->num_impresoras;
    
    // Intentar adquirir scanner
    numScanner -= proc->num_scanners;
    
    // Intentar adquirir modem
    numModem -= proc->num_modems;
    
    // Intentar adquirir lectores DVD
    numLectoresDVD -= proc->num_DVDs;
    
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
}