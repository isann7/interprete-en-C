#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

typedef struct {
    char nombre[50];
    int es_str;
    union {
      char texto[256];  
      int entero;
    }valor; 
} Simbolo;

typedef struct {
    Simbolo tabla[100];
    int cantidad;
} TablaSimbolos;

void Tabla_init(TablaSimbolos* t);
int Existe_simbolo(TablaSimbolos* t, char* nombre);
Simbolo Obtener_simbolo(TablaSimbolos* t, char* nombre);
void Guardar_simbolo(TablaSimbolos* t, char* nombre, int tipo, int entero, char* texto);

#endif 

