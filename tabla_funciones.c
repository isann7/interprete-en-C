
#include "tabla_funciones.h"
#include "ast.h"            
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Tabla_func_init(TablaFunciones* tf){
  memset(&(tf->tabla), 0, sizeof(tf->tabla));
  tf->cantidad = 0;
}

int Existe_func(TablaFunciones* tf, char nombre[50]){
  int i = 0;
  while(i < tf->cantidad){
    if (!strcmp(tf->tabla[i].nombre, nombre)){
      return i;
    }
    i++;
  }
  return -1;
}

Func Obtener_func(TablaFunciones* tf, char nombre[50]){
  int idx = Existe_func(tf, nombre);
  if (idx == -1){
    printf("error, no existe la función %s\n", nombre);
    exit(1);
  }
  return tf->tabla[idx];
}

void Guardar_func(TablaFunciones* tf, char nombre[50], Nodo* func_def){
  int idx = Existe_func(tf, nombre);
  if (idx != -1){
    printf("error, solo se admite 1 definición por función");
    exit(1);
  } 
  strcpy(tf->tabla[tf->cantidad].nombre, nombre);    
  tf->tabla[tf->cantidad].func_def = func_def;
  tf->cantidad++;
}





