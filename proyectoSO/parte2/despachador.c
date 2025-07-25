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
Cola usuario;
Cola prioridad[3];
pthread_t hilos_de_procesos[1000];

int numImpresoras = 2, numScanner = 1, numModem = 1, numLectoresDVD = 2;
//sem_t impresora, scanner, modem, lectoresDVD;
sem_t sem_ejecucion, sem_hilos_terminaron;

int segundo_actual = 0;
int max_hilos_ejecucion = 0, cont_hilos_ejecucion = 0;
int id_actual = 0;
int hay_proceso_en_ejecucion = 0;


int main (int argc, char *argv[]) {

    crear_Cola(&tiempo_real, 1000);

    for(int i = 0; i < 3; i++) {
        crear_Cola(&prioridad[i], 1000);
    }
    
    // Leer archivo de entrada
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE; // Error por falta de argumentos
    }

    const char *lista_procesos_nombre = argv[1];

    // Inicializar semáforos
    sem_init(&sem_ejecucion, 0, 0);
    sem_init(&sem_hilos_terminaron, 0, 0);
    int contador_proceso = 0;

    //Crea la lista de Procesos
    int cap = leer_archivo_ini(lista_procesos_nombre);
    Proceso proc_sig = lista_procesos[0];
    Proceso proc_first = lista_procesos[0];
    //segundo_actual = proc_sig.tiempo_llegada;

    while(cap > contador_proceso) {

        printf("UWUUUU\n");

        while(proc_sig.tiempo_llegada == segundo_actual) {
            printf("UWU1\n");

            Proceso *proc_temp = malloc(sizeof(Proceso));
            *proc_temp = proc_sig;
            
            //Asignar procesos a la cola correspondiente
            if(proc_temp->prioridad == 0){
                agregar_proceso(&tiempo_real, *proc_temp);
            } else {
                agregar_proceso(&usuario, *proc_temp);
            }

            //Crear Proceso || Contador en tiempo del proceso en SO
            pthread_create(&hilos_de_procesos[proc_temp->id], NULL, ejecutar_proceso, proc_temp);
            //pthread_detach(hilos_de_procesos[proc_temp->id]);
            
            contador_proceso++;
            max_hilos_ejecucion++;
            proc_sig = lista_procesos[contador_proceso];
        }

        //hay proceso en ejecucion = 0 NO HAY
        //hay proceso en ejecucion = 1 HAY, PERO ES TIEMPO REAL
        //hay proceso en ejecucion = 2 HAY, PERO ES USUARIO
        // Ejecutar procesos en tiempo real
        if(!is_empty(&tiempo_real) && hay_proceso_en_ejecucion != 1){
            printf("UWU2\n");

            if(hay_proceso_en_ejecucion == 2){
                 printf("Segundo #%d: ",segundo_actual);
            }

            hay_proceso_en_ejecucion = 1;

            Proceso *proc = malloc(sizeof(Proceso)); // Asignar memoria para el proceso
            *proc = eliminar_proceso(&tiempo_real);
            id_actual = proc->id;


        //  Ejecutar procesos de usuario
        } else if (!is_empty(&usuario) && hay_proceso_en_ejecucion != 1){
            printf("UWU3\n");

            Proceso *proc = malloc(sizeof(Proceso)); // Asignar memoria para el proceso
            hay_proceso_en_ejecucion = 2;

            //ASIGNAR PROCESO A SU COLA DE PRIORIDAD
            while(!is_empty(&usuario)){

                if(adquirir_recursos(proc)) {
                
                *proc = eliminar_proceso(&usuario);
                agregar_proceso(&prioridad[proc->prioridad-1], *proc);

                } else {
                    // Si no hay recursos, volver a encolar
                    agregar_proceso(&usuario, *proc);
                }

            }

            if(!is_empty(&prioridad[0])){
               *proc = eliminar_proceso(&prioridad[0]);
            } else if(!is_empty(&prioridad[1])) {
                *proc = eliminar_proceso(&prioridad[1]);
            } else {
                *proc = eliminar_proceso(&prioridad[2]);
            }
            
            id_actual = proc->id;
        }

        // Verificar si todos los procesos han terminado
        /*if(is_empty(&tiempo_real) && hay_proceso_en_ejecucion == 0 && segundo_actual > 0) {
            int todas_vacias = 1;
            for(int j = 0; j < 3; j++) {
                if(!is_empty(&prioridad[j])) {
                    todas_vacias = 0;
                    break;
                }
            }
            if(todas_vacias) break;
        }*/

        for(int i = 0; i < max_hilos_ejecucion; i++){
            sem_post(&sem_ejecucion);
        }

        if(proc_first.tiempo_llegada <= contador_proceso){
            sem_wait(&sem_hilos_terminaron);
        }
        cont_hilos_ejecucion = 0;

        segundo_actual++;
        
    }

    // Destruir semaforos
    sem_destroy(&sem_ejecucion);
    sem_destroy(&sem_hilos_terminaron);

    return 0;
}