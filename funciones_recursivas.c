#include "funciones_recursivas.h"
#include <stdio.h>
#include <stdlib.h>


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

void mutar_elementos_array(Nodo** elementos, int cantidad, Tipo_simbolo tipo_a_mutar){
  for (int i = 0; i < cantidad; i++) {
    if(elementos[i]->tipo == NODO_ARRAY_INIT){
      mutar_elementos_array(elementos[i]->valor.array_init.elementos, elementos[i]->valor.array_init.cantidad, tipo_a_mutar);
    } 
    else if(elementos[i]->tipo == NODO_STRING && tipo_a_mutar == ENTERO){
      int num = atoi(elementos[i]->valor.str);
      free(elementos[i]);
      elementos[i] = Crear_nodo_num(num);
    }
    else if(elementos[i]->tipo == NODO_NUM && tipo_a_mutar == STRING){ 
      char str[256];
      sprintf(str, "%d", elementos[i]->valor.entero);
      free(elementos[i]);
      elementos[i] = Crear_nodo_string(str);
    }
  }
}



