#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

typedef enum{
  NOENCONTRADO, CUALQUIERA, ENTERO, STRING, ARRAY
}Tipo_simbolo;

typedef union{
  char texto[256];
  int entero;
  struct{
    struct Nodo** elementos;
    int cantidad;
    int capacidad;
  }array;
}Contenido;

typedef struct {
    char nombre[50];
    Tipo_simbolo tipo;
    Tipo_simbolo estricto;
    Contenido valor; 
}Simbolo;

typedef struct {
    Simbolo tabla[100];
    int cantidad;
}TablaSimbolos;

void Tabla_init(TablaSimbolos* t);
int Existe_simbolo(TablaSimbolos* t, char* nombre);
Simbolo Obtener_simbolo(TablaSimbolos* t, char* nombre);
void Guardar_simbolo(TablaSimbolos* t, char* nombre, Contenido valor, Tipo_simbolo tipo, Tipo_simbolo estricto);
Simbolo* Obtener_simbolo_ptr(TablaSimbolos* t, char* nombre);

#endif 

