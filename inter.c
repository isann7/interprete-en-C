#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"
#include "tabla_simbolos.h"
#include "parser.h"


void Ejecutar_programa(TablaSimbolos* memoria, char* fuente){
 Parser par;
 par.lex = malloc(sizeof(Lexer));
 Parser_init(&par, fuente);

 while (par.token_actual.tipo != TOKEN_EOF) {
   Nodo* ast = Parser_instruccion(&par);
   Evaluar_instruccion(ast, memoria);
 }

 free(par.lex);
}

int main() {
    TablaSimbolos memoria;
    Tabla_init(&memoria);
    
    char p[] = 
        "VAR diegolit9 : 67;\n"
        "WHILE (diegolit9 >= 0){\n"
        "   PRINT diegolit9;\n"
        "   VAR diegolit9 : diegolit9 - 1;"
        "}\n";    
    printf("--- Ejecutando Programa ---\n");
    Ejecutar_programa(&memoria, p);
        
    return 0;
}

