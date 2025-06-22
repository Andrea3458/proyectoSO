#include "./leer_archivo.h"
#include "./empaquetarArchivos.h"
#include <errno.h>

void verificar_archivo() {
	if (access("./proy1.ini", F_OK) != 0) {
		syslog(LOG_ERR, "El archivo de configuración no existe."); // CAMBIAR A LA ETIQUETA DEFINIDA
		exit(EXIT_FAILURE);
	}
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

		// // Logica principal del demonio
		// while(1) {
		// 	Leerlogs();
		// 	sleep(30);
		// }
}


int main() {

	char* directorio = "./var/log/PROYECTO SO 1";
	char log_tag[128];
    int intervalo;
	archivoHash* hashes_revisados = NULL;
	
	int hashes_contados = 0;

    leer_archivo(log_tag, &intervalo);
	log_tag[127] = '\0';
    crear_daemon();
    openlog(log_tag, LOG_PID | LOG_CONS, LOG_DAEMON);

	if(mkdir(directorio, 0755) == 0){
		syslog(LOG_INFO, "Se creo exitosamente el directorio PROYECTO SO 1.");
	}
	else{
		if(errno == EEXIST){
			syslog(LOG_ERR, "Error: el directorio PROYECTO SO 1 ya existe.");
		}
		else{
			syslog(LOG_ERR, "Error: error desconocido al intentar crear directorio PROYECTO SO 1.");
		}
	}
	
    syslog(LOG_INFO, "Daemon iniciando con etiqueta %s.", log_tag);
    syslog(LOG_INFO, "Cantidad de tiempo de intervalos %d.", intervalo); 

    while(1){
        syslog(LOG_INFO, "Revisando el directorio var/log..");
		int contador = 0;
		Leerlogs(&hashes_revisados, &contador);
		hashes_contados = contador;
        
        //Resto de la implementacion

        sleep(intervalo);
    }

    closelog();    
    
    return 0;
}
