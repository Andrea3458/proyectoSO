#ifndef VERIFICAR_LOGS_H  
#define VERIFICAR_LOGS_H 

#include "leerLogs.h"
#include <string.h>

void Verificarlogs(const char *nombre, const char *md5_actual);

#endif

const int BUFFER_SIZE = 256;

// EL STRUCT GUARDA LOS HASHES A MANERA DE DICCIONARIO NOMBRE:HASH
typedef struct {

    char nombre[BUFFER_SIZE];
    char resultadoMd5[33];

} archivoHash;

archivoHash *hashesPrevios = NULL;
int contador = 0;

void Verificarlogs(const char *nombre, const char *md5_actual) {

    char *hash_previo = "0";
    int es_nuevo = 1;

    // Por cada hash (cambio nuevo)
    for (int i = 0; i < contador; i++) {
        if (strcmp(hashesPrevios[i].nombre, nombre) == 0) { // si el archivo ya ha sido guardado previamente
            hash_previo = hashesPrevios[i].resultadoMd5;
            es_nuevo = 0; // el archivo no es nuevo, por lo tanto tiene un resultado anterior que comparar
            break;
        }
    }

    if (strcmp(hash_previo, "0") == 0) { // si es la primera verificacion o el archivo es nuevo
        es_nuevo = 1;
        archivoHash *tmp = realloc(hashesPrevios, (contador + 1)*sizeof(archivoHash));

        hashesPrevios = tmp;

        strncpy(hashesPrevios[contador].nombre, nombre, 32);
        strncpy(hashesPrevios[contador].resultadoMd5, md5_actual, 32);

        contador++;
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*

    } else if (strcmp(md5_actual, hash_previo) != 0) { // si el archivo ha tenido cambios
        for (int j = 0; j < contador; j++) {
            if (strcmp(hashesPrevios[j].nombre, nombre) == 0) {
                strncpy(hashesPrevios[j].resultadoMd5, md5_actual, 32);
                break;
            }
        }
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*
    }

}