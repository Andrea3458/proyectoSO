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
void tiempo(char* fecha_hora) {
    time_t tiempo;
    struct tm *info_tiempo;
    
    time(&tiempo);
    info_tiempo = localtime(&tiempo);
    memset(fecha_hora, 0, 20);
    strftime(fecha_hora, 30, "%Y%m%d-%H%M%S", info_tiempo);
}

// recibe la lista de archivos modificados
void empaquetar(const archivoHash** archivos, int num_archivos, char* nombre_pak) {
       
    char fecha_hora[20];
    tiempo(fecha_hora);
    
    // crear el nombre del archivo en la ruta especificada y el formato de fecha-hora
    snprintf(nombre_pak, 100, "/var/log/PROYECTO SO 1/logs%s.pak", fecha_hora);
    pak_cabecera cabecera;
    

    FILE *archivo_pak = fopen(nombre_pak, "wb");
    if(archivo_pak == NULL){
        syslog(LOG_ERR, "Error: puntero del archivo es nulo");
    }
    char path[512] = "/var/log/";

    // Por cada archivo en la lista
    for (int i = 0; i < num_archivos; i++) {
        
        char path[512] = "/var/log/";
        strcat(path, (*archivos)[i].nombre);
        
        FILE* archivo = fopen(path, "rb");
        if(archivo == NULL){
            syslog(LOG_ERR, "Error: puntero del archivo es nulo");
        }

        fseek(archivo, 0, SEEK_END); // ir hasta el final del archivo
        int64_t tamano = ftell(archivo);
        fseek(archivo, 0, SEEK_SET); // volver al inicio
        // crear cabecera para el archivo actual
        strncpy(cabecera.nombre, (*archivos)[i].nombre, 31);
        cabecera.nombre[31] = '\0';
        cabecera.bytes = tamano;

        // escribir cabecera y contenido
        size_t bits_escritos = fwrite(&cabecera, sizeof(pak_cabecera), 1, archivo_pak);
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

    char fecha_hora[30];
    char nombre_pak[100];
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
    printf("%s", fecha_hora);
    empaquetar(&hasesh, 4, nombre_pak);
    comprimir_pak(nombre_pak);

    return 0;
}