#ifndef TOKEN_H 
#define TOKEN_H 


typedef enum {
  TOKEN_VAR, TOKEN_NUM, TOKEN_IDEN, TOKEN_LPAR, TOKEN_RPAR, TOKEN_MAS, TOKEN_MENOS, 
  TOKEN_MUL, TOKEN_DIV, 
  TOKEN_L_LLAV, TOKEN_R_LLAV, TOKEN_PUNCOMA, TOKEN_EOF, TOKEN_ASIGNAR,
  TOKEN_FUNC, TOKEN_PRINT, TOKEN_IF, TOKEN_MAYOR, TOKEN_MENOR, TOKEN_IGUAL, TOKEN_MAYOR_IGUAL, TOKEN_MENOR_IGUAL, TOKEN_WHILE, TOKEN_STRING
}Tipo_token; 

typedef struct{ 
  Tipo_token tipo; 
  union{ 
    int entero;
    char frase[256];
  }valor;
}Token;

void Print_token(int t); 

typedef struct {
    int pos_actual;
    char* fuente;
    char char_actual;
} Lexer;

void Lexer_init(Lexer* lex, char* fuente);
void Lexer_avanzar(Lexer* lex);
Token Obtener_token(Lexer* lex);


#endif
