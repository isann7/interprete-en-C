#ifndef AST_H
#define AST_H
#include "tabla_simbolos.h"
#include "tabla_funciones.h"
#include "token.h"
 
typedef enum {
    OP_SUMA, OP_RESTA, OP_MUL, OP_DIV, OP_RESTO, OP_MAYOR, OP_MENOR, OP_MAYOR_IGUAL, OP_MENOR_IGUAL, OP_IGUAL, OP_AND, OP_OR, OP_XOR, OP_NOT
} Tipo_binario;

typedef struct {
    Tipo_binario tipo;
    struct Nodo* izq;
    struct Nodo* der;
} Binario;

typedef enum {
    NODO_ASIGNAR, NODO_PRINT, NODO_NUM, NODO_IDEN, NODO_BINARIO, NODO_IF, NODO_BLOQUE, NODO_WHILE, NODO_STRING, NODO_NOT, NODO_INPUT, NODO_FUNC_DEF, 
    NODO_FUNC_CALL, NODO_FINISH, NODO_ARRAY_INIT, NODO_ARRAY_ACCESO
} Tipo_nodo;

typedef struct Nodo {
    Tipo_nodo tipo;
    union {
        int entero;
        struct Nodo* print;
        char iden[50];
        char str[256];
        struct {
            struct Nodo* destino;
            struct Nodo* Nodo_asignado;
            Tipo_simbolo estricto;
        } asignacion;
        Binario binario;
        struct {
            struct Nodo* condicion;
            struct Nodo* rama_verdadera;
            struct Nodo* rama_falsa;
        } condicional;
        struct {
            struct Nodo* instrucciones[50];
            int cantidad;
        } bloque;
        struct Nodo* not;
        struct Nodo* input;
        struct{
            char nombre[50];
            char param[10][50];
            char param_salida[50];
            int num_param;
            Tipo_simbolo tipos_param[50];
            Tipo_simbolo tipo_salida;
            struct Nodo* bloque;
            TablaSimbolos entorno;
        }func_def;
        struct{
           char nombre[50];
           struct Nodo* args[10];
           int num_args; 
        }func_call;
        struct Nodo* finish;
        struct{
          struct Nodo** elementos;
          int cantidad;
          int capacidad;
        }array_init;
        struct{
          struct Nodo* base;
          struct Nodo* indice;
        }array_acceso;
    } valor;
} Nodo;

typedef struct{
  Tipo_simbolo tipo;
  Contenido valor;
}Evaluador;

Nodo* Crear_nodo_num(int num);
Nodo* Crear_nodo_asig(Nodo* destino, Nodo* expresion, Tipo_simbolo estricto);
Nodo* Crear_nodo_print(Nodo* n);
Nodo* Crear_nodo_iden(char* nombre);
Nodo* Crear_nodo_binario(Tipo_binario b, Nodo* izquierda, Nodo* derecha);
Nodo* Crear_nodo_bloque(void);
Nodo* Crear_nodo_if_while(Nodo* cond, Nodo* rama_verd, Nodo* rama_falsa, Tipo_token tipo);
Nodo* Crear_nodo_string(char* nombre);
Nodo* Crear_nodo_not(Nodo* expresion);
Nodo* Crear_nodo_input(void);
Nodo* Crear_nodo_func_def(char* nombre, Tipo_simbolo* tipos_param, char param[][50], Tipo_simbolo tipo_salida, char param_salida[50], int num_param, Nodo* bloque);
Nodo* Crear_nodo_func_call(char* nombre, Nodo** args, int num_args);
Nodo* Crear_nodo_finish(void);
Nodo* Crear_nodo_array_init(Nodo** n, int cantidad, int capacidad);
Nodo* Crear_nodo_array_acceso(Nodo* base, Nodo* indice);
Evaluador Evaluar_expresion(Nodo* nodo, TablaSimbolos* t, TablaFunciones* tf);
Evaluador Evaluar_instruccion(Nodo* nodo, TablaSimbolos* t, TablaFunciones* tf);
Nodo** Obtener_referencia_array(Nodo* destino, TablaSimbolos* t, TablaFunciones* tf);
int es_numero_entero(const char* str);

void Imprimir_ast(Nodo* nodo);

#endif 
