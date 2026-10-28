#ifndef FUNCIONES_RECURSIVAS_H 
#define FUNCIONES_RECURSIVAS_H


#include "ast.h"



void print_array(Evaluador result, TablaSimbolos* t, TablaFunciones* tf);
void mutar_elementos_array(Nodo** elementos, int cantidad, Tipo_simbolo tipo_a_mutar);



#endif
