#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "cola.h"
#include "comunicaciondespachadorproceso.h"
#include "./leerarchivoentrada.h"

Cola tiempo_real;
Cola usuario;
Cola prioridad[3];
pthread_t hilos_de_procesos[1000];

int numImpresoras = 2, numScanner = 1, numModem = 1, numLectoresDVD = 2;
sem_t impresora, scanner, modem, lectoresDVD;

int main (int argc, char *argv[]) {

    crear_Cola(&tiempo_real, 1000);
    crear_Cola(&usuario, 1000);
    for(int i = 0; i < 3; i++){
        crear_Cola(&prioridad[i], 1000);
    }
    

    // Leer archivo de entrada
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE; // Indica que el programa terminó con un error
    }

    const char *lista_procesos_nombre = argv[1];

    // Inicializar semáforos
    sem_init(&impresora, 0, 2);
    sem_init(&scanner, 0, 1);
    sem_init(&modem, 0, 1);
    sem_init(&lectoresDVD, 0, 2);
    int seg = 1, cont = 0;

    //Crea la lista de Procesos
    int cap = leer_archivo_ini(lista_procesos_nombre);

    Proceso proc_sig = lista_procesos[0];

    //PUEDE OCURRIR ERROR SI SOLO QUEDA UN PROCESO EN LA LISTA O SOLO HAY UN PROCESO EN LISTA
    while(1){
        
        while(proc_sig.tiempo_llegada == seg){

            Proceso *proc_temp = malloc(sizeof(Proceso));
            *proc_temp = proc_sig;
            
            //Asignar procesos a la cola correspondiente
            if(proc_temp->prioridad == 0){
                agregar_proceso(&tiempo_real, *proc_temp);
            } else {
                agregar_proceso(&usuario, *proc_temp);
            }

            //Crear Proceso || Contador en tiempo del proceso en SO
            pthread_create(&hilos_de_procesos[proc_temp->id], NULL, crear_proceso, proc_temp);
            pthread_detach(hilos_de_procesos[proc_temp->id]);
            
            cont++;
            proc_sig = lista_procesos[cont];
        }


        if(!is_empty(&tiempo_real)){

            //write(msg.pipe_fd[1], mensaje, strlen(mensaje));

        } else if(!is_empty(&usuario)){

            //ASIGNA POR PRIORIDAD 
            
            if(!is_empty(&prioridad[0])){

            } else if(!is_empty(&prioridad[1])){

            } else if(!is_empty(&prioridad[2])){

            } else {
                return 0;
            }
            

        }

        sleep(1);
        seg++;

        if(seg == 20){
            break;
        }
    }

    // HOLA, SOY EL DESPACHADOR Y HE ELEGIDO ESTE PROCESO EN LA COLA DE MAYOR PRIORIDAD
    // HOLA, SOY EL DESPACHADOR Y LLAME A LA FUNCION EJECUTAR
    // HOLA, SOY EL DESPACHADOR Y SI ELEGI LA PRIORIDAD 0, DEBO ESPERAR A QUE TERMINE
    // SI ELIGIO LA PRIODAD 1, DEBO ESPERAR QUE EL PROCESO SE EJECUTE POR 3 SEGUNDOS
    // ETC COIN 2 Y 1 SEGUNDO
    return 0;
}