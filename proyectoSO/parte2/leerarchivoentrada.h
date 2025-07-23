#ifndef LEERENTRADA_H
#define LEERENTRADA_H

#include "procesos.h"

extern Proceso *lista_procesos;
extern int capacidad;

void leer_archivo_ini(const char *arg, int num);
void guardar_proceso(char *arg, int num);

#endif