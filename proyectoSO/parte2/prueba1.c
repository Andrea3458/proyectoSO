
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int tiempo_llegada;
    int prioridad;
    int tiempo_procesador; 
    int num_impresoras;
    int num_scanners;
    int num_modems;
    int num_DVDs;
} Proceso;

Proceso *lista_procesos;
int capacidad = 1;

void leer_archivo_ini(const char *arg);
void guardar_proceso(char *arg);

void guardar_proceso(char *arg){
    Proceso temp;
    printf("%s\n", arg);
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


void leer_archivo_ini(const char *arg){
    char linea[256];
    FILE *archivo = fopen("listaprocesos.in", "r");

    if(archivo == NULL){
        printf("Error: error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        guardar_proceso(linea);
    }

    if(fclose(archivo) != 0){
        printf("Error: error al cerrar el archivo.\n");
        exit(EXIT_FAILURE);
    }

}

int main(){
    char *null;
    leer_archivo_ini(null);
    for(int i = 0; i < capacidad-1; i++){
        printf("T.llegada: %d Prioridad: %d T.procesador: %d Impresoras: %d Scanners: %d Modems: %d DVDs: %d.\n", lista_procesos[i].tiempo_llegada, lista_procesos[i].prioridad, lista_procesos[i].tiempo_procesador, lista_procesos[i].num_impresoras, lista_procesos[i].num_scanners, lista_procesos[i].num_modems, lista_procesos[i].num_DVDs);
    }

    return 0;
}