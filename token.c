#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

void Lexer_init(Lexer* lex, char* fuente) {
    lex->fuente = fuente;
    lex->pos_actual = 0;
    lex->char_actual = lex->fuente[0];
}

void Lexer_avanzar(Lexer* lex) {
    lex->pos_actual++;
    lex->char_actual = lex->fuente[lex->pos_actual];
}

Token Obtener_token(Lexer* lex) {
    Token t;
    if(lex->char_actual == '\0'){
        t.tipo = TOKEN_EOF;
        return t;
    }
    while (lex->char_actual == ' ' || lex->char_actual == '\t' || lex->char_actual == '\n') {
        Lexer_avanzar(lex);
    }

    if(lex->char_actual == '\0'){
        t.tipo = TOKEN_EOF;
        return t;
    }

    if(!(isdigit(lex->char_actual)) && !(isalpha(lex->char_actual)) && lex->char_actual != '"' && lex->char_actual != '_'){
        switch (lex->char_actual) {
            case '+': t.tipo = TOKEN_MAS; Lexer_avanzar(lex); break;
            case '-': t.tipo = TOKEN_MENOS; Lexer_avanzar(lex); break;
            case '*': t.tipo = TOKEN_MUL; Lexer_avanzar(lex); break;
            case '/': t.tipo = TOKEN_DIV; Lexer_avanzar(lex); break;
            case '%': t.tipo = TOKEN_RESTO; Lexer_avanzar(lex); break;
            case '(': t.tipo = TOKEN_LPAR; Lexer_avanzar(lex); break;
            case ')': t.tipo = TOKEN_RPAR; Lexer_avanzar(lex); break;
            case '{': t.tipo = TOKEN_L_LLAV; Lexer_avanzar(lex); break;
            case '}': t.tipo = TOKEN_R_LLAV; Lexer_avanzar(lex); break;
            case '[': t.tipo = TOKEN_L_CORCHETE; Lexer_avanzar(lex); break;
            case ']': t.tipo = TOKEN_R_CORCHETE; Lexer_avanzar(lex); break;
            case ';': t.tipo = TOKEN_PUNCOMA; Lexer_avanzar(lex); break;
            case ':': t.tipo = TOKEN_ASIGNAR; Lexer_avanzar(lex); break;
            case ',': t.tipo = TOKEN_COMA; Lexer_avanzar(lex); break; 
            case '#':{
                while(lex->char_actual != '\n' && lex->char_actual != '\0'){
                  Lexer_avanzar(lex);      
                }
                return Obtener_token(lex);}
            case '<':
                Lexer_avanzar(lex);
                if(lex->char_actual == '='){ t.tipo = TOKEN_MENOR_IGUAL; Lexer_avanzar(lex); }
                else{ t.tipo = TOKEN_MENOR; }
                break;
            case '>':
                Lexer_avanzar(lex);
                if(lex->char_actual == '='){ t.tipo = TOKEN_MAYOR_IGUAL; Lexer_avanzar(lex); }
                else{ t.tipo = TOKEN_MAYOR; }
                break;
            case '=':{
              Lexer_avanzar(lex);
              if(lex->char_actual == '='){
                printf("Error: Sintaxis comparación: =, asignación: :");
                exit(1);
              }
              else{
                t.tipo = TOKEN_IGUAL;
                break;
              }
                     }
            default: Lexer_avanzar(lex); break;
            case '&': t.tipo = TOKEN_AND; Lexer_avanzar(lex); break;
            case '|': t.tipo = TOKEN_OR; Lexer_avanzar(lex); break;
            case '^': t.tipo = TOKEN_XOR; Lexer_avanzar(lex); break;
            case '~': t.tipo = TOKEN_NOT; Lexer_avanzar(lex); break;
        }
    }
    else if((isdigit(lex->char_actual))) {
        int n = 0;
        while (isdigit(lex->char_actual)) {
            n *= 10;
            n += (lex->char_actual - '0');
            Lexer_avanzar(lex);    
        }
        t.tipo = TOKEN_NUM;
        t.valor.entero = n;
    }
    else {
        char nombre[256];
        int i = 0;
        int str = 0;
        if(lex->char_actual == '"'){Lexer_avanzar(lex); str++;}
        while( (lex->char_actual != '"' && lex->char_actual != '\0' && lex->char_actual != '\n' && str) || ((isalnum(lex->char_actual) || lex->char_actual == '_') && !str) ){
            nombre[i] = lex->char_actual;
            i++;
            Lexer_avanzar(lex);
        } 
        if(lex->char_actual == '"'){
          Lexer_avanzar(lex); 
        } 
        else if(str){
          printf("Error: cadena sin cerrar\n");
          exit(3);
    }

    nombre[i] = '\0';

    if(!str){
        if(!strcmp("VAR", nombre)){ t.tipo = TOKEN_VAR; }
        else if(!strcmp("FUNC", nombre)){ t.tipo = TOKEN_FUNC; }
        else if(!strcmp("PRINT", nombre)){ t.tipo = TOKEN_PRINT; }
        else if(!strcmp("IF", nombre)){ t.tipo = TOKEN_IF; }
        else if(!strcmp("IFNOT", nombre)){ t.tipo = TOKEN_IFNOT; }
        else if(!strcmp("WHILE", nombre)){ t.tipo = TOKEN_WHILE; }
        else if(!strcmp("INPUT", nombre)){ t.tipo = TOKEN_INPUT; }
        else if(!strcmp("FUNC", nombre)){ t.tipo = TOKEN_FUNC; }
        else if(!strcmp("FINISH", nombre)){ t.tipo = TOKEN_FINISH; }
        else if(!strcmp("INT", nombre)){ t.tipo = TOKEN_TIPO_INT; }
        else if(!strcmp("STR", nombre)){ t.tipo = TOKEN_TIPO_STR; }
        else if(!strcmp("ARR", nombre)){ t.tipo = TOKEN_TIPO_ARR; }
        else {
            t.tipo = TOKEN_IDEN;
            strcpy(t.valor.frase, nombre);
        }
    } else {
        t.tipo = TOKEN_STRING;
        strcpy(t.valor.frase, nombre);
    } 
   }
    return t;
}

void Print_token(int t) {
    switch (t) {
        case TOKEN_VAR:         printf("token_var"); break;
        case TOKEN_NUM:         printf("token_num"); break;
        case TOKEN_IDEN:        printf("token_iden"); break;
        case TOKEN_LPAR:        printf("token_lpar"); break;
        case TOKEN_RPAR:        printf("token_rpar"); break;
        case TOKEN_MAS:         printf("token_mas"); break;
        case TOKEN_MENOS:       printf("token_menos"); break;
        case TOKEN_MUL:         printf("token_mul"); break;
        case TOKEN_DIV:         printf("token_div"); break;
        case TOKEN_L_LLAV:      printf("token_l_llav"); break;
        case TOKEN_R_LLAV:      printf("token_r_llav"); break;
        case TOKEN_PUNCOMA:     printf("token_puncoma"); break;
        case TOKEN_EOF:         printf("token_eof"); break;
        case TOKEN_ASIGNAR:     printf("token_asignar"); break;
        case TOKEN_FUNC:        printf("token_func"); break;
        case TOKEN_PRINT:       printf("token_print"); break;
        case TOKEN_IF:          printf("token_if"); break;
        case TOKEN_MAYOR:       printf("token_mayor"); break;
        case TOKEN_MENOR:       printf("token_menor"); break;
        case TOKEN_IGUAL:       printf("token_igual"); break;
        case TOKEN_MAYOR_IGUAL: printf("token_mayor_igual"); break;
        case TOKEN_MENOR_IGUAL: printf("token_menor_igual"); break;
        case TOKEN_WHILE:       printf("token_while"); break;
        case TOKEN_STRING:      printf("token_string"); break;
        case TOKEN_AND:         printf("token_and"); break;
        case TOKEN_OR:          printf("token_or"); break;
        case TOKEN_XOR:         printf("token_xor"); break;
        case TOKEN_NOT:         printf("token_not"); break;
        case TOKEN_R_CORCHETE:  printf("token_r_corchete"); break;
        case TOKEN_L_CORCHETE:  printf("token_l_corchete"); break;
        case TOKEN_COMA:        printf("token_coma"); break;  
        default:                printf("token_desconocido"); break;
    }
    printf("\n");
}

