#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "funciones_recursivas.h"
#include <ctype.h>

Nodo* Crear_nodo_num(int num, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_NUM;
    nodo->valor.entero = num;
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_asig(Nodo* destino, Nodo* expresion, Tipo_simbolo estricto, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_ASIGNAR;
    nodo->valor.asignacion.Nodo_asignado = expresion;
    nodo->valor.asignacion.destino = destino;
    nodo->valor.asignacion.estricto = estricto;
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_print(Nodo* n, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_PRINT;
    nodo->valor.print = n;
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_iden(char* nombre, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_IDEN;
    strcpy(nodo->valor.iden, nombre);
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_binario(Tipo_binario b, Nodo* izquierda, Nodo* derecha, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_BINARIO;
    nodo->valor.binario.izq = izquierda;
    nodo->valor.binario.der = derecha;
    nodo->valor.binario.tipo = b;
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_bloque(int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_BLOQUE;
    nodo->valor.bloque.cantidad = 0;
    memset(&(nodo->valor.bloque.instrucciones), 0, sizeof(nodo->valor.bloque.instrucciones));
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_if_while(Nodo* cond, Nodo* rama_verd, Nodo* rama_falsa, Tipo_token tipo, int linea) {
    Nodo* nodo = malloc(sizeof(Nodo));
    Tipo_nodo t;
    switch ((int) tipo) {
      case TOKEN_IF: t = NODO_IF; break;
      case TOKEN_WHILE: t = NODO_WHILE; break;
    }
    nodo->tipo = t;
    nodo->valor.condicional.condicion = cond;
    nodo->valor.condicional.rama_verdadera = rama_verd;
    nodo->valor.condicional.rama_falsa = rama_falsa;
    nodo->linea = linea;
    return nodo;
}

Nodo* Crear_nodo_string(char* nombre, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_STRING;
  strcpy(nodo->valor.str, nombre);
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_not(Nodo* expresion, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_NOT;
  nodo->valor.not = expresion;
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_input(int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_INPUT;
  nodo->linea = linea;
  return nodo;
}


Nodo* Crear_nodo_func_def(char* nombre, Tipo_simbolo* tipos_param, char param[][50], Tipo_simbolo tipo_salida, char param_salida[50], int num_param, Nodo* bloque, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FUNC_DEF;
  strcpy(nodo->valor.func_def.nombre, nombre);
  for (unsigned int i = 0; i < num_param; i++) {
    strcpy(nodo->valor.func_def.param[i], param[i]);
    nodo->valor.func_def.tipos_param[i] = tipos_param[i];
  } 
  strcpy(nodo->valor.func_def.param_salida, param_salida);
  nodo->valor.func_def.num_param = num_param;
  nodo->valor.func_def.tipo_salida = tipo_salida;
  nodo->valor.func_def.bloque = bloque;
  nodo->valor.func_def.entorno = malloc(sizeof(TablaSimbolos));
  nodo->valor.func_def.entorno->cantidad = 0;
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_func_call(char* nombre, Nodo** args, int num_args, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FUNC_CALL;
  strcpy(nodo->valor.func_call.nombre, nombre);
  for(unsigned int i = 0; i < num_args; i++){
    nodo->valor.func_call.args[i] = args[i];
  }
  nodo->valor.func_call.num_args = num_args;
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_finish(int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_FINISH;
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_array_init(Nodo** n, int cantidad, int capacidad, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->valor.array_init.cantidad = cantidad;
  nodo->valor.array_init.capacidad = capacidad;
  nodo->tipo = NODO_ARRAY_INIT; 
  nodo->valor.array_init.elementos = n;
  nodo->linea = linea;
  return nodo;
}
 
Nodo* Crear_nodo_array_acceso(Nodo* base, Nodo* indice, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_ARRAY_ACCESO; 
  nodo->valor.array_acceso.base = base;
  nodo->valor.array_acceso.indice = indice;
  nodo->linea = linea;
  return nodo;
}

Nodo* Crear_nodo_values(Nodo* expresion, Nodo** casos, Nodo** bloques, int cantidad_casos, Nodo* caso_default, int linea){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_VALUES;
  nodo->valor.values.expresion = expresion;
  nodo->valor.values.casos = casos;
  nodo->valor.values.bloques = bloques;
  nodo->valor.values.cantidad_casos = cantidad_casos;
  nodo->valor.values.caso_default = caso_default;
  nodo->linea = linea;
  return nodo;
}

void Imprimir_ast(Nodo* nodo) {
    if (nodo == NULL) return;
    switch (nodo->tipo) {
        case NODO_ASIGNAR:
            printf("Nodo Asignacion\n");
            printf("  |__ Destino: ");
            Imprimir_ast(nodo->valor.asignacion.destino);
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
            printf("Acceso a Array\n");
            printf("  |__ Base: ");
            Imprimir_ast(nodo->valor.array_acceso.base);
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

int es_numero_entero(const char* str){
  if(str == NULL || *str == '\0'){
    return 0;
  }
  int i = 0;

  if(str[0] == '-' || str[0] == '+'){
    i++;
  }
  
  if(str[i] == '\0'){
    return 0;
  }

  for(; str[i] != '\0'; i++){
    if(!isdigit((unsigned char) str[i])){
      return 0;
    }
  }
  return 1;
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
              printf("Linea %i: intentando operar con tipos distintos", nodo->linea);
              exit(1);
            }
            else if(ev_izq.tipo == ENTERO){
              int izq = ev_izq.valor.entero;
              int der = ev_der.valor.entero;
              ev.tipo = ENTERO;
              switch (nodo->valor.binario.tipo) {
                  case OP_SUMA:         ev.valor.entero = izq + der; break;
                  case OP_RESTA:        ev.valor.entero = izq - der; break;
                  case OP_MUL:          ev.valor.entero = izq * der; break;
                  case OP_DIV:   
                      if (der == 0) { printf("Error: división por cero en linea %i", nodo->valor.binario.der->linea); exit(1); }
                      ev.valor.entero = izq / der; break;
                  case OP_RESTO:        ev.valor.entero = izq % der;  break;
                  case OP_MENOR:        ev.valor.entero = izq < der;  break;
                  case OP_MAYOR:        ev.valor.entero = izq > der;  break;
                  case OP_IGUAL:        ev.valor.entero = izq == der; break;
                  case OP_NO_IGUAL:     ev.valor.entero = izq != der; break;
                  case OP_MENOR_IGUAL:  ev.valor.entero = izq <= der; break;
                  case OP_MAYOR_IGUAL:  ev.valor.entero = izq >= der; break;
                  case OP_AND:          ev.valor.entero = izq && der; break;
                  case OP_OR:           ev.valor.entero = izq || der; break;
                  case OP_XOR:          ev.valor.entero = izq ^ der;  break;
                  case OP_L_SHIFT:      ev.valor.entero = izq << der; break;
                  case OP_R_SHIFT:      ev.valor.entero = izq >> der; break;
                  case OP_AND_BITABIT:  ev.valor.entero = izq & der;  break;
                  case OP_XOR_BITABIT:  ev.valor.entero = izq ^ der;  break;
                  case OP_OR_BITABIT:   ev.valor.entero = izq | der;  break;
                  default: break;
              }
            }
              else if(ev_izq.tipo == STRING){
                switch (nodo->valor.binario.tipo) {
                  case OP_SUMA: strcpy(ev.valor.texto, strcat(ev_izq.valor.texto, ev_der.valor.texto)); 
                                ev.tipo = STRING; break;
                  case OP_IGUAL: ev.valor.entero = !strcmp(ev_izq.valor.texto, ev_der.valor.texto); 
                                 ev.tipo = ENTERO; break;
                  case OP_NO_IGUAL: ev.valor.entero = strcmp(ev_izq.valor.texto, ev_der.valor.texto);
                                  ev.tipo = ENTERO; break;
                  default:
                                printf("Linea %i: Operación no definida para strings", nodo->valor.binario.izq->linea); exit(1);
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

          char buff[256];
          if(fgets(buff, sizeof(buff), stdin) != NULL){
            buff[strcspn(buff, "\n")] = 0;
            if(es_numero_entero(buff)){
              ev.tipo = ENTERO;
              ev.valor.entero = atoi(buff);
            }
            else{
              ev.tipo = STRING;
              strcpy(ev.valor.texto, buff);
            }
          }
          else {
            ev.tipo = STRING;
            strcpy(ev.valor.texto, "");
          }
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
                printf("Linea %i: tipo no valido", nodo->linea);
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
                  mutar_elementos_array(arg.valor.array.elementos, arg.valor.array.cantidad, ENTERO);
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
                  mutar_elementos_array(arg.valor.array.elementos, arg.valor.array.cantidad, STRING);
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
            else if(!(strcmp(f.nombre, "READ"))){
              char* ruta = arg.valor.texto;
              FILE* archivo = fopen(ruta, "r");
              if(archivo == NULL){
                printf("Linea %i: No se pudo abrir el archivo '%s'", nodo->linea, ruta);
                strcpy(ev.valor.texto, "");
                ev.tipo = STRING;
              }
              else{
                fseek(archivo, 0, SEEK_END);
                long tamaño = ftell(archivo);
                rewind(archivo);

                char* buffer = malloc(tamaño + 1);

                if(buffer == NULL){
                  printf("Error al reservar memoria");
                  exit(1);
                }

                fread(buffer, 1, tamaño, archivo);
                buffer[tamaño] = '\0';

                fclose(archivo);

                strncpy(ev.valor.texto, buffer, 4095);
                ev.valor.texto[4095] = '\0';
                ev.tipo = STRING;
                free(buffer);
              }
            }
            else if(!(strcmp(f.nombre, "WRITE"))){ 
              Evaluador arg2 = Evaluar_expresion(nodo->valor.func_call.args[1], t, tf);
              char* ruta = arg.valor.texto;
              char* contenido = arg2.valor.texto;
              FILE* archivo = fopen(ruta, "w");
              if(archivo == NULL){
                printf("Linea %i: No se pudo abrir el archivo '%s'", nodo->linea, ruta);
                ev.tipo = ENTERO;
                ev.valor.entero = 0;
              }
              else{
                fputs(contenido, archivo);
                fclose(archivo);
                ev.tipo = ENTERO;
                ev.valor.entero = 1;
              }

            }
            else if(!(strcmp(f.nombre, "APPEND"))){
              Evaluador arg2 = Evaluar_expresion(nodo->valor.func_call.args[1], t, tf);
              char* ruta = arg.valor.texto;
              char* contenido = arg2.valor.texto;
              FILE* archivo = fopen(ruta, "a");
              if(archivo == NULL){
                printf("Linea %i: No se pudo abrir el archivo '%s'", nodo->linea, ruta);
                ev.tipo = ENTERO;
                ev.valor.entero = 0;
              }
              else{
                fputs(contenido, archivo);
                fclose(archivo);
                ev.tipo = ENTERO;
                ev.valor.entero = 1;
              }
            }
          }
          else{
            Nodo* func_def = f.func_def;
            int i = 0;
            TablaSimbolos* ttemp = malloc(sizeof(TablaSimbolos));
            Tabla_init(ttemp);
            Contenido v;  
            Tipo_simbolo tipo_init = ENTERO;
            if (func_def->valor.func_def.tipo_salida == STRING) {
                strcpy(v.texto, ""); 
                tipo_init = STRING;
            } 
            else if (func_def->valor.func_def.tipo_salida == ARRAY) {
                v.array.cantidad = 0; 
                v.array.capacidad = 10;
                v.array.elementos = malloc(10 * sizeof(Nodo*));
                tipo_init = ARRAY;
            } 
            else {
                v.entero = 0;
            }
            
            Guardar_simbolo(ttemp, func_def->valor.func_def.param_salida, v, tipo_init, 
                            func_def->valor.func_def.tipo_salida);
            
            while(i < nodo->valor.func_call.num_args){
              Evaluador ev_t = Evaluar_expresion(nodo->valor.func_call.args[i], t, tf);
              Guardar_simbolo(ttemp, func_def->valor.func_def.param[i], ev_t.valor, ev_t.tipo, 
                              func_def->valor.func_def.tipos_param[i]);
              i++;
            }
            i = 0;
            while(i < func_def->valor.func_def.entorno->cantidad){
              Guardar_simbolo(ttemp, func_def->valor.func_def.entorno->tabla[i].nombre,
                                      func_def->valor.func_def.entorno->tabla[i].valor, 
                                      func_def->valor.func_def.entorno->tabla[i].tipo,
                                      func_def->valor.func_def.entorno->tabla[i].estricto);
              i++;
            }
            Evaluador ev_t = Evaluar_instruccion(func_def->valor.func_def.bloque, ttemp, tf); 
            Simbolo s = Obtener_simbolo(ttemp, func_def->valor.func_def.param_salida);
            ev.tipo = s.tipo;
            ev.valor = s.valor;
            Borrar_tabla(ttemp); 
            free(ttemp);
            }
            break;} 
        case NODO_STRING:{
          ev.tipo = STRING;
          strcpy(ev.valor.texto, nodo->valor.str);
          break;}
        case NODO_ARRAY_INIT:{
          Nodo** elementos_evaluados = malloc(nodo->valor.array_init.capacidad * sizeof(Nodo*));
          for(int i = 0; i < nodo->valor.array_init.capacidad; i++){
            elementos_evaluados[i] = Crear_nodo_num(0, nodo->linea);
          }
          for (int i = 0; i < nodo->valor.array_init.cantidad; i++) {
            Evaluador ev_elem = Evaluar_expresion(nodo->valor.array_init.elementos[i], t, tf);
            if (ev_elem.tipo == ENTERO) {
                elementos_evaluados[i] = Crear_nodo_num(ev_elem.valor.entero, nodo->linea);
            } 
            else if (ev_elem.tipo == STRING) {
                elementos_evaluados[i] = Crear_nodo_string(ev_elem.valor.texto, nodo->linea);   
            }
            else if (ev_elem.tipo == ARRAY){
              elementos_evaluados[i] = Crear_nodo_array_init(ev_elem.valor.array.elementos, ev_elem.valor.array.cantidad, ev_elem.valor.array.capacidad, nodo->linea);
            } 
          }
          ev.valor.array.elementos = elementos_evaluados;
          ev.valor.array.cantidad = nodo->valor.array_init.cantidad;
          ev.valor.array.capacidad = nodo->valor.array_init.capacidad;
          ev.tipo = ARRAY;
          break;}
        case NODO_ARRAY_ACCESO:{ 
            Evaluador base = Evaluar_expresion(nodo->valor.array_acceso.base, t, tf);
            Evaluador indice = Evaluar_expresion(nodo->valor.array_acceso.indice, t, tf);
            
            if (indice.tipo != ENTERO) {
                printf("Linea %i: el indice debe ser un numero entero.", nodo->linea);
                exit(1);
            }
            int idx = indice.valor.entero;
            
            if(base.tipo == ARRAY){
              if(idx >= 0 && idx < base.valor.array.cantidad){
                Nodo* elemento = base.valor.array.elementos[idx]; 
                ev = Evaluar_expresion(elemento, t, tf);
              }
              else{
                ev.tipo = ENTERO;
                ev.valor.entero = 0;
              }
            }
            else if(base.tipo == STRING){
              if(idx < 0 || idx >= strlen(base.valor.texto)){
                  printf("Linea %i: Indice %d fuera de rango.", nodo->linea, idx);
                  exit(1);
              }
              ev.tipo = STRING;
              ev.valor.texto[0] = base.valor.texto[idx];
              ev.valor.texto[1] = '\0';
            }
            break;
        }
        default:
            printf("Linea %i: tipo de nodo no valido en expresion\n", nodo->linea);
            exit(1);
    }
    return ev;
}

Nodo** Obtener_referencia_array(Nodo* destino, TablaSimbolos* t, TablaFunciones* tf) {
    if (destino->tipo == NODO_ARRAY_ACCESO) {
        Evaluador ind_ev = Evaluar_expresion(destino->valor.array_acceso.indice, t, tf);
        int idx = ind_ev.valor.entero;

        if (destino->valor.array_acceso.base->tipo == NODO_IDEN) {
            Simbolo* s = Obtener_simbolo_ptr(t, destino->valor.array_acceso.base->valor.iden);
            if (s->tipo == ARRAY) {
                if (idx >= 0) {
                  if (idx >= s->valor.array.capacidad) {
                    int nueva_cap = s->valor.array.capacidad;
                    while(idx >= nueva_cap) nueva_cap *= 2;
                    s->valor.array.elementos = realloc(s->valor.array.elementos, sizeof(Nodo*) * nueva_cap);
                    for(int j = s->valor.array.capacidad; j < nueva_cap; j++){
                      s->valor.array.elementos[j] = Crear_nodo_num(0, destino->linea);
                    }
                    s->valor.array.capacidad = nueva_cap;
                  }
                    if (idx >= s->valor.array.cantidad) {
                        s->valor.array.cantidad = idx + 1;
                    }
                    return &(s->valor.array.elementos[idx]);
                }
            }
        } 
        else if (destino->valor.array_acceso.base->tipo == NODO_ARRAY_ACCESO) {
            Nodo** ref_base = Obtener_referencia_array(destino->valor.array_acceso.base, t, tf);
            if (ref_base != NULL) {
                Nodo* nodo_array = *ref_base; 
                if (nodo_array != NULL && nodo_array->tipo == NODO_ARRAY_INIT) {
                    if (idx >= 0) {
                       if (idx >= nodo_array->valor.array_init.capacidad) {
                         int nueva_cap = nodo_array->valor.array_init.capacidad;
                         while(idx >= nueva_cap) nueva_cap *= 2;
                         nodo_array->valor.array_init.elementos = realloc(nodo_array->valor.array_init.elementos, sizeof(Nodo*) * nueva_cap);
                         for(int j = nodo_array->valor.array_init.capacidad; j < nueva_cap; j++){
                            nodo_array->valor.array_init.elementos[j] = Crear_nodo_num(0, destino->linea);                            
                         }
                        nodo_array->valor.array_init.capacidad = nueva_cap;
                       } 
                        if (idx >= nodo_array->valor.array_init.cantidad) {
                            nodo_array->valor.array_init.cantidad = idx + 1;
                        }
                        return &(nodo_array->valor.array_init.elementos[idx]);
                    }
                }
            }
        }
    }
    return NULL;
}

void Borrar_tabla(TablaSimbolos* t){
  if (t == NULL) return;
  for (int i = 0; i < t->cantidad; i++) {
    if (t->tabla[i].tipo == ARRAY) {
      for (int j = 0; j < t->tabla[i].valor.array.capacidad; j++) {          
        if (t->tabla[i].valor.array.elementos[j] != NULL) {
          Destruir_nodo(t->tabla[i].valor.array.elementos[j]);
        }
      }
            free(t->tabla[i].valor.array.elementos);
    }
  }
}

void Destruir_nodo(Nodo* nodo) {
    if (nodo == NULL) return;

    switch (nodo->tipo) {
        case NODO_ASIGNAR:
            Destruir_nodo(nodo->valor.asignacion.destino);
            Destruir_nodo(nodo->valor.asignacion.Nodo_asignado);
            break;
        case NODO_PRINT:
            Destruir_nodo(nodo->valor.print);
            break;
        case NODO_BINARIO:
            Destruir_nodo(nodo->valor.binario.izq);
            Destruir_nodo(nodo->valor.binario.der);
            break;
        case NODO_NOT:
            Destruir_nodo(nodo->valor.not);
            break;
        case NODO_BLOQUE:
            for (int i = 0; i < nodo->valor.bloque.cantidad; i++) {
                Destruir_nodo(nodo->valor.bloque.instrucciones[i]);
            }
            break;
        case NODO_IF:
        case NODO_WHILE:
            Destruir_nodo(nodo->valor.condicional.condicion);
            Destruir_nodo(nodo->valor.condicional.rama_verdadera);
            if (nodo->valor.condicional.rama_falsa != NULL) {
                Destruir_nodo(nodo->valor.condicional.rama_falsa);
            }
            break;
        case NODO_FUNC_DEF:
            Destruir_nodo(nodo->valor.func_def.bloque);
            if (nodo->valor.func_def.entorno != NULL) {
                Borrar_tabla(nodo->valor.func_def.entorno); 
                free(nodo->valor.func_def.entorno);           
            }
            break;
        case NODO_FUNC_CALL:
            for (int i = 0; i < nodo->valor.func_call.num_args; i++) {
                Destruir_nodo(nodo->valor.func_call.args[i]);
            }
            break;
        case NODO_ARRAY_INIT:
            for (int i = 0; i < nodo->valor.array_init.cantidad; i++) {
                if (nodo->valor.array_init.elementos[i] != NULL) {
                    Destruir_nodo(nodo->valor.array_init.elementos[i]);
                }
            }
            free(nodo->valor.array_init.elementos); 
            break;
        case NODO_ARRAY_ACCESO:
            Destruir_nodo(nodo->valor.array_acceso.base);
            Destruir_nodo(nodo->valor.array_acceso.indice);
            break;
        case NODO_VALUES:
            Destruir_nodo(nodo->valor.values.expresion);
            for (int i = 0; i < nodo->valor.values.cantidad_casos; i++) {
                Destruir_nodo(nodo->valor.values.casos[i]);
            }
            free(nodo->valor.values.casos);
            free(nodo->valor.values.bloques); 
            if (nodo->valor.values.caso_default != NULL) {
                Destruir_nodo(nodo->valor.values.caso_default);
            }
            break;
        case NODO_NUM:
        case NODO_STRING:
        case NODO_IDEN:
        case NODO_INPUT:
        case NODO_FINISH:
        default:
            break;
    }
    free(nodo);
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
            Nodo* destino = nodo->valor.asignacion.destino;
            if(destino->tipo == NODO_IDEN){
                Guardar_simbolo(t, destino->valor.iden, ev_t.valor, ev_t.tipo, nodo->valor.asignacion.estricto);  
            }
            else if(destino->tipo == NODO_ARRAY_ACCESO){
                Nodo** ref = Obtener_referencia_array(destino, t, tf);
                if (ref != NULL && *ref != NULL) {
                    if (ev_t.tipo == ENTERO) {
                        (*ref)->tipo = NODO_NUM;
                        (*ref)->valor.entero = ev_t.valor.entero;
                    } else if (ev_t.tipo == STRING) {
                        *ref = Crear_nodo_string(ev_t.valor.texto, nodo->linea);
                        (*ref)->tipo = NODO_STRING;
                        strcpy((*ref)->valor.str, ev_t.valor.texto);
                    } else if (ev_t.tipo == ARRAY) {
                        *ref = Crear_nodo_array_init(ev_t.valor.array.elementos, 
                                                     ev_t.valor.array.cantidad, 
                                                     ev_t.valor.array.capacidad, nodo->linea);
                    }
                } else {
                    Evaluador base_ev = Evaluar_expresion(destino->valor.array_acceso.base, t, tf);
                    Evaluador ind_ev = Evaluar_expresion(destino->valor.array_acceso.indice, t, tf);
                    if(base_ev.tipo == STRING){
                        Simbolo* s = Obtener_simbolo_ptr(t, destino->valor.array_acceso.base->valor.iden);
                        s->valor.texto[ind_ev.valor.entero] = ev_t.valor.texto[0];
                    }
                    else{
                      printf("Linea %i, asingacion invalida", nodo->linea);
                      exit(1);
                    }
                }
            }
            break;
        }
        case NODO_PRINT: { 
            Evaluador result = Evaluar_expresion(nodo->valor.print, t, tf);
            if (result.tipo == STRING) {
              printf("%s\n", result.valor.texto);
            } 
            else if (result.tipo == ENTERO) {
              printf("%d\n", result.valor.entero);
            }
            else if (result.tipo == ARRAY){
              print_array(result, t, tf);     
              printf("\n");
            }
            break;}
        case NODO_BLOQUE: {
            TablaSimbolos* tabla_bloque = malloc(sizeof(TablaSimbolos));;
            tabla_bloque->cantidad = 0;

            for(int i = 0; i < t->cantidad; i++){
              Guardar_simbolo(tabla_bloque,
                              t->tabla[i].nombre,
                              t->tabla[i].valor,
                              t->tabla[i].tipo,
                              t->tabla[i].estricto);
            }

            for (int i = 0; i < nodo->valor.bloque.cantidad; i++) {
               Evaluador j = Evaluar_instruccion(nodo->valor.bloque.instrucciones[i], tabla_bloque, tf);
               if(!(j.valor.entero)){ ev.valor.entero = 0; break;}
            }

            for(int i = 0; i < t->cantidad; i++){
              Simbolo s = Obtener_simbolo(tabla_bloque, t->tabla[i].nombre);
              t->tabla[i].valor = s.valor;
              t->tabla[i].tipo = s.tipo;
            }
            for (int i = t->cantidad; i < tabla_bloque->cantidad; i++) {
                if (tabla_bloque->tabla[i].tipo == ARRAY) {
                    for (int j = 0; j < tabla_bloque->tabla[i].valor.array.capacidad; j++) {          
                        if (tabla_bloque->tabla[i].valor.array.elementos[j] != NULL) {
                            Destruir_nodo(tabla_bloque->tabla[i].valor.array.elementos[j]);
                        }
                    }
                    free(tabla_bloque->tabla[i].valor.array.elementos);
                }
            }
            
            free(tabla_bloque);
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
            nodo->valor.func_def.entorno->cantidad = 0;
            for(int i = 0; i < t->cantidad; i++){
              Guardar_simbolo(nodo->valor.func_def.entorno,
                              t->tabla[i].nombre,
                              t->tabla[i].valor,
                              t->tabla[i].tipo,
                              t->tabla[i].estricto);
            }
            Guardar_func(tf, nodo->valor.func_def.nombre, nodo);
            break;} 
        case NODO_FINISH:{ ev.valor.entero = 0; break; }
        case NODO_FUNC_CALL:{
            Evaluador i = Evaluar_expresion(nodo, t, tf);
            break;}
        case NODO_VALUES:{
            Evaluador ev_expresion = Evaluar_expresion(nodo->valor.values.expresion, t, tf);
            int coincidencia = 0;

            for (int i = 0; i < nodo->valor.values.cantidad_casos; i++) {
              int match = 0;
              Evaluador ev_caso = Evaluar_expresion(nodo->valor.values.casos[i], t, tf);
              if(ev_expresion.tipo == ev_caso.tipo){
                if(ev_expresion.tipo == ENTERO && ev_expresion.valor.entero == ev_caso.valor.entero){
                  match = 1;
                }
                else if(ev_expresion.tipo == STRING && !strcmp(ev_expresion.valor.texto, ev_caso.valor.texto)){
                  match = 1;
                }
              }
              if (match){
                coincidencia = 1;
                Evaluador j = Evaluar_instruccion(nodo->valor.values.bloques[i], t, tf);
                if(!(j.valor.entero)){ ev.valor.entero = 0; }
                break;
              }
            }
            if(!coincidencia && nodo->valor.values.caso_default != NULL){
              Evaluador j = Evaluar_instruccion(nodo->valor.values.caso_default, t, tf);
              if(!(j.valor.entero)){ ev.valor.entero = 0; }
            }
            break;}

        default: break;
    }
    return ev;
}
