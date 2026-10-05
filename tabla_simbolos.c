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
        printf("error, no existe el simbolo %s\n", nombre); 
        exit(1);
    }
    return t->tabla[idx];
}

void Guardar_simbolo(TablaSimbolos* t, char* nombre, int valor) {
    int idx = Existe_simbolo(t, nombre);
    if (idx == -1) {
        strcpy(t->tabla[t->cantidad].nombre, nombre); 
        t->tabla[t->cantidad].valor = valor;
        t->cantidad++;
    } else {
        t->tabla[idx].valor = valor;
    }
}
