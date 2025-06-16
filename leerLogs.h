#ifndef LEER_LOGS_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define LEER_LOGS_H

#include <dirent.h> 
#include <sys/stat.h> 
#include <sys/wait.h>
#include <unistd.h>  
#include <string.h>  
#include "verificarLogs.h"  

void Leerlogs();

#endif

const int BUFFER_SIZE = 256;

void Leerlogs() {
    DIR *dir = opendir("/var/log");
    struct dirent *archivo; 

    while((archivo = readdir(dir)) != NULL) {

        // AGREGAR COMPROBACION POR SI STRRCHR RETORNA NULL
        if(archivo->d_type == DT_REG  && strcmp(strrchr(archivo->d_name, '.'), ".gz") == 0) {

            pid_t pid = fork();
            int pipe_message[2];
            
            if (pid == 0) {
                close(pipe_message[0]); // 0 es el extremo de lectura

                dup2(pipe_message[1], STDOUT_FILENO);   //pipe recibe la salida de md5sum
                close(pipe_message[1]);

                char archivo_actual[BUFFER_SIZE];
                sprintf(archivo_actual, "/var/log/%s", archivo->d_name);
                execlp("md5sum", "md5sum", archivo_actual , NULL);
                
            } else if (pid > 0) {
                close(pipe_message[1]); // 1 es el extremo de escritura
                
                char resultado[33];
                waitpid(pid, NULL, 0);
                ssize_t bytes_leidos(read(pipe_message[0], resultado, sizeof(resultado) - 1));
                resultado[32] = '\0';
                close(pipe_message[0]);

                Verificarlogs(archivo->d_name, resultado);
            } else {
                // error en el fork
            }

        }
    }
    closedir(dir);
}