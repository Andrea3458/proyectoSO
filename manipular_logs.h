#ifndef MANIPULAR_LOGS H
#define MANIPULAR_LOGS H

#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>   
#include <dirent.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <unistd.h>  
#include <string.h>  

#define BUFFER_SIZE 512

typedef struct {

    char nombre[BUFFER_SIZE];
    char resultadoMd5[33];

} archivoHash;

void Verificarlogs(const char *nombre, const char *md5_actual, archivoHash **hashes_revisados, int* contador);
void Leerlogs(archivoHash **hashes_revisados, int* contador);

#endif