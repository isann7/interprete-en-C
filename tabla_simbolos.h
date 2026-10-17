#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

typedef enum{
  ENTERO, STRING, ARRAY
}Tipo_simbolo;

typedef union{
  char texto[256];
  int entero;
  struct Nodo** array;
}Contenido;

typedef struct {
    char nombre[50];
    Tipo_simbolo tipo;
    Contenido valor; 
}Simbolo;

typedef struct {
    Simbolo tabla[100];
    int cantidad;
}TablaSimbolos;

void Tabla_init(TablaSimbolos* t);
int Existe_simbolo(TablaSimbolos* t, char* nombre);
Simbolo Obtener_simbolo(TablaSimbolos* t, char* nombre);
void Guardar_simbolo(TablaSimbolos* t, char* nombre, Contenido valor, Tipo_simbolo tipo);

#endif 

