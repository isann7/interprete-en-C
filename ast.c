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

Nodo* Crear_nodo_asig(char* nombre, Nodo* expresion) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = NODO_ASIGNAR;
    strcpy(nodo->valor.asignacion.nombre, nombre);
    nodo->valor.asignacion.Nodo_asignado = expresion;
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

Nodo* Crear_nodo_if_while(Nodo* cond, Nodo* rama_verd, int tipo) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->tipo = (tipo == 0 ? NODO_IF : NODO_WHILE);
    nodo->valor.condicional.condicion = cond;
    nodo->valor.condicional.rama_verdadera = rama_verd;
    return nodo;
}

Nodo* Crear_nodo_string(char* nombre){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo->tipo = NODO_STRING;
  strcpy(nodo->valor.str, nombre);
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
        case NODO_PRINT:
            printf("Nodo Print\n");
            printf("  |__ Expresion: ");
            Imprimir_ast(nodo->valor.print);
            break;
        case NODO_BINARIO:
            printf("Operacion Binaria (%s)\n", 
                nodo->valor.binario.tipo == OP_SUMA ? "+" :
                nodo->valor.binario.tipo == OP_RESTA ? "-" :
                nodo->valor.binario.tipo == OP_MUL ? "*" : "/");
            printf("  |__ Izq: ");
            Imprimir_ast(nodo->valor.binario.izq);
            printf("  |__ Der: ");
            Imprimir_ast(nodo->valor.binario.der);
            break;
        default:
            printf("Otros Nodos (Bloque/IF/WHILE)\n");
            break;
    }
}


int Evaluar_expresion(Nodo* nodo, TablaSimbolos* t) {
    if (nodo == NULL) return 0;
    switch (nodo->tipo) {
        case NODO_NUM:
            return nodo->valor.entero;
        case NODO_IDEN: {
            Simbolo s = Obtener_simbolo(t, nodo->valor.iden);
            return s.valor.entero;
        }
        case NODO_BINARIO: {
            int izq = Evaluar_expresion(nodo->valor.binario.izq, t);
            int der = Evaluar_expresion(nodo->valor.binario.der, t);
            switch (nodo->valor.binario.tipo) {
                case OP_SUMA:         return izq + der;
                case OP_RESTA:        return izq - der;
                case OP_MUL:          return izq * der;
                case OP_DIV:   
                    if (der == 0) { printf("Error: división por cero\n"); exit(1); }
                    return izq / der;
                case OP_MENOR:        return izq < der;
                case OP_MAYOR:        return izq > der;
                case OP_IGUAL:        return izq == der;
                case OP_MENOR_IGUAL:  return izq <= der;
                case OP_MAYOR_IGUAL:  return izq >= der;
                default: break;
            }
            break;
        }
        default:
            printf("tipo de nodo no valido en expresion\n");
            exit(1);
    }
    return 0;
}

void Evaluar_instruccion(Nodo* nodo, TablaSimbolos* t) {
    if (nodo == NULL) return;
    switch (nodo->tipo) {
        case NODO_ASIGNAR: {
            if(nodo->valor.asignacion.Nodo_asignado->tipo == NODO_STRING){
              Guardar_simbolo(t, nodo->valor.asignacion.nombre, 1, 0, nodo->valor.asignacion.Nodo_asignado->valor.str);
              break;
            } 
            int result = Evaluar_expresion(nodo->valor.asignacion.Nodo_asignado, t);
            Guardar_simbolo(t, nodo->valor.asignacion.nombre, 0, result, "");
            break;
        }
        case NODO_PRINT: { 
            if(nodo->valor.print->tipo == NODO_STRING){
              printf("%s\n", nodo->valor.print->valor.str);
              break;
            }

            if(nodo->valor.print->tipo == NODO_IDEN){
              Simbolo s = Obtener_simbolo(t, nodo->valor.print->valor.iden);
              if(s.es_str){
                printf("%s\n", s.valor.texto);
                break;
              }
            }

            int result = Evaluar_expresion(nodo->valor.print, t);
            printf("%d\n", result);
            break;
        }
        case NODO_BLOQUE: {
            for (int i = 0; i < nodo->valor.bloque.cantidad; i++) {
                Evaluar_instruccion(nodo->valor.bloque.instrucciones[i], t);
            }
            break;
        }
        case NODO_IF: {
            if (Evaluar_expresion(nodo->valor.condicional.condicion, t)) {
                Evaluar_instruccion(nodo->valor.condicional.rama_verdadera, t);
            }
            break;
        }
        case NODO_WHILE: {
            while (Evaluar_expresion(nodo->valor.condicional.condicion, t)) {
                Evaluar_instruccion(nodo->valor.condicional.rama_verdadera, t);
            }
            break;
        }
        default: break;
    }
}
