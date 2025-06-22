#ifndef REGISTRO_PID_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define REGISTRO_PID_H

#include<stdio.h>
#include<unistd.h>

void registrarPID(pid_t padre, bool *esPrimeraIteracion, int *i){
    FILE* archivo;

    archivo = fopen("30371074_31532714_31462340.txt","a+");

    if(archivo == NULL){
        printf("Hubo un problema, no se pudo crear archivo\n");
        return;
    }

    if(esPrimeraIteracion){
        fprintf(archivo, "PID Padre: %d\n", getpid());
        esPrimeraIteracion = false;
    } else {
        fprintf(archivo, "PID Hijo: %d\n", padre); /*Modificar para N Cantidad de Hijos*/
    }
    fprintf(archivo, "Intervalo %d\n",i);

    i++;
    fclose(archivo);
}