#ifndef TABLA_FUNCIONES
#define TABLA_FUNCIONES

#define CAPACIDAD_TABLA 100

struct Nodo;

typedef struct{
  char nombre[50];
  int es_nativa;
  struct Nodo* func_def;
}Func;

typedef struct{
  Func tabla[CAPACIDAD_TABLA];
  int cantidad;
}TablaFunciones;


void Tabla_func_init(TablaFunciones* tf);
int Existe_func(TablaFunciones* tf, char nombre[50]);
Func Obtener_func(TablaFunciones* tf, char nombre[50]);
void Guardar_func(TablaFunciones* tf, char nombre[50], struct Nodo* func_def);
void Inyectar_func_nativa(TablaFunciones* tf);

#endif
