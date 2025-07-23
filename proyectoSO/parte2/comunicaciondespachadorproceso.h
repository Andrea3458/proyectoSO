#ifndef MENSAJEPROCESO_H
#define MENSAJEPROCESO_H

#include "procesos.h"

typedef struct {
    Proceso proceso;
    int pipe_fd[2];
} MensajeProceso;

#endif