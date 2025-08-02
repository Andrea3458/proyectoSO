#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "cola.h"
#include "leerarchivoentrada.h"
#include "control.h"
#include <unistd.h>

Cola tiempo_real;
Cola usuario;
Cola prioridad[3];
pthread_t hilos_de_procesos[1000];

int numImpresoras = 2, numScanner = 1, numModem = 1, numLectoresDVD = 2;
Proceso proc_first;
sem_t sem_ejecucion, sem_hilos_terminaron, sem_mutex, sem_mutex2;

int segundo_actual = 0, quantum = 0;
int max_hilos_ejecucion = 0, cont_hilos_ejecucion = 0;
int id_actual = -1;
int hay_proceso_en_ejecucion = 0, esPrimeraVez = 1;

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
    sem_init(&sem_mutex2, 0, 1);
    int contador_proceso = 0;

    //Crea la lista de Procesos
    int cap = leer_archivo_ini(lista_procesos_nombre);
    Proceso proc_sig = lista_procesos[0];
    proc_first = lista_procesos[0];
    sem_wait(&sem_mutex2);

    while(1) {

        //Recibir procesos en el segundo que corresponde y meterlos a sus colas respectivas
        while(proc_sig.tiempo_llegada == segundo_actual) {

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
            
            contador_proceso++;
            max_hilos_ejecucion++;
            proc_sig = lista_procesos[contador_proceso];
        }

        //hay proceso en ejecucion = 0 NO HAY
        //hay proceso en ejecucion = 1 HAY, PERO ES TIEMPO REAL
        //hay proceso en ejecucion = 2 HAY, PERO ES USUARIO

        // Ejecutar procesos en tiempo real
        Proceso proc;
        // Si la cola de tiempo real no esta vacia y el proceso en ejecucion no es tiempo real entonces...
        if(!is_empty(&tiempo_real) && hay_proceso_en_ejecucion != 1){

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
        // Si las colas de tiempo usuario no estan vacias y el proceso en ejecucion no es tiempo real entonces...
        } else if (!is_empty(&usuario) && hay_proceso_en_ejecucion != 1){
            
            int tamano_temp = usuario.tamano_actual;

            //ASIGNAR PROCESO A SU COLA DE PRIORIDAD
            for(int i = 0; i < tamano_temp; i++){

                proc = eliminar_proceso(&usuario);
                //Verifica Recursos
                if(adquirir_recursos(&proc)) {
                
                agregar_proceso(&prioridad[proc.prioridad-1], proc);

                } else {
                    // Si no hay recursos, volver a encolar
                    liberar_recursos(&proc);
                    agregar_proceso(&usuario, proc);
                }

            }

            //Elegir el de cola de prioridad màs alta
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

                if(lista_procesos[id_actual].prioridad != 3){ 
                    lista_procesos[id_actual].prioridad++;
                }
                agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1], lista_procesos[id_actual]);

                //Actualiza id
                id_actual = proc.id;

            //Si no hay proceso en ejecución actualmente
            } else if(hay_proceso_en_ejecucion == 0){
                quantum = 0;
                esPrimeraVez = 1;
                id_actual = proc.id;
            }

            hay_proceso_en_ejecucion = 2;

        //Si no hay proceso en ejecucion, todas las colas están vacías y el primer proceso ya llegó al sistema entonces...
        } else if(hay_proceso_en_ejecucion == 0 && is_empty(&tiempo_real) && is_empty(&usuario) && is_empty(&prioridad[0]) && is_empty(&prioridad[1]) && is_empty(&prioridad[2]) && proc_first.tiempo_llegada <= contador_proceso){
            break;
        }

        //Verificar Quantums y supervisar colas de usuario
        if(hay_proceso_en_ejecucion != 1 && (!is_empty(&prioridad[0]) || !is_empty(&prioridad[1]) || !is_empty(&prioridad[2]))){

            //Si no hay proceso actual en ejecución...
            if(hay_proceso_en_ejecucion == 0){
                if(!is_empty(&prioridad[0])){
                    proc = eliminar_proceso(&prioridad[0]);
                } else if(!is_empty(&prioridad[1])) {
                    proc = eliminar_proceso(&prioridad[1]);
                } else {
                    proc = eliminar_proceso(&prioridad[2]);
                }
                quantum = 0;
                esPrimeraVez = 1;
                hay_proceso_en_ejecucion = 2;
                id_actual = proc.id;
            }

            //Lógica del quantum
            if(quantum == 0){
                if(!esPrimeraVez){

                    //Si la cola actual no está vacia entonces...
                    if(!is_empty(&prioridad[lista_procesos[id_actual].prioridad-1])){
                        proc = eliminar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1]);

                        //Se reduce si se puede...
                        if(lista_procesos[id_actual].prioridad != 3){ 
                            lista_procesos[id_actual].prioridad++; 
                        }

                        agregar_proceso(&prioridad[lista_procesos[id_actual].prioridad-1], lista_procesos[id_actual]);

                    }

                    id_actual = proc.id;
                }

                esPrimeraVez = 0;

                //Selecciona de acuerdo a prioridad
                switch(proc.prioridad) {
                    case 1: quantum = 4; break;
                    case 2: quantum = 3; break;
                    case 3: quantum = 2; break;
                    default: quantum = 0; // Tiempo Real ejecuta completo
                }
            }

            quantum--;
        }

        //Lógica semáforos
        control_semaforos();
        //SIMULACIÓN DE LOS SEGUNDOS
        sleep(1); /**/
        
    }

    // Destruir semaforos
    sem_destroy(&sem_ejecucion);
    sem_destroy(&sem_hilos_terminaron);
    sem_destroy(&sem_mutex);
    sem_destroy(&sem_mutex2);

    return 0;
}