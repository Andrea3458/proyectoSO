#ifndef RECURSOS_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define RECURSOS_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "procesos.h"

extern int numImpresoras, numScanner, numModem, numLectoresDVD;

int adquirir_recursos(Proceso* proc);
void liberar_recursos(Proceso* proc);

#endif