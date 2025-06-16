#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>
#include "leerLogs.h"

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

		// Logica principal del demonio
		while(1) {
			Leerlogs();
			sleep(30);
		}
}


int main() {

	verificar_archivo();
	crear_daemon();


	return 0;
}
