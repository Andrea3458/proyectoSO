#include <stdio.h>
#include <semaphore.h>
#include "cola.h"
#include "procesos.h"

Cola tiempo_real;
Cola usuario;
Cola prioridad[3];

int numImpresoras = 2, numScanner = 1, numModem = 1, lectoresDVD = 2;
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

    const char *lista_procesos = argv[1];

    // Inicializar semáforos
    sem_init(&impresora, 0, 2);
    sem_init(&scanner, 0, 1);
    sem_init(&modem, 0, 1);
    sem_init(&lectoresDVD, 0, 2);
    int seg = 1;
    
    while(!feof(lista_procesos)){

        // // ARENA LEE TRABAJA CON lista_procesos
        
        while( /*Exista procesos en la lista con <tiempo de llegada> n*/){

            // Inserta proceso
            crear_procesos();

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