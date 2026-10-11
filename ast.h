#ifndef AST_H
#define AST_H
#include "tabla_simbolos.h"
 
typedef enum {
    OP_SUMA, OP_RESTA, OP_MUL, OP_DIV, OP_RESTO, OP_MAYOR, OP_MENOR, OP_MAYOR_IGUAL, OP_MENOR_IGUAL, OP_IGUAL, OP_AND, OP_OR, OP_XOR, OP_NOT
} Tipo_binario;

typedef struct {
    Tipo_binario tipo;
    struct Nodo* izq;
    struct Nodo* der;
} Binario;

typedef enum {
    NODO_ASIGNAR, NODO_PRINT, NODO_NUM, NODO_IDEN, NODO_BINARIO, NODO_IF, NODO_BLOQUE, NODO_WHILE, NODO_STRING, NODO_NOT, NODO_INPUT 
} Tipo_nodo;

typedef struct Nodo {
    Tipo_nodo tipo;
    union {
        int entero;
        struct Nodo* print;
        char iden[50];
        char str[256];
        struct {
            char nombre[50];
            struct Nodo* Nodo_asignado;
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
    } valor;
} Nodo;

Nodo* Crear_nodo_num(int num);
Nodo* Crear_nodo_asig(char* nombre, Nodo* expresion);
Nodo* Crear_nodo_print(Nodo* n);
Nodo* Crear_nodo_iden(char* nombre);
Nodo* Crear_nodo_binario(Tipo_binario b, Nodo* izquierda, Nodo* derecha);
Nodo* Crear_nodo_bloque(void);
Nodo* Crear_nodo_if_while(Nodo* cond, Nodo* rama_verd, Nodo* rama_falsa, int tipo);
Nodo* Crear_nodo_string(char* nombre);
Nodo* Crear_nodo_not(Nodo* expresion);
Nodo* Crear_nodo_input();
int Evaluar_expresion(Nodo* nodo, TablaSimbolos* t);
void Evaluar_instruccion(Nodo* nodo, TablaSimbolos* t);

void Imprimir_ast(Nodo* nodo);

#endif 
