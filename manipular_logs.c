#include "./manipular_logs.h"

void Verificarlogs(const char *nombre, const char *md5_actual, archivoHash **hashes_revisados, int* contador) {
    char *hash_previo = "0";
    int es_nuevo = 1;

    
    // Por cada hash (cambio nuevo)
    for (int i = 0; i < *contador; i++) {
        if (strcmp((*hashes_revisados)[i].nombre, nombre) == 0) { // si el archivo ya ha sido guardado previamente
            hash_previo = (*hashes_revisados)[i].resultadoMd5;
            es_nuevo = 0; // el archivo no es nuevo, por lo tanto tiene un resultado anterior que comparar
            break;
        }
    }

    if (strcmp(hash_previo, "0") == 0 && es_nuevo != 0) { // si es la primera verificacion o el archivo es nuevo
        es_nuevo = 1;
        *hashes_revisados = (archivoHash*)realloc(*hashes_revisados, (*contador + 1)*sizeof(archivoHash));
        

        strncpy((*hashes_revisados)[*contador].nombre, nombre, 32);
        strncpy((*hashes_revisados)[*contador].resultadoMd5, md5_actual, 32);
        

        (*contador)++;
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*

    } else if (strcmp(md5_actual, hash_previo) != 0 && es_nuevo == 0) { // si el archivo ha tenido cambios
        for (int j = 0; j < *contador; j++) {
            if (strcmp((*hashes_revisados)[j].nombre, nombre) == 0) {
                syslog(LOG_INFO, "El archivo %s fue modificado.", (*hashes_revisados)[j].nombre);
                strncpy((*hashes_revisados)[j].resultadoMd5, md5_actual, 32);
                break;
            }
        }
        // GUARDAR PARA EMPAQUETAR *INSERTE CODIGO AQUI*
    }

}

void Leerlogs(archivoHash **hashes_revisados, int* contador) {
    DIR *dir = opendir("/var/log");
    struct dirent *archivo; 
    while((archivo = readdir(dir)) != NULL) {

        
        // AGREGAR COMPROBACION POR SI STRRCHR RETORNA NULL
        if(strrchr(archivo->d_name, '.') == NULL){
            continue;
        }
        else if(archivo->d_type == DT_REG  && strcmp(strrchr(archivo->d_name, '.'), ".gz") != 0) {

            int pipe_message[2];
            pipe(pipe_message);

            pid_t pid = fork();
            
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
                syslog(LOG_INFO, "Se leyo el archivo %s con un resultado de MD5 %s.", archivo->d_name, resultado);
                Verificarlogs(archivo->d_name, resultado, hashes_revisados, contador);
            } else {
                syslog(LOG_ERR, "Error al crear proceso hijo.");
                exit(EXIT_FAILURE);
                // error en el fork
            }

        }
    }
    closedir(dir);
}
