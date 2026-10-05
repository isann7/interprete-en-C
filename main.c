#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"
#include "tabla_simbolos.h"
#include "parser.h"


char* Leer_archivo(const char* ruta) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", ruta);
        exit(1);
    }

    
    fseek(archivo, 0, SEEK_END);
    long longitud = ftell(archivo);
    rewind(archivo);

    
    char* buffer = malloc(longitud + 1);
    if (buffer == NULL) {
        printf("Error: Memoria insuficiente\n");
        fclose(archivo);
        exit(1);
    }

    size_t leidos = fread(buffer, 1, longitud, archivo);
    buffer[leidos] = '\0'; 

    fclose(archivo);
    return buffer;
}

int main() {
    TablaSimbolos memoria;
    Tabla_init(&memoria);

    
    char* codigo_fuente = Leer_archivo("programa.txt");

    
    Parser par;
    par.lex = malloc(sizeof(Lexer));
    Parser_init(&par, codigo_fuente);

    printf("--- Ejecutando Archivo ---\n");

    
    while (par.token_actual.tipo != TOKEN_EOF) {
        Nodo* ast = Parser_instruccion(&par);
        Evaluar_instruccion(ast, &memoria);
    }

    
    free(par.lex);
    free(codigo_fuente);

    return 0;
}
