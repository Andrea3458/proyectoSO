#include <stdio.h>
#include<semaphore.h>
#include "cola.h"
#include "procesos.h"

Cola tiempo_real[1000];
Cola usuario[1000];

int numImpresoras = 2, numScanner = 1, numModem = 1, lectoresDVD = 2;
sem_t impresora, scanner, modem, lectoresDVD;

int main (int argc, char *argv[]) {

    // Leer archivo de entrada
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE; // Indica que el programa terminó con un error
    }

    const char *lista_procesos = argv[1];

    // ARENA LEE TRABAJA CON lista_procesos


    // Inicializar semáforos
    sem_init(&impresora, 0, 2);
    sem_init(&scanner, 0, 1);
    sem_init(&modem, 0, 1);
    sem_init(&lectoresDVD, 0, 2);
    int seg = 1;
    
    while(!feof(lista_procesos)){

        while( /*Exista procesos en la lista con <tiempo de llegada> n*/){

            // Inserta proceso

        }

        sleep(1);
        seg++;
    }

    return 0;
}