#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "cola.h"
#include "colaHilos.h"
#include "./leerarchivoentrada.h"

ColaHilos tiempo_real;
ColaHilos usuario;
ColaHilos prioridad[3];

int numImpresoras = 2, numScanner = 1, numModem = 1, numLectoresDVD = 2;
sem_t impresora, scanner, modem, lectoresDVD;


int main (int argc, char *argv[]) {

    tiempo_real.capacidad_max = 1000;
    usuario.capacidad_max = 1000;
    for(int i = 0; i < 3; i++){
        prioridad[i].capacidad_max = 1000;
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
    int seg = 1;
    
    while(!feof(lista_procesos_nombre)){

        leer_archivo_ini(lista_procesos_nombre);
        /*for(int i = 0; i < capacidad-1; i++){
            printf("T.llegada: %d Prioridad: %d T.procesador: %d Impresoras: %d Scanners: %d Modems: %d DVDs: %d.\n", lista_procesos[i].tiempo_llegada, lista_procesos[i].prioridad, lista_procesos[i].tiempo_procesador, lista_procesos[i].num_impresoras, lista_procesos[i].num_scanners, lista_procesos[i].num_modems, lista_procesos[i].num_DVDs);
        }*/
    }
    Proceso proc_temp = eliminar_proceso(&lista_procesos);

    while(!is_empty(&lista_procesos) || proc_temp != NULL){
        
        while(proc_temp->tiempo_llegada == seg){

            pthread_t hilo_de_proceso;

            //Asignar procesos a la cola correspondiente
            if(proc_temp->prioridad == 0){
                agregar_hilo(&tiempo_real, hilo_de_proceso);

                //Crear Proceso || Contador en tiempo del proceso en SO
                pthread_create(&tiempo_real->final, NULL, crear_proceso, &proc_temp);
                pthread_join(tiempo_real->final, NULL);
            } else {
                agregar_hilo(&usuario, hilo_de_proceso);

                //Crear Proceso || Contador en tiempo del proceso en SO
                pthread_create(&usuario->final, NULL, crear_proceso, &proc_temp);
                pthread_join(usuario->final, NULL);
            }
            
            Proceso proc_temp = eliminar_proceso(&lista_procesos);
        }


        if(!is_empty(&tiempo_real)){

            

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
    }

    // HOLA, SOY EL DESPACHADOR Y HE ELEGIDO ESTE PROCESO EN LA COLA DE MAYOR PRIORIDAD
    // HOLA, SOY EL DESPACHADOR Y LLAME A LA FUNCION EJECUTAR
    // HOLA, SOY EL DESPACHADOR Y SI ELEGI LA PRIORIDAD 0, DEBO ESPERAR A QUE TERMINE
    // SI ELIGIO LA PRIODAD 1, DEBO ESPERAR QUE EL PROCESO SE EJECUTE POR 3 SEGUNDOS
    // ETC COIN 2 Y 1 SEGUNDO
    return 0;
}