#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"

void Tabla_init(TablaSimbolos* t) {
    memset(&(t->tabla), 0, sizeof(t->tabla));
    t->cantidad = 0;
}

int Existe_simbolo(TablaSimbolos* t, char* nombre) {
    int i = 0;
    while (i < t->cantidad) { 
        if (!strcmp(t->tabla[i].nombre, nombre)) {
            return i;
        }
        i++;
    }
    return -1;
}

Simbolo Obtener_simbolo(TablaSimbolos* t, char* nombre) {
    int idx = Existe_simbolo(t, nombre);
    if (idx == -1) {
      printf("no existe el simbolo %s", nombre);
      exit(1);
    }
    return t->tabla[idx];
}

void Guardar_simbolo(TablaSimbolos* t, char* nombre, Contenido valor, Tipo_simbolo tipo, Tipo_simbolo estricto) {
  int idx = Existe_simbolo(t, nombre);
  if (idx == -1) {
    if(estricto != CUALQUIERA && tipo != estricto){
      printf("Error de Tipos: La variable '%s' es de tipo estricto, no puedes asignarle este valor.\n", nombre);
      exit(1);
    }
    strcpy(t->tabla[t->cantidad].nombre, nombre); 
    switch ((int) tipo) {
      case ENTERO:
          t->tabla[t->cantidad].valor.entero = valor.entero;
          break;
      case STRING: 
          strcpy(t->tabla[t->cantidad].valor.texto, valor.texto);
          break;
      case ARRAY:
          t->tabla[t->cantidad].valor.array = valor.array;
          break;
    }
    t->tabla[t->cantidad].tipo = tipo;
    t->tabla[t->cantidad].estricto = estricto;
    t->cantidad++;
   } 
  else {
    if(t->tabla[idx].estricto != CUALQUIERA && tipo != t->tabla[idx].estricto){
      printf("Error de Tipos: La variable '%s' es de tipo estricto, no puedes asignarle este nuevo valor.\n", nombre);
      exit(1);
    }
    switch ((int) tipo) {
      case ENTERO:
          t->tabla[idx].valor.entero = valor.entero;
          break;
      case STRING:
          strcpy(t->tabla[idx].valor.texto, valor.texto);
          break;
      case ARRAY:
          t->tabla[idx].valor.array = valor.array;
          break;
    }
    t->tabla[idx].tipo = tipo;
  }
}



Simbolo* Obtener_simbolo_ptr(TablaSimbolos* t, char* nombre){
  int idx = Existe_simbolo(t, nombre);
  if(idx == -1){
    printf("error, no existe el simbolo %s\n", nombre);
    exit(1);
  }
  return &(t->tabla[idx]);
}



