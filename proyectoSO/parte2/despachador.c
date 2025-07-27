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
sem_t sem_ejecucion, sem_hilos_terminaron, sem_mutex;

int segundo_actual = 0, quantum = 0;
int max_hilos_ejecucion = 0, cont_hilos_ejecucion = 0;
int id_actual = -1;
int hay_proceso_en_ejecucion = 0, esPrimeraVez = 1;

int estaEnColaDeUsuarios(Proceso p){
    return esta_proceso_en_la_cola(&usuario, p);
}

void borrarProcesoDeAcuerdoACola(Proceso p){
    
    if(esta_proceso_en_la_cola(&tiempo_real, p)){
        eliminar_de_cola_proceso_especifico(&tiempo_real, p);
        return;
    }

    if(esta_proceso_en_la_cola(&usuario, p)){
        eliminar_de_cola_proceso_especifico(&usuario, p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[0], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[0], p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[1], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[1], p);
        return;
    }

    if(esta_proceso_en_la_cola(&prioridad[2], p)){
        eliminar_de_cola_proceso_especifico(&prioridad[2], p);
        return;
    }
}

int main (int argc, char *argv[]) {

    crear_Cola(&tiempo_real, 1000);
    crear_Cola(&usuario, 1000);
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
    sem_init(&sem_mutex, 0, 1);
    int contador_proceso = 0;

    //Crea la lista de Procesos
    int cap = leer_archivo_ini(lista_procesos_nombre);
    Proceso proc_sig = lista_procesos[0];
    Proceso proc_first = lista_procesos[0];
    //segundo_actual = proc_sig.tiempo_llegada;

    while(1) {

        //printf("UWUUUU\n");

        //Recibir procesos en el segundo que corresponde y meterlos a sus colas respectivas
        while(proc_sig.tiempo_llegada == segundo_actual) {
            //printf("UWU1\n");

            Proceso *proc_temp = malloc(sizeof(Proceso));
            *proc_temp = proc_sig;
            if (proc_temp == NULL) { break;}
            
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
        Proceso proc; // Asignar memoria para el proceso
        // Si la cola de tiempo real no esta vacia y el proceso en ejecucion no es tiempo real entonces...
        if(!is_empty(&tiempo_real) && hay_proceso_en_ejecucion != 1){
            //printf("UWU2\n");

            //Si hay un proceso de usuario
            if(hay_proceso_en_ejecucion == 2){
                
                //Si la prioridad es menor a 3 aumenta
                if(lista_procesos[id_actual].prioridad != 3){ 
                    lista_procesos[id_actual].prioridad++;
                }
                agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1], lista_procesos[id_actual]);

            }

            hay_proceso_en_ejecucion = 1;

            proc = eliminar_proceso(&tiempo_real);
            id_actual = proc.id;
            quantum = 0;
            esPrimeraVez = 1;


        //  Ejecutar procesos de usuario
        // Si la cola de tiempo usuario no esta vacia y el proceso en ejecucion no es tiempo real entonces...
        } else if (!is_empty(&usuario) && hay_proceso_en_ejecucion != 1){
            //printf("UWU3\n");
            
            int tamano_temp = usuario.tamano_actual;

            //ASIGNAR PROCESO A SU COLA DE PRIORIDAD
            for(int i = 0; i < tamano_temp; i++){

                proc = eliminar_proceso(&usuario);
                //printf("ID PROCESO A ELIMINAR: %d\n",proc.id);
                if(adquirir_recursos(&proc)) {
                
                agregar_proceso(&prioridad[proc.prioridad-1], proc);
                

                } else {
                    // Si no hay recursos, volver a encolar
                    liberar_recursos(&proc);
                    agregar_proceso(&usuario, proc);
                }

            }

            if(!is_empty(&prioridad[0])){
                proc = eliminar_proceso(&prioridad[0]);
            } else if(!is_empty(&prioridad[1])) {
                proc = eliminar_proceso(&prioridad[1]);
            } else if(!is_empty(&prioridad[2])){
                proc = eliminar_proceso(&prioridad[2]);
            } else if(id_actual != -1){
                proc = lista_procesos[id_actual];
            }

            //Si hay un proceso de usuario y el proceso en cola es inferior al actual entonces decrementa la prioridad
            if(hay_proceso_en_ejecucion == 2 && lista_procesos[id_actual].prioridad > proc.prioridad){
                
                quantum = 0;
                esPrimeraVez = 1;

                //LOS PROCESOS PRIORIDAD 3 NUNCA SUSPENDERAN A OTROS PROCESOS
                //if(lista_procesos[id_actual].prioridad != 3){ 
                lista_procesos[id_actual].prioridad++;
                //}
                agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-2], lista_procesos[id_actual]);

                //Actualiza id
                
                id_actual = proc.id;
            } else if(hay_proceso_en_ejecucion == 0){
                quantum = 0;
                esPrimeraVez = 1;
                id_actual = proc.id;
            }

            //printf("ID: %d HAY: %d\n",id_actual, hay_proceso_en_ejecucion);
            hay_proceso_en_ejecucion = 2;
        } else if(hay_proceso_en_ejecucion == 0 && is_empty(&tiempo_real) && is_empty(&usuario) && is_empty(&prioridad[0]) && is_empty(&prioridad[1]) && is_empty(&prioridad[2]) && proc_first.tiempo_llegada <= contador_proceso){
            break;
        }

        //printf("ID ACTUAL: %d, HAY PROCESO EN EJECUCION: %d, QUANTUM: %d\n",id_actual, hay_proceso_en_ejecucion, quantum);

        //Verificar Quantums
        if(hay_proceso_en_ejecucion == 2){

            if(quantum == 0){
                if(!esPrimeraVez){

                    if(lista_procesos[id_actual].prioridad != 3){ 
                        lista_procesos[id_actual].prioridad++;
                        agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-2], lista_procesos[id_actual]);
                    } else {
                        agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1], lista_procesos[id_actual]);
                    }
                    //printf("Prioridad: %d IDEn: %d TAMANO: %d ",lista_procesos[id_actual].prioridad, id_actual, prioridad[lista_procesos[id_actual].prioridad-1].tamano_actual);

                    agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1], lista_procesos[id_actual]);
                    proc = eliminar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1]);
                    id_actual = proc.id;
                    printf("IDMOD: %d\n",id_actual);
                }

                esPrimeraVez = 0;

                switch(proc.prioridad) {
                    case 1: quantum = 4; break;
                    case 2: quantum = 3; break;
                    case 3: quantum = 2; break;
                    default: quantum = 0; // Tiempo Real ejecuta completo
                }
            }

            quantum--;
        } 

        //printf("TR: %d U: %d, P0: %d P1: %d P2: %d Hay: %d ID: %d\n",is_empty(&tiempo_real), is_empty(&usuario), is_empty(&prioridad[0]), is_empty(&prioridad[1]), is_empty(&prioridad[2]), hay_proceso_en_ejecucion, id_actual);

        sem_wait(&sem_mutex);
        for(int i = 0; i < max_hilos_ejecucion; i++){
            sem_post(&sem_ejecucion);
        }

        if(proc_first.tiempo_llegada <= segundo_actual){
            //Registrar mensaje
            printf("Segundo %d: ", segundo_actual);
            sem_post(&sem_mutex);
            sem_wait(&sem_hilos_terminaron);
        } else {
            sem_post(&sem_mutex);
        }

        sem_wait(&sem_mutex);
        cont_hilos_ejecucion = 0;
        if(proc_first.tiempo_llegada <= segundo_actual){
            printf("\n");
        }
        segundo_actual++;
        sem_post(&sem_mutex);
        
    }

    // Destruir semaforos
    sem_destroy(&sem_ejecucion);
    sem_destroy(&sem_hilos_terminaron);
    sem_destroy(&sem_mutex);

    return 0;
}