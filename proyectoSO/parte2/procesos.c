#include "procesos.h"
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

void *crear_proceso (void *arg) {
    Proceso *info = (Proceso*)arg;

    for(int i = 0; i < 20; i++){
        
        printf("Hola soy %d como tas?\n", info->id);      

        sleep(1);
    }
}

void ejecutar_proceso() {

    //pide_recursos();

    

}