#include "./registroPID.h"


static FILE* archivo = NULL;
static time_t ultimo_intervalo = 0;
static int intervalo_actual = 0;
static int intervalo_segundos = 7200;

void registrarPID(pid_t pid){

    time_t ahora;
    time(&ahora);

    archivo = fopen("/30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        syslog(LOG_INFO, "Error al abrir archivo de registro");
        exit(EXIT_FAILURE);
    }

    // Escribir PID del padre si el tamaño del archivo es 0 (esta vacio)
    fseek(archivo, 0, SEEK_END);
    if (ftell(archivo) == 0) {
        fprintf(archivo, "PID Padre: %d\n", getpid());
    }

    fflush(archivo); // controlar buffer de escritura

    if (difftime(ahora, ultimo_intervalo) >= intervalo_segundos) {
        
        // Es un nuevo intervalo (llamada desde el padre)
        fprintf(archivo, "\nIntervalo %d\n", intervalo_actual++ * intervalo_segundos);
        ultimo_intervalo = ahora;
    } 
    
    if (pid > 0) {
        fprintf(archivo, "PID Hijo: %d\n", pid);
    }
    
    fflush(archivo); // controlar buffer de escritura

    fclose(archivo);
}