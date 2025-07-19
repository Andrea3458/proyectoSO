# Nombre del archivo ejecutable
TARGET = despachador

# Archivos compilados con extension .o
OBJECTS = despachador.o manejarlista.o procesos.o
CFLAGS = -O3 -Wall

# Punto de entrada para make si se ejecuta sin parametros.
all: TARGET

# Esta regla compila el proyecto completo una vez que todos los archivos objeto
# ya han sido compilados.
TARGET: $(OBJECTS)
	gcc -o $(TARGET) $(OBJECTS) $(CFLAGS)

# Compilar los archivos definidos en $(OBJECTS)
despachador.o: manejarlista.c manejarlista.h
	gcc -c manejarlista.c -o manejarlista.o

# Sustituye las banderas que se pasan al compilador por banderas para depuracion.
debug: CFLAGS = -g -Wall -D_DEBUG
debug: all

# Esta regla limpia los archivos creados por la compilacion.
clean:
	$(RM) $(TARGET) *.o
