#include "procesos.h"
#include "comunicaciondespachadorproceso.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void registrar_mensajes (int segundo, int id_proceso, const char *estado) {

    FILE *archivo = fopen("/30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        // error
    }

    char mensaje[256];
    snprintf(mensaje, sizeof(mensaje), "Segundo %d: #%d %s\n", segundo, id_proceso, estado);

    printf("%s", mensaje);

    fprintf(archivo, "%s", mensaje);
    fclose(archivo);
}

void crear_proceso (void *arg) {
    MensajeProceso info = *(MensajeProceso*)arg;
    char buffer[256];
    ssize_t bytes_leidos;

    for(int i = 0; i < 20; i++){
        bytes_leidos = read(info.pipe_fd[0], buffer, sizeof(buffer) - 1);

        if(bytes_leidos > 0){
            printf("Hola soy %d Recibi el mensaje: %s \n", info.proceso.id, buffer);
        } else if (bytes_leidos == 0){
            printf("Hola soy %d. En espera o terminado \n", info.proceso.id);
        } else {
            printf("Hola soy %d ERROR", info.proceso.id);
        }       

        sleep(1);
    }
}

void ejecutar_proceso() {

    //pide_recursos();

    

}