#ifndef EMPAQUETAR_ARCHIVOS H
#define EMPAQUETAR_ARCHIVOS H

#include "./manipular_logs.h"
#include "stdint.h"
// cabecera
typedef struct {
    char nombre[32];
    int64_t bytes;
} pak_cabecera;



// obtener fecha y hora
void tiempo(char* fecha_hora);

// recibe la lista de archivos modificados
void empaquetar(const archivoHash** archivos, int num_archivos, char* nombre_pak);

void comprimir_pak(const char* nombre);

#endif