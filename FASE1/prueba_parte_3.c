#include <syslog.h>
#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>
#include <syslog.h>

void verificar_archivo() {
	if (access("./proy1.ini", F_OK) != 0) {
		syslog(LOG_ERR, "El archivo de configuración no existe."); // CAMBIAR A LA ETIQUETA DEFINIDA
		exit(EXIT_FAILURE);
	}
}

void leer_archivo(char* log_tag, int* intervalo){
    FILE *archivo_ini = fopen("./proy1.ini", "r");
    if(archivo_ini == NULL){
        fprintf(stderr, "Error: El archivo de configuración proy1.ini no existe.\n");
        exit(EXIT_FAILURE);
    }
    
    char linea[256];
    char indicador[20];
    char* valor;
    
    while(!feof(archivo_ini)){
        if(fgets(linea, sizeof(linea), archivo_ini) == NULL){break;}
        
        if(linea[0] != ';'){
            char* cadena_evaluar = strtok(linea, "\r");
            
            if(sscanf(cadena_evaluar, "%[^=]s", indicador) == 1 && strcmp(indicador, "log_tag") == 0){
                valor = strtok(cadena_evaluar, "=");
                valor = strtok(NULL, "\0");
                strcpy(log_tag, valor);
            }

            if(sscanf(cadena_evaluar, "%[^=]s", indicador) == 1 && strcmp(indicador, "interval") == 0){
                valor = strtok(cadena_evaluar, "=");
                valor = strtok(NULL, "\r");
                *intervalo = strtol(valor, &valor, 10);
            }
        }
    }

    if(log_tag == NULL || intervalo == NULL){
        syslog(LOG_ERR, "Formato incorrecto en /etc/proyecto so 1/proy1.ini: 'interval' o 'log_tag' no encontrado o inválido.");
        fclose(archivo_ini);
        exit(EXIT_FAILURE);
    }

    fclose(archivo_ini);

}

void crear_daemon() {
	pid_t pid = fork();
		if (pid < 0) {
			perror("Error en fork()");
			exit(EXIT_FAILURE);
		}
		if (pid > 0) {
			exit(EXIT_SUCCESS);
			// El proceso padre termina, y el hijo queda "huérfano" (es adoptado por init/systemd)
		}

		// permisos para el daemon
		umask(0);

		// El proceso se independiza del terminal y se convierte en líder de sesión.
		if (setsid() < 0) {
			perror("Error en setsid()");
			exit(EXIT_FAILURE);
		}

		// Cambiar el directorio de trabajo a /
		chdir("/");

		// Cerrar descriptores de archivo estándar, daemon no necesita interactuar con la terminal
		close(STDIN_FILENO); 
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
}

int main(){
    char log_tag[128];
    int intervalo;

    leer_archivo(log_tag, &intervalo);
    crear_daemon();
    openlog(log_tag, LOG_PID | LOG_CONS, LOG_DAEMON);

    syslog(LOG_INFO, "Demonio iniciado correctamente con etiqueta de log: %s", log_tag);
    syslog(LOG_INFO, "El intervalo de revisión es de %d segundos.", intervalo); 

    while(1){
        syslog(LOG_INFO, "Realizando revisión del directorio /var/log...");
        
        //Resto de la implementacion

        sleep(intervalo);
    }

    closelog();    
    
    return 0;
}