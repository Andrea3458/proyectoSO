#ifndef REGISTRO_PID_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define REGISTRO_PID_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void registrarPID(pid_t pid);

#endif

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static pid_t pid_padre = 0;
static int intervalo = 0;

void registrarPID(pid_t pid){

    // Verificar si es la primera llamada (proceso padre)
     if (pid_padre == 0) {
        pid_padre = getpid();
    }

    FILE* archivo;

    archivo = fopen("30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        perror("Error al abrir archivo de registro");
        exit(EXIT_FAILURE);
    }

    // Escribir PID del padre si el tamaño del archivo es 0 (esta vacio)
    fseek(archivo, 0, SEEK_END);
    if (ftell(archivo) == 0) {
        fprintf(archivo, "PID Padre: %d\n", pid_padre);
    }

    fflush(archivo); // controlar buffer de escritura

    if (pid_padre == 0) {
        // Es un nuevo intervalo (llamada desde el padre)
        fprintf(archivo, "\nIntervalo %d\n", intervalo++);
    } else if (pid > 0 && getpid() == pid_padre) {
        fprintf(archivo, "PID Hijo: %d\n", pid);
    }
    
    fflush(archivo);

    fclose(archivo);
}