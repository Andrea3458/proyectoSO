#include "leerarchivoentrada.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Proceso *lista_procesos;
int capacidad = 1;

void guardar_proceso(char *arg, int num){
    Proceso temp;
    temp.id = num; 
    sscanf(arg, "%d, %d, %d, %d, %d, %d, %d", &temp.tiempo_llegada, &temp.prioridad, &temp.tiempo_procesador, &temp.num_impresoras, &temp.num_scanners, &temp.num_modems, &temp.num_DVDs);

    if(lista_procesos == NULL){
        lista_procesos = (Proceso *)malloc(capacidad*sizeof(Proceso));
        lista_procesos[0] = temp;
    }

    Proceso *lista_temp = (Proceso *)realloc(lista_procesos, (capacidad + 1)*sizeof(Proceso));
    lista_temp[capacidad-1] = temp;

    if(lista_temp == NULL){
        printf("Error: error al ampliar la lista.\n");
        free(lista_procesos);
        lista_procesos = NULL;
        exit(EXIT_FAILURE);
    }

    lista_procesos = lista_temp;
    capacidad+=1;
}


int leer_archivo_ini(const char *arg){
    char linea[256];
    int cont = 0;
    FILE *archivo = fopen(arg, "r");

    if(archivo == NULL){
        printf("Error: error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        guardar_proceso(linea, cont);
        cont++;
    }

    if(fclose(archivo) != 0){
        printf("Error: error al cerrar el archivo.\n");
        exit(EXIT_FAILURE);
    }

    return cont;

}

