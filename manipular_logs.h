#ifndef MANIPULAR_LOGS.H
#define MANIPULAR_LOGS.H

#include <stdlib.h>
#include <stdio.h>   
#include <dirent.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <unistd.h>  
#include <string.h>  

#define BUFFER_SIZE 256

typedef struct {

    char nombre[BUFFER_SIZE];
    char resultadoMd5[33];

} archivoHash;

archivoHash *hashesPrevios = NULL;
int contador = 0;

void Verificarlogs(const char *nombre, const char *md5_actual) {

    char *hash_previo = "0";
    int es_nuevo = 1;

    // Por cada hash (cambio nuevo)
    for (int i = 0; i < contador; i++) {
        if (strcmp(hashesPrevios[i].nombre, nombre) == 0) { // si el archivo ya ha sido guardado previamente
            hash_previo = hashesPrevios[i].resultadoMd5;
            es_nuevo = 0; // el archivo no es nuevo, por lo tanto tiene un resultado anterior que comparar
            break;
        }
    }

    if (strcmp(hash_previo, "0") == 0) { // si es la primera verificacion o el archivo es nuevo
        es_nuevo = 1;
        archivoHash *tmp = realloc(hashesPrevios, (contador + 1)*sizeof(archivoHash));

        hashesPrevios = tmp;

        strncpy(hashesPrevios[contador].nombre, nombre, 32);
        strncpy(hashesPrevios[contador].resultadoMd5, md5_actual, 32);

        contador++;
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*

    } else if (strcmp(md5_actual, hash_previo) != 0) { // si el archivo ha tenido cambios
        for (int j = 0; j < contador; j++) {
            if (strcmp(hashesPrevios[j].nombre, nombre) == 0) {
                strncpy(hashesPrevios[j].resultadoMd5, md5_actual, 32);
                break;
            }
        }
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*
    }

}

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
                ssize_t bytes_leidos= read(pipe_message[0], resultado, sizeof(resultado) - 1);
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


#endif