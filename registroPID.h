#ifndef REGISTRO_PID_H  // Guardia de inclusión para evitar problemas de múltiples inclusiones
#define REGISTRO_PID_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <syslog.h> 
#include <time.h> 

void registrarPID(pid_t pid);

#endif