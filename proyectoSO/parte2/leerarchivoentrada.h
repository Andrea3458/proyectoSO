#ifndef LEERENTRADA_H
#define LEERENTRADA_H

#include "procesos.h"

extern Proceso *lista_procesos;
extern int capacidad;

int leer_archivo_ini(const char *arg);
void guardar_proceso(char *arg, int num);

#endif