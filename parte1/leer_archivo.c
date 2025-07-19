#include "./leer_archivo.h"

// Lee el archivo .ini
void leer_archivo(char* log_tag, int* intervalo){
    FILE *archivo_ini = fopen("./proy1.ini", "r");
    //Verifica si el archivo existe
    if(archivo_ini == NULL){
        fprintf(stderr, "Error: El archivo de configuración proy1.ini no existe.\n");
        exit(EXIT_FAILURE);
    }
    
    char linea[256];
    char indicador[20];
    char* valor;
    
    while(!feof(archivo_ini)){
        if(fgets(linea, sizeof(linea), archivo_ini) == NULL){break;}
        //Permite ignorar los comentarios que empiezan con ;
        if(linea[0] != ';'){
            char* cadena_evaluar = strtok(linea, "\r");
            
            if(sscanf(cadena_evaluar, "%[^=]s", indicador) == 1 && strcmp(indicador, "log_tag") == 0){
                valor = strtok(cadena_evaluar, "=");
                valor = strtok(NULL, "\0");
                strcpy(log_tag, valor);//guarda el nombre de la etiqueta leida
            }

            if(sscanf(cadena_evaluar, "%[^=]s", indicador) == 1 && strcmp(indicador, "interval") == 0){
                valor = strtok(cadena_evaluar, "=");
                valor = strtok(NULL, "\r");
                *intervalo = strtol(valor, &valor, 10);//guarda los valores de intervalo
            }
        }
    }

    if(log_tag == NULL || intervalo == NULL){
        fclose(archivo_ini);
        exit(EXIT_FAILURE);
    }

    fclose(archivo_ini);

}