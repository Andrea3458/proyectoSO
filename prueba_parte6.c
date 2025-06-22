#include <time.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>   
#include <dirent.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <unistd.h>  
#include <string.h> 
#include "stdint.h"
#include <errno.h>

#define BUFFER_SIZE 512

typedef struct {

    char nombre[BUFFER_SIZE];
    char resultadoMd5[33];

} archivoHash;


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
    for(int i=0; i<num_archivos; i++){
       printf("Nombre: %s, Indice: %d\n", (*archivos)[i].nombre, i);
    }
    
    char fecha_hora[20];
    tiempo(fecha_hora);
    printf("El tiempo actual %s\n", fecha_hora);
    // crear el nombre del archivo en la ruta especificada y el formato de fecha-hora
    char nombre_pak[100];
    snprintf(nombre_pak, sizeof(nombre_pak), "/var/log/PROYECTO SO 1/logs%s.pak", fecha_hora);
    pak_cabecera cabecera;
    printf("el nombre del pack: %s\n", nombre_pak);

    FILE *archivo_pak = fopen(nombre_pak, "wb");
    if(archivo_pak == NULL){
        printf("el puntero es nulo 1\n");
    }
    char path[512] = "/var/log/";

    // Por cada archivo en la lista
    for (int i = 0; i < num_archivos; i++) {
        printf("Nombre: %s, Indice: %d\n", (*archivos)[i].nombre, i);
        char path[512] = "/var/log/";
        strcat(path, (*archivos)[i].nombre);
        printf("Path: %s\n", path);
        FILE* archivo = fopen(path, "rb");
        if(archivo == NULL){
            printf("el puntero es nulo 2\n");
        }

        fseek(archivo, 0, SEEK_END); // ir hasta el final del archivo
        printf("Paso 1\n");
        int64_t tamano = ftell(archivo);
        fseek(archivo, 0, SEEK_SET); // volver al inicio
        printf("Paso 2\n");
        // crear cabecera para el archivo actual
        strncpy(cabecera.nombre, (*archivos)[i].nombre, 31);
        cabecera.nombre[31] = '\0';
        cabecera.bytes = tamano;

        // escribir cabecera y contenido
        size_t bits_escritos = fwrite(&cabecera, sizeof(pak_cabecera), 1, archivo_pak);
        printf("Paso 3\n");
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

int main(){

    char fecha_hora[20];
    char* directorio = "/var/log/PROYECTO SO 1";

    if(mkdir(directorio, 0755) == 0){
		syslog(LOG_INFO, "Se creo exitosamente el directorio PROYECTO SO 1.");
	}
	else{
		if(errno == EEXIST){
			syslog(LOG_ERR, "Error: el directorio PROYECTO SO 1 ya existe.");
		}
		else{
			syslog(LOG_ERR, "Error: error desconocido al intentar crear directorio PROYECTO SO 1.");
		}
	}

    archivoHash* hasesh = NULL;
    hasesh = (archivoHash*)realloc(hasesh, (1)*sizeof(archivoHash));
    hasesh = (archivoHash*)realloc(hasesh, (2)*sizeof(archivoHash));
    hasesh = (archivoHash*)realloc(hasesh, (3)*sizeof(archivoHash));
    hasesh = (archivoHash*)realloc(hasesh, (4)*sizeof(archivoHash));
    sprintf(hasesh[0].nombre, "syslog.1");
    sprintf(hasesh[1].nombre, "btmp.1");
    sprintf(hasesh[2].nombre, "auth.log");
    sprintf(hasesh[3].nombre, "kern.log");
    
    tiempo(fecha_hora);
    //empaquetar(&hasesh, 4);

    return 0;
}