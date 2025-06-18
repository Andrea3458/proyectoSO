#include <syslog.h>
#include "./leer_archivo.h"

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

	openlog("PP", LOG_PID | LOG_CONS, LOG_DAEMON);
	syslog(LOG_INFO, "Si funciona el log");
	closelog();

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
