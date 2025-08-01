# Nombre del archivo ejecutable
TARGET = despachador

# Archivos compilados con extension .o
OBJECTS = despachador.o leerarchivoentrada.o control.o cola.o procesos.o
CFLAGS = -O3 -Wall

# Punto de entrada para make si se ejecuta sin parametros.
all: TARGET

# Esta regla compila el proyecto completo una vez que todos los archivos objeto
# ya han sido compilados.
TARGET: $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS) $(CFLAGS)

despachador.o: despachador.c leerarchivoentrada.h cola.h control.h
	gcc -c despachador.c -o despachador.o

leerarchivoentrada.o: leerarchivoentrada.c leerarchivoentrada.h procesos.h
	gcc -c leerarchivoentrada.c -o leerarchivoentrada.o

control.o: control.c control.h cola.h procesos.h
	gcc -c control.c -o control.o

cola.o: cola.c cola.h procesos.h
	gcc -c cola.c -o cola.o

procesos.o: procesos.c procesos.h
	gcc -c procesos.c -o procesos.o

# Sustituye las banderas que se pasan al compilador por banderas para depuracion.
debug: CFLAGS = -g -Wall -D_DEBUG
debug: all

# Esta regla limpia los archivos creados por la compilacion.
clean:
	$(RM) $(TARGET) *.o
