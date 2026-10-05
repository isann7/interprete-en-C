#ifndef PARSER_H
#define PARSER_H

#include "token.h"          
#include "ast.h"            
#include "tabla_simbolos.h" 

typedef struct {
    Lexer* lex;
    Token token_actual;
} Parser;

void Parser_init(Parser* par, char* fuente);
void Parser_avanzar(Parser* par);
void Parser_consumir(Parser* par, Tipo_token tipo_esperado);

Nodo* Parser_factor(Parser* par);
Nodo* Parser_termino(Parser* par);
Nodo* Parser_expresion(Parser* par);
Nodo* Parser_bloque(Parser* par);
Nodo* Parser_comparacion(Parser* par);
Nodo* Parser_instruccion(Parser* par);

#endif
