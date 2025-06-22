#include "./manipular_logs.h"
#include <time.h>
#include "stdint.h"
// cabecera
typedef struct {
    char nombre[32];
    int64_t bytes;
} pak_cabecera;



// obtener fecha y hora
void tiempo(char *fecha_hora) {
    time_t tiempo;
    struct tm *info_tiempo;
    
    time(&tiempo);
    info_tiempo = localtime(&tiempo);
    strftime(fecha_hora, sizeof(fecha_hora), "%Y%m%d-%H%M%S", info_tiempo);
}

// recibe la lista de archivos modificados
void empaquetar(const archivoHash** archivos, int num_archivos) {
    char fecha_hora[20];
    tiempo(fecha_hora);
    // crear el nombre del archivo en la ruta especificada y el formato de fecha-hora
    char nombre_pak[100];
    snprintf(nombre_pak, sizeof(nombre_pak), "/var/log/PROYECTO SO 1/logs %s.pak", fecha_hora);
    pak_cabecera cabecera;

    FILE *archivo_pak = fopen(nombre_pak, "wb");

    // Por cada archivo en la lista
    for (int i = 0; i < num_archivos; i++) {

        FILE* archivo = fopen(archivos[i]->nombre, "rb");

        fseek(archivo, 0, SEEK_END); // ir hasta el final del archivo
        int64_t tamano = ftell(archivo);
        fseek(archivo, 0, SEEK_SET); // volver al inicio

        // crear cabecera para el archivo actual
        strncpy(cabecera.nombre, archivos[i]->nombre, 31);
        cabecera.nombre[31] = '\0';
        cabecera.bytes = tamano;

        // escribir cabecera y contenido
        fwrite(&cabecera, sizeof(cabecera), 1, archivo_pak);
        size_t leidos;
        char buffer[1024];
        while ((leidos = fread(buffer, 1, 1024, archivo))) {
            fwrite(buffer, 1, leidos, archivo_pak);
        }

        fclose(archivo);
    }

    memset(&cabecera, 0, sizeof(cabecera));
    strcpy(cabecera.nombre, "FIN");
    cabecera.bytes = 0;
    fwrite(&cabecera, sizeof(cabecera), 1, archivo_pak);

    fclose(archivo_pak);
}

void comprimir_pak(const char* nombre) {

    pid_t pid = fork();

    if (pid == 0) {

        execlp("gzip", "gzip", "-f", nombre, NULL);

        // si se llega a esta linea, hubo un error
        exit(EXIT_FAILURE);

    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // comprimido correctamente
        } else {
            // error comprimir
        }

    } else if (pid < 0) {
        // error creando hijo
    }
}