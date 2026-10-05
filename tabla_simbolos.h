#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

typedef struct {
    char nombre[50];
    int valor;
} Simbolo;

typedef struct {
    Simbolo tabla[100];
    int cantidad;
} TablaSimbolos;

void Tabla_init(TablaSimbolos* t);
int Existe_simbolo(TablaSimbolos* t, char* nombre);
Simbolo Obtener_simbolo(TablaSimbolos* t, char* nombre);
void Guardar_simbolo(TablaSimbolos* t, char* nombre, int valor);

#endif 

