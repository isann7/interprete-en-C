# Nombre del ejecutable que se va a generar
TARGET = interprete

# Detecta automáticamente todos los archivos .c de la carpeta
SRCS = $(wildcard *.c)

# Regla por defecto para compilar todo automáticamente
all:
	gcc $(SRCS) -o $(TARGET)

# Regla opcional para limpiar la carpeta y borrar el ejecutable viejo
clean:
	rm -f $(TARGET)
