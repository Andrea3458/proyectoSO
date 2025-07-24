#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "cola.h"
#include "leerarchivoentrada.h"
#include "controlrecursos.h"
#include <unistd.h>

Cola tiempo_real;
Cola prioridad[3];
pthread_t hilos_de_procesos[1000];

int numImpresoras = 2, numScanner = 1, numModem = 1, numLectoresDVD = 2;
sem_t impresora, scanner, modem, lectoresDVD;


int main (int argc, char *argv[]) {

    crear_Cola(&tiempo_real, 1000);

    for(int i = 0; i < 3; i++){
        crear_Cola(&prioridad[i], 1000);
    }
    
    // Leer archivo de entrada
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE; // Error por falta de argumentos
    }

    const char *lista_procesos_nombre = argv[1];

    // Inicializar semáforos
    sem_init(&impresora, 0, 2);
    sem_init(&scanner, 0, 1);
    sem_init(&modem, 0, 1);
    sem_init(&lectoresDVD, 0, 2);
    int contador_proceso = 0, seg = 0;

    //Crea la lista de Procesos
    int cap = leer_archivo_ini(lista_procesos_nombre);
    Proceso proc_sig = lista_procesos[0];

    while(1) {

        // asignar procesos a sus respectivas colas
        while (contador_proceso < cap) {
            proc_sig = lista_procesos[contador_proceso];
            proc_sig.tiempo_ejecutado = 0;

            // Esperar hasta que el proceso deba ser asignado
            // AQUI ESTOY ASUMIENDO QUE EN LA LISTA LOS PROCESOS ESTARAN EN ORDEN,
            // ES DECIR, QUE UN PROCESO QUE LLEGUE EN EL SEGUNDO 1 NO PUEDE ESTAR EN LA LINEA DESPUES
            // DE UN PROCESO QUE LLEGUE EN EL SEGUNDO 3, NO SE SI ESTO VAYA A SER ASI SIEMPRE
            while (seg < proc_sig.tiempo_llegada) {
                seg++;
            }

            Proceso *proc_temp = malloc(sizeof(Proceso));
            *proc_temp = proc_sig;
            
            //Asignar procesos a la cola correspondiente
            if(proc_temp->prioridad == 0){
                agregar_proceso(&tiempo_real, *proc_temp);
            } else {

                agregar_proceso(&prioridad[proc_temp->prioridad-1], *proc_temp);
            }
            contador_proceso++;
            free(proc_temp);
        }

        // Ejecutar procesos en tiempo real
        if(!is_empty(&tiempo_real)){
            Proceso *proc = malloc(sizeof(Proceso)); // Asignar memoria para el proceso
            *proc = eliminar_proceso(&tiempo_real);

            if(pthread_create(&hilos_de_procesos[proc->id], NULL, ejecutar_proceso, proc) != 0) {
                perror("Error al crear hilo");
                free(proc);
            } else {
                pthread_detach(hilos_de_procesos[proc->id]);
            }
        //  Ejecutar procesos de usuario
        } else {

            for (int j = 0; j < 3; j++) {

                if(!is_empty(&prioridad[j])){

                    Proceso *proc = malloc(sizeof(Proceso)); // Asignar memoria para el proceso
                    *proc = eliminar_proceso(&prioridad[j]);

                    if(adquirir_recursos(proc)) {
                        if(pthread_create(&hilos_de_procesos[proc->id], NULL, ejecutar_proceso, proc) != 0) {
                            perror("Error al crear hilo");
                            liberar_recursos(proc);
                            free(proc);
                        } else {
                            pthread_detach(hilos_de_procesos[proc->id]);
                        }
                    } else {
                        // Si no hay recursos, volver a encolar
                        agregar_proceso(&prioridad[j], *proc);
                    }
                    break;
                }
            }
        }

        // Verificar si todos los procesos han terminado
        if(is_empty(&tiempo_real)) {
            int todas_vacias = 1;
            for(int j = 0; j < 3; j++) {
                if(!is_empty(&prioridad[j])) {
                    todas_vacias = 0;
                    break;
                }
            }
            if(todas_vacias) break;
        }
        sleep(1);
    }

    // Destruir semaforos
    sem_destroy(&impresora);
    sem_destroy(&scanner);
    sem_destroy(&modem);
    sem_destroy(&lectoresDVD);

    return 0;
}