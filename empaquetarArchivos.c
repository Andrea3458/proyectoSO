#include "./empaquetarArchivos.h"


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
       
    char fecha_hora[30];
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
            syslog(LOG_INFO, "Paquete comprimido correctamente.");
            // comprimido correctamente
        } else {
            syslog(LOG_ERR, "Error: No se pudo comprimir el paquete correctamente.");
            // error comprimir
        }

    } else if (pid < 0) {
        syslog(LOG_ERR, "Error: No se pudo crear proceso hijo.");
        // error creando hijo
    }
}