#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Nodo* Crear_nodo_num(int num) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_NUM;
    nodo->valor.entero = num;
    return nodo;
}

Nodo* Crear_nodo_asig(char* nombre, Nodo* expresion, Nodo* indice) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_ASIGNAR;
    strcpy(nodo->valor.asignacion.nombre, nombre);
    nodo->valor.asignacion.Nodo_asignado = expresion;
    nodo->valor.asignacion.indice = indice;
    return nodo;
}

Nodo* Crear_nodo_print(Nodo* n) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_PRINT;
    nodo->valor.print = n;
    return nodo;
}

Nodo* Crear_nodo_iden(char* nombre) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_IDEN;
    strcpy(nodo->valor.iden, nombre);
    return nodo;
}

Nodo* Crear_nodo_binario(Tipo_binario b, Nodo* izquierda, Nodo* derecha) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_BINARIO;
    nodo->valor.binario.izq = izquierda;
    nodo->valor.binario.der = derecha;
    nodo->valor.binario.tipo = b;
    return nodo;
}

Nodo* Crear_nodo_bloque(void) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_BLOQUE;
    nodo->valor.bloque.cantidad = 0;
    memset(&(nodo->valor.bloque.instrucciones), 0, sizeof(nodo->valor.bloque.instrucciones));
    return nodo;
}

Nodo* Crear_nodo_if_while(Nodo* cond, Nodo* rama_verd, Nodo* rama_falsa, int tipo) {
    Nodo* nodo = malloc(sizeof(Nodo));
    Tipo_nodo t;
    switch (tipo) {
      case 0: t = NODO_IF; break;
      case 1: t = NODO_WHILE; break;
    }
    nodo->tipo = t;
    nodo->valor.condicional.condicion = cond;
    nodo->valor.condicional.rama_verdadera = rama_verd;
    nodo->valor.condicional.rama_falsa = rama_falsa;
    return nodo;
}

Nodo* Crear_nodo_string(char* nombre){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_STRING;
  strcpy(nodo->valor.str, nombre);
  return nodo;
}

Nodo* Crear_nodo_not(Nodo* expresion){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_NOT;
  nodo->valor.not = expresion;
  return nodo;
}

Nodo* Crear_nodo_input(void){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_INPUT;
  return nodo;
}

Nodo* Crear_nodo_func_def(char* nombre, char param[][50], char param_salida[50], int num_param, Nodo* bloque){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FUNC_DEF;
  strcpy(nodo->valor.func_def.nombre, nombre);
  for (unsigned int i = 0; i < num_param; i++) {
    strcpy(nodo->valor.func_def.param[i], param[i]);
  } 
  strcpy(nodo->valor.func_def.param_salida, param_salida);
  nodo->valor.func_def.num_param = num_param;
  nodo->valor.func_def.bloque = bloque;
  return nodo;
}

Nodo* Crear_nodo_func_call(char* nombre, Nodo** args, int num_args){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FUNC_CALL;
  strcpy(nodo->valor.func_call.nombre, nombre);
  for(unsigned int i = 0; i < num_args; i++){
    nodo->valor.func_call.args[i] = args[i];
  }
  nodo->valor.func_call.num_args = num_args;
  return nodo;
}

Nodo* Crear_nodo_finish(void){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FINISH;
  return nodo;
}

Nodo* Crear_nodo_array_init(Nodo** n, int cantidad, int capacidad){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->valor.array_init.cantidad = cantidad;
  nodo->valor.array_init.capacidad = capacidad;
  nodo->tipo = NODO_ARRAY_INIT; 
  nodo->valor.array_init.elementos = n;
  return nodo;
}
 
Nodo* Crear_nodo_array_acceso(char* nombre, Nodo* indice){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_ARRAY_ACCESO; 
  nodo->valor.array_acceso.indice = indice;
  strcpy(nodo->valor.array_acceso.nombre, nombre);
  return nodo;
}

void Imprimir_ast(Nodo* nodo) {
    if (nodo == NULL) return;
    switch (nodo->tipo) {
        case NODO_ASIGNAR:
            printf("Nodo Asignacion (variable: %s)\n", nodo->valor.asignacion.nombre);
            printf("  |__ Expresion: ");
            Imprimir_ast(nodo->valor.asignacion.Nodo_asignado);
            break;
        case NODO_NUM:
            printf("Numero (%d)\n", nodo->valor.entero);
            break;
        case NODO_IDEN:
            printf("Identificador (%s)\n", nodo->valor.iden);
            break;
        case NODO_STRING:
            printf("String (%s)\n", nodo->valor.str);
            break;
        case NODO_PRINT:
            printf("Nodo Print\n");
            printf("  |__ Expresion: ");
            Imprimir_ast(nodo->valor.print);
            break;
        case NODO_BINARIO:
            printf("Operacion Binaria\n");
            printf("  |__ Izq: ");
            Imprimir_ast(nodo->valor.binario.izq);
            printf("  |__ Der: ");
            Imprimir_ast(nodo->valor.binario.der);
            break;
        case NODO_ARRAY_INIT:
            printf("Inicializacion de Array (Elementos: %d)\n", nodo->valor.array_init.cantidad);
            for (int i = 0; i < nodo->valor.array_init.cantidad; i++) {
                printf("  |__ Elemento [%d]: ", i);
                Imprimir_ast(nodo->valor.array_init.elementos[i]);
            }
            break;
        case NODO_ARRAY_ACCESO:
            printf("Acceso a Array (variable: %s)\n", nodo->valor.array_acceso.nombre);
            printf("  |__ Indice: ");
            Imprimir_ast(nodo->valor.array_acceso.indice);
            break;
        case NODO_BLOQUE:
            printf("Nodo Bloque (Instrucciones: %d)\n", nodo->valor.bloque.cantidad);
            for (int i = 0; i < nodo->valor.bloque.cantidad; i++) {
                printf("  |__ [%d] ", i);
                Imprimir_ast(nodo->valor.bloque.instrucciones[i]);
            }
            break;
        case NODO_IF:
            printf("Nodo IF\n");
            printf("  |__ Condicion: ");
            Imprimir_ast(nodo->valor.condicional.condicion);
            printf("  |__ Rama Verdadeira: ");
            Imprimir_ast(nodo->valor.condicional.rama_verdadera);
            if (nodo->valor.condicional.rama_falsa != NULL) {
                printf("  |__ Rama Falsa: ");
                Imprimir_ast(nodo->valor.condicional.rama_falsa);
            }
            break;
        case NODO_WHILE:
            printf("Nodo WHILE\n");
            printf("  |__ Condicion: ");
            Imprimir_ast(nodo->valor.condicional.condicion);
            printf("  |__ Bloque: ");
            Imprimir_ast(nodo->valor.condicional.rama_verdadera);
            break;
        case NODO_FUNC_DEF:
            printf("Definicion de Funcion (%s)\n", nodo->valor.func_def.nombre);
            break;
        case NODO_FUNC_CALL:
            printf("Llamada a Funcion (%s)\n", nodo->valor.func_call.nombre);
            break;
        case NODO_FINISH:
            printf("Nodo FINISH\n");
            break;
        default:
            printf("Nodo Desconocido\n");
            break;
    }
}

Evaluador Evaluar_expresion(Nodo* nodo, TablaSimbolos* t, TablaFunciones* tf) {
    if (nodo == NULL) {
      Evaluador vacio; vacio.tipo = ENTERO; vacio.valor.entero = 0; 
      return vacio; 
    }
    Evaluador ev;
    switch (nodo->tipo) {
        case NODO_NUM:{
            ev.tipo = ENTERO;
            ev.valor.entero = nodo->valor.entero;
            break;}
        case NODO_IDEN:{
            Simbolo s = Obtener_simbolo(t, nodo->valor.iden);
            ev.valor = s.valor;
            ev.tipo = s.tipo;
            break;}
        case NODO_BINARIO: {
            Evaluador ev_izq = Evaluar_expresion(nodo->valor.binario.izq, t, tf);
            Evaluador ev_der = Evaluar_expresion(nodo->valor.binario.der, t, tf);
            if(ev_izq.tipo != ev_der.tipo){
              printf("intentando operar con tipos distintos");
              exit(1);
            }
            else if(ev_izq.tipo == ENTERO){
              int izq = ev_izq.valor.entero;
              int der = ev_der.valor.entero;
              switch (nodo->valor.binario.tipo) {
                  case OP_SUMA:         ev.valor.entero = izq + der; break;
                  case OP_RESTA:        ev.valor.entero = izq - der; break;
                  case OP_MUL:          ev.valor.entero = izq * der; break;
                  case OP_DIV:   
                      if (der == 0) { printf("Error: división por cero\n"); exit(1); }
                      ev.valor.entero = izq / der; break;
                  case OP_RESTO:        ev.valor.entero = izq % der;  break;
                  case OP_MENOR:        ev.valor.entero = izq < der;  break;
                  case OP_MAYOR:        ev.valor.entero = izq > der;  break;
                  case OP_IGUAL:        ev.valor.entero = izq == der; break;
                  case OP_MENOR_IGUAL:  ev.valor.entero = izq <= der; break;
                  case OP_MAYOR_IGUAL:  ev.valor.entero = izq >= der; break;
                  case OP_AND:          ev.valor.entero = izq && der; break;
                  case OP_OR:           ev.valor.entero = izq || der; break;
                  case OP_XOR:          ev.valor.entero = izq ^ der;  break;
                  default: break;
              }
            }
              else if(ev_izq.tipo == STRING){
                switch (nodo->valor.binario.tipo) {
                  case OP_SUMA: strcpy(ev.valor.texto, strcat(ev_izq.valor.texto, ev_der.valor.texto)); 
                                ev.tipo = STRING; break;
                  case OP_IGUAL: ev.valor.entero = !strcmp(ev_izq.valor.texto, ev_der.valor.texto); 
                                 ev.tipo = ENTERO; break;
                  default:
                                printf("Operación no definida para strings"); exit(1);
                }
              }
            break;
        }
        case NODO_NOT:{
          Evaluador valor = Evaluar_expresion(nodo->valor.not, t, tf);
          ev.valor.entero = !(valor.valor.entero);
          ev.tipo = ENTERO;
          break;}
        case NODO_INPUT:{
          printf(">> ");
          scanf("%d", &(ev.valor.entero));
          ev.tipo = ENTERO;
          break;} 
        case NODO_FUNC_CALL:{
          Func f = Obtener_func(tf, nodo->valor.func_call.nombre);
          if(f.es_nativa == 1){
            Evaluador arg = Evaluar_expresion(nodo->valor.func_call.args[0], t, tf); 
            if(!(strcmp(f.nombre, "LEN"))){
              ev.tipo = ENTERO;
              if(arg.tipo == ARRAY){
                ev.valor.entero = arg.valor.array.cantidad;
              }
              else if(arg.tipo == STRING){
                ev.valor.entero = strlen(arg.valor.texto);
              }
              else{
                printf("tipo no valido");
                exit(1);
              }
            } 
            else if(!(strcmp(f.nombre, "TYPE"))){
              ev.tipo = STRING;
              switch ((int) arg.tipo) {
                case ENTERO:
                  strcpy(ev.valor.texto, "ENTERO");
                  break;
                case STRING:
                  strcpy(ev.valor.texto, "STRING");
                  break;
                case ARRAY:
                  strcpy(ev.valor.texto, "ARRAY");
                  break;
              } 
            }
            else if(!(strcmp(f.nombre, "TO_INTEGER"))){
              switch ((int) arg.tipo) {
                case STRING:
                  ev.valor.entero = atoi(arg.valor.texto);
                  ev.tipo = ENTERO;
                  break;
                case ENTERO:
                  ev.valor = arg.valor;
                  ev.tipo = ENTERO;
                  break;
                case ARRAY:
                  for(int i = 0; i < arg.valor.array.cantidad; i++){
                    if(arg.valor.array.elementos[i]->tipo == NODO_STRING){
                      int num = atoi(arg.valor.array.elementos[i]->valor.str);
                      free(arg.valor.array.elementos[i]);
                      arg.valor.array.elementos[i] = Crear_nodo_num(num);
                    } 
                  }
                  ev.valor = arg.valor;
                  ev.tipo = ARRAY;
                  break;
              }
            }
            else if(!(strcmp(f.nombre, "TO_STRING"))){
              switch ((int) arg.tipo) {
                case STRING:
                  ev.valor = arg.valor;
                  ev.tipo = STRING;
                  break;
                case ENTERO:
                  sprintf(ev.valor.texto, "%d", arg.valor.entero);
                  ev.tipo = STRING;
                  break;
                case ARRAY:
                  for (int i = 0; i < arg.valor.array.cantidad; i++) {
                    if(arg.valor.array.elementos[i]->tipo == NODO_NUM){
                      char str[256];
                      sprintf(str, "%d", arg.valor.array.elementos[i]->valor.entero);
                      free(arg.valor.array.elementos[i]);
                      arg.valor.array.elementos[i] = Crear_nodo_string(str);
                    }
                  }
                  ev.valor = arg.valor;
                  ev.tipo = ARRAY;
                  break;
              }
            }
            else if(!(strcmp(f.nombre, "RAND"))){
              ev.tipo = ENTERO;
              if(arg.valor.entero != 0){
                ev.valor.entero = rand() % arg.valor.entero;
              }
              else{
                ev.valor.entero = rand();
              }
            }
          }
          else{
            Nodo* func_def = f.func_def;
            int i = 0;
            TablaSimbolos ttemp;
            Tabla_init(&ttemp);
            Contenido v; v.entero = 0;
            Guardar_simbolo(&ttemp, func_def->valor.func_def.param_salida, v, ENTERO);
            while(i < nodo->valor.func_call.num_args){
              Evaluador ev_t = Evaluar_expresion(nodo->valor.func_call.args[i], t, tf);
              Guardar_simbolo(&ttemp, func_def->valor.func_def.param[i], ev_t.valor, ev_t.tipo);
              i++;
            }
            Evaluador ev_t = Evaluar_instruccion(func_def->valor.func_def.bloque, &ttemp, tf); 
            Simbolo s = Obtener_simbolo(&ttemp, func_def->valor.func_def.param_salida);
            ev.tipo = s.tipo;
            ev.valor = s.valor;
            }
            break;}
        case NODO_STRING:{
          ev.tipo = STRING;
          strcpy(ev.valor.texto, nodo->valor.str);
          break;}
        case NODO_ARRAY_INIT:{
          Nodo** elementos_evaluados = malloc(nodo->valor.array_init.capacidad * sizeof(Nodo*));
          for(int i = 0; i < nodo->valor.array_init.capacidad; i++){
            elementos_evaluados[i] = Crear_nodo_num(0);
          }
          for (int i = 0; i < nodo->valor.array_init.cantidad; i++) {
            Evaluador ev_elem = Evaluar_expresion(nodo->valor.array_init.elementos[i], t, tf);
            if (ev_elem.tipo == ENTERO) {
                elementos_evaluados[i] = Crear_nodo_num(ev_elem.valor.entero);
             } else if (ev_elem.tipo == STRING) {
                 elementos_evaluados[i] = Crear_nodo_string(ev_elem.valor.texto);               
             }
            }
          ev.valor.array.elementos = elementos_evaluados;
          ev.valor.array.cantidad = nodo->valor.array_init.cantidad;
          ev.valor.array.capacidad = nodo->valor.array_init.capacidad;
          ev.tipo = ARRAY;
          break;}
        case NODO_ARRAY_ACCESO:{ 
          Simbolo s = Obtener_simbolo(t, nodo->valor.array_acceso.nombre);
            if (s.tipo != ARRAY && s.tipo != STRING) {
                printf("Error: La variable '%s' no es un array ni un string.\n", s.nombre);
                exit(1);
            }
            Evaluador indice = Evaluar_expresion(nodo->valor.array_acceso.indice, t, tf);
            if (indice.tipo != ENTERO) {
                printf("Error: El indice para '%s' debe ser un numero entero.\n", s.nombre);
                exit(1);
            }
            int idx = indice.valor.entero;
            if(s.tipo == ARRAY){
              if(idx >= 0){
                Nodo* elemento = s.valor.array.elementos[idx];
                if(idx >= s.valor.array.cantidad){
                s.valor.array.cantidad = idx;
                }
                ev = Evaluar_expresion(elemento, t, tf);
              }
            }
            else if(s.tipo == STRING){
              if(idx < 0 || idx > strlen(s.valor.texto)){
              printf("Error: Indice %d fuera de rango para el string '%s' (Longitud: %d).\n", idx, s.nombre, 
                      strlen(s.valor.texto));
              exit(1);
              }
              ev.tipo = STRING;
              ev.valor.texto[0] = s.valor.texto[idx];
              ev.valor.texto[1] = '\0';
            }
            break;}
        default:
            printf("tipo de nodo no valido en expresion\n");
            exit(1);
    }
    return ev;
}

Evaluador Evaluar_instruccion(Nodo* nodo, TablaSimbolos* t, TablaFunciones* tf) {
    if (nodo == NULL) {
      Evaluador vacio; vacio.tipo = ENTERO; vacio.valor.entero = 1; 
      return vacio; 
    }   
    Evaluador ev;
    ev.tipo = ENTERO;
    ev.valor.entero = 1;
    switch (nodo->tipo) {
        case NODO_ASIGNAR: {
            Evaluador ev_t = Evaluar_expresion(nodo->valor.asignacion.Nodo_asignado, t, tf);
            if(nodo->valor.asignacion.indice == NULL){
              Guardar_simbolo(t, nodo->valor.asignacion.nombre, ev_t.valor, ev_t.tipo);  
            }
            else{
              Simbolo* s = Obtener_simbolo_ptr(t, nodo->valor.asignacion.nombre);
              if(s->tipo == ARRAY){
                Evaluador ev_indice = Evaluar_expresion(nodo->valor.asignacion.indice, t, tf);  
                int idx = ev_indice.valor.entero; 
                if(idx >= s->valor.array.capacidad){
                  int nueva_capacidad = (idx + 1) * 2;
                  s->valor.array.elementos = realloc(s->valor.array.elementos, sizeof(Nodo*) * nueva_capacidad);
                  s->valor.array.capacidad = nueva_capacidad;
                }
                for(int i = s->valor.array.cantidad; i < idx; i++){
                    s->valor.array.elementos[i] = Crear_nodo_num(0);
                }
                switch ((int) ev_t.tipo) {
                  case ENTERO:
                    s->valor.array.elementos[idx] = Crear_nodo_num(ev_t.valor.entero);
                    break;
                  case STRING:
                    s->valor.array.elementos[idx] = Crear_nodo_string(ev_t.valor.texto);
                    break;
                }
                if (idx >= s->valor.array.cantidad) {
                    s->valor.array.cantidad = idx + 1;
                }
              }
              else if(s->tipo == STRING){
                Evaluador ev_indice = Evaluar_expresion(nodo->valor.asignacion.indice, t, tf);
                int idx = ev_indice.valor.entero;
                if(idx >= 256){
                  printf("Error, tamaño no soportado, máximo es 255\n");
                  exit(1);
                }
                if(idx >= strlen(s->valor.texto)){
                  for(int i = strlen(s->valor.texto); i < idx; i++){
                    s->valor.texto[i] = '0';
                  }
                  s->valor.texto[idx + 1] = '\0';
                } 
                s->valor.texto[idx] = ev_t.valor.texto[0];
              }
            }
            break;
        }
        case NODO_PRINT: { 
            Evaluador result = Evaluar_expresion(nodo->valor.print, t, tf);
            if (result.tipo == STRING) {
              printf("\"%s\"\n", result.valor.texto);
            } 
            else if (result.tipo == ENTERO) {
              printf("%d\n", result.valor.entero);
            }
            else if (result.tipo == ARRAY){
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
                if(i < result.valor.array.cantidad - 1){ printf(", "); }
                i++;
              }
              printf("]\n");
            }
            break;}
        case NODO_BLOQUE: {
            for (int i = 0; i < nodo->valor.bloque.cantidad; i++) {
               Evaluador j = Evaluar_instruccion(nodo->valor.bloque.instrucciones[i], t, tf);
               if(!(j.valor.entero)){ ev.valor.entero = 0; break;}
            }
            break;
        }
        case NODO_IF: {
            if ((Evaluar_expresion(nodo->valor.condicional.condicion, t, tf)).valor.entero) {
               Evaluador i = Evaluar_instruccion(nodo->valor.condicional.rama_verdadera, t, tf);
               if(!(i.valor.entero)){ ev.valor.entero = 0; break; }
            }
            else if(nodo->valor.condicional.rama_falsa != NULL){
              Evaluador i = Evaluar_instruccion(nodo->valor.condicional.rama_falsa, t, tf);
              if(!(i.valor.entero)){ ev.valor.entero = 0; break; }
            }
            break;
        } 
        case NODO_WHILE: {
            while ((Evaluar_expresion(nodo->valor.condicional.condicion, t, tf)).valor.entero) {
               Evaluador i = Evaluar_instruccion(nodo->valor.condicional.rama_verdadera, t, tf);
               if(!(i.valor.entero)){ ev.valor.entero = 0; break; }
            }
            break;
        } 
        case NODO_FUNC_DEF:{
            Guardar_func(tf, nodo->valor.func_def.nombre, nodo);
            break;} 
        case NODO_FINISH:{ ev.valor.entero = 0; break; }
        case NODO_FUNC_CALL:{
            Evaluador i = Evaluar_expresion(nodo, t, tf);
            break;}
        default: break;
    }
    return ev;
}
