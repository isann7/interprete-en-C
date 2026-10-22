#include "funciones_recursivas.h"
#include <stdio.h>


void print_array(Evaluador result, TablaSimbolos* t, TablaFunciones* tf){
  int i = 0;
  printf("[");
  while(i < result.valor.array.cantidad){ 
    Evaluador ev = Evaluar_expresion(result.valor.array.elementos[i], t, tf);
    if(ev.tipo == ENTERO){
      printf("%d", ev.valor.entero);
    } 
    else if(ev.tipo == STRING){
      printf("\"%s\"", ev.valor.texto);
    }
    else if(ev.tipo == ARRAY){
      print_array(ev, t, tf);
    }
    if(i < result.valor.array.cantidad - 1){ printf(", "); }
      i++;
    }
    printf("]");
}




