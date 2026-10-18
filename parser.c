#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void Parser_init(Parser* par, char* fuente){
  Lexer_init(par->lex, fuente);
  par->token_actual = Obtener_token(par->lex);  
} 

void Parser_avanzar(Parser* par){
  par->token_actual = Obtener_token(par->lex);
}

void Parser_consumir(Parser* par, Tipo_token tipo_esperado){
  if(par->token_actual.tipo == tipo_esperado){
    Parser_avanzar(par);
  }
  else{
    printf("error, token esperado: "); 
    Print_token(tipo_esperado);
    printf("token recibido: ");
    Print_token(par->token_actual.tipo);
    exit(1);
  }
}

Nodo* Parser_bloque(Parser* par){
  Parser_consumir(par, TOKEN_L_LLAV);
  Nodo* nodo = Crear_nodo_bloque();
  int len = sizeof(nodo->valor.bloque.instrucciones) / sizeof(Nodo*);
  for(;par->token_actual.tipo != TOKEN_R_LLAV; nodo->valor.bloque.cantidad++){
    if(nodo->valor.bloque.cantidad >= len && par->token_actual.tipo != TOKEN_R_LLAV){
      printf("error, demasiadas instrucciones dentro del condicional, instrucciones máximas: %d\n", len); 
      exit(2);      
    }
    nodo->valor.bloque.instrucciones[nodo->valor.bloque.cantidad] = Parser_instruccion(par);  
  }
  Parser_consumir(par, TOKEN_R_LLAV);
  return nodo;
}

Nodo* Parser_factor(Parser* par){
  Nodo* nodo; 
  switch ((int) par->token_actual.tipo) {
    case TOKEN_NUM:{
      int valor = par->token_actual.valor.entero;
      Parser_consumir(par, TOKEN_NUM);
      nodo = Crear_nodo_num(valor);
      break;}
    case TOKEN_IDEN:{
      char nombre[50];
      strcpy(nombre, par->token_actual.valor.frase); 
      Parser_consumir(par, TOKEN_IDEN);
      if(par->token_actual.tipo == TOKEN_LPAR){ 
        Parser_consumir(par, TOKEN_LPAR); 
        Nodo* argumentos[10]; 
        unsigned int i = 0; 
        while(par->token_actual.tipo != TOKEN_RPAR){ 
          argumentos[i] = Parser_logico_or(par);
          i++;  
          if(par->token_actual.tipo == TOKEN_COMA){
            Parser_consumir(par, TOKEN_COMA);
          }
        } 
        Parser_consumir(par, TOKEN_RPAR);  
        nodo = Crear_nodo_func_call(nombre, argumentos, i);
      }
      else if(par->token_actual.tipo == TOKEN_L_CORCHETE){
        Parser_consumir(par, TOKEN_L_CORCHETE);
        Nodo* indice = Parser_logico_or(par);
        Parser_consumir(par, TOKEN_R_CORCHETE);
        nodo = Crear_nodo_array_acceso(nombre, indice);
      }
      else{  
        nodo = Crear_nodo_iden(nombre);
      }
      break;
    } 
    case TOKEN_L_CORCHETE:{
        Parser_consumir(par, TOKEN_L_CORCHETE); 
        int capacidad = 10;
        int cantidad = 0;
        Nodo** n = malloc(sizeof(Nodo*) * capacidad);
        while(par->token_actual.tipo != TOKEN_R_CORCHETE){
          if(cantidad == capacidad){
            capacidad *= 2;
            n = realloc(n, sizeof(Nodo*) * capacidad);
          }
          if(par->token_actual.tipo == TOKEN_COMA){
            Parser_consumir(par, TOKEN_COMA);
          }
          n[cantidad++] = Parser_logico_or(par);
        }
        Parser_consumir(par, TOKEN_R_CORCHETE);
        nodo = Crear_nodo_array_init(n, cantidad, capacidad);
        break;}
    case TOKEN_LPAR:{
      Parser_consumir(par, TOKEN_LPAR);
      nodo = Parser_logico_or(par);
      Parser_consumir(par, TOKEN_RPAR);
      break;}
    case TOKEN_STRING:{
      char texto[256];
      strcpy(texto, par->token_actual.valor.frase);
      Parser_consumir(par, TOKEN_STRING);
      nodo = Crear_nodo_string(texto);
      break;}
    case TOKEN_NOT:{
      Parser_consumir(par, TOKEN_NOT);
      Nodo* exp = Parser_factor(par);
      nodo = Crear_nodo_not(exp);
      break;}
    case TOKEN_INPUT:{
      Parser_consumir(par, TOKEN_INPUT);
      nodo = Crear_nodo_input();
      break;}
  }
  return nodo;
}

Nodo* Parser_termino(Parser* par){
  Nodo* izq = Parser_factor(par);
  while(par->token_actual.tipo == TOKEN_MUL || par->token_actual.tipo == TOKEN_DIV || par->token_actual.tipo == TOKEN_RESTO){
    Tipo_binario op;
    switch ((int) par->token_actual.tipo) {
      case TOKEN_MUL: op = OP_MUL; break;
      case TOKEN_DIV: op = OP_DIV; break;
      case TOKEN_RESTO: op = OP_RESTO; break;
    }
    Parser_avanzar(par);
    Nodo* der = Parser_factor(par);
    izq = Crear_nodo_binario(op, izq, der);
  }
  return izq;
}

Nodo* Parser_expresion(Parser* par){
  Nodo* izq = Parser_termino(par);
  while (par->token_actual.tipo == TOKEN_MAS || par->token_actual.tipo == TOKEN_MENOS) { 
    Tipo_binario op = (par->token_actual.tipo == TOKEN_MAS ? OP_SUMA : OP_RESTA);
    Parser_avanzar(par);
    Nodo* der = Parser_termino(par);
    izq = Crear_nodo_binario(op, izq, der);
  }
  return izq;
}

Nodo* Parser_comparacion(Parser* par){
  Nodo* izq = Parser_expresion(par);
  while(par->token_actual.tipo == TOKEN_MENOR || par->token_actual.tipo == TOKEN_MAYOR || par->token_actual.tipo == TOKEN_IGUAL 
        || par->token_actual.tipo == TOKEN_MAYOR_IGUAL || par->token_actual.tipo == TOKEN_MENOR_IGUAL)
  {
    Tipo_binario op;
    switch ((int) par->token_actual.tipo) {
      case TOKEN_MENOR:       op = OP_MENOR; break;
      case TOKEN_MAYOR:       op = OP_MAYOR; break;
      case TOKEN_IGUAL:       op = OP_IGUAL; break;
      case TOKEN_MAYOR_IGUAL: op = OP_MAYOR_IGUAL; break;
      case TOKEN_MENOR_IGUAL: op = OP_MENOR_IGUAL; break;
    }
    Parser_avanzar(par);
    Nodo* der = Parser_expresion(par);
    izq = Crear_nodo_binario(op, izq, der);
  }
  return izq;
}

Nodo* Parser_logico_and(Parser* par){
  Nodo* izq = Parser_comparacion(par);
  while(par->token_actual.tipo == TOKEN_AND){
    Parser_avanzar(par);
    Nodo* der = Parser_comparacion(par);
    izq = Crear_nodo_binario(OP_AND, izq, der);
  }
  return izq;
}

Nodo* Parser_logico_or(Parser* par){
  Nodo* izq = Parser_logico_and(par);
  while(par->token_actual.tipo == TOKEN_OR || par->token_actual.tipo == TOKEN_XOR){
    Tipo_binario op = par->token_actual.tipo == TOKEN_OR ? OP_OR : OP_XOR;
    Parser_avanzar(par); 
    Nodo* der = Parser_logico_and(par);
    izq = Crear_nodo_binario(op, izq, der);
  }
  return izq;
}

Nodo* Parser_instruccion(Parser* par){
  Nodo* nodo; 
  switch ((int) par->token_actual.tipo) {
    case TOKEN_VAR:{
      Parser_consumir(par, TOKEN_VAR);
      char nombre[50];
      Nodo* indice = NULL;
      strcpy(nombre, par->token_actual.valor.frase);
      Parser_consumir(par, TOKEN_IDEN);
      if(par->token_actual.tipo == TOKEN_L_CORCHETE){
        Parser_consumir(par, TOKEN_L_CORCHETE);
        indice = Parser_logico_or(par);
        Parser_consumir(par, TOKEN_R_CORCHETE);
      }
      Parser_consumir(par, TOKEN_ASIGNAR);
      Nodo* exp = Parser_logico_or(par);
      Parser_consumir(par, TOKEN_PUNCOMA);
      nodo = Crear_nodo_asig(nombre, exp, indice);
      break;}
    case TOKEN_PRINT:{
      Parser_consumir(par, TOKEN_PRINT);
      Nodo* exp = Parser_logico_or(par);
      Parser_consumir(par, TOKEN_PUNCOMA);
      nodo = Crear_nodo_print(exp);
      break;} 
    case TOKEN_IF:
    case TOKEN_WHILE:{
      int tipo_actual = (par->token_actual.tipo == TOKEN_IF ? 0 : 1);
      Parser_consumir(par, par->token_actual.tipo == TOKEN_IF ? TOKEN_IF : TOKEN_WHILE);
      Nodo* condicion = Parser_logico_or(par);
      Nodo* bloque_verdadero = Parser_bloque(par);
      Nodo* bloque_falso = NULL;      
      if(tipo_actual == 0 && par->token_actual.tipo == TOKEN_IFNOT){ 
        Parser_consumir(par, TOKEN_IFNOT);
        bloque_falso = Parser_bloque(par);
      } 
      nodo = Crear_nodo_if_while(condicion, bloque_verdadero, bloque_falso, tipo_actual);
      break;}   
    case TOKEN_FUNC:{
        Parser_consumir(par, TOKEN_FUNC);
        char nombre[50];
        strcpy(nombre, par->token_actual.valor.frase);
        Parser_consumir(par, TOKEN_IDEN);
        Parser_consumir(par, TOKEN_LPAR);
        unsigned int i = 0;
        char param[10][50];
        while(par->token_actual.tipo != TOKEN_RPAR){
          if(par->token_actual.tipo == TOKEN_COMA){
            Parser_consumir(par, TOKEN_COMA);
          }
          else if(par->token_actual.tipo == TOKEN_IDEN){
            strcpy(param[i], par->token_actual.valor.frase);
            Parser_consumir(par, TOKEN_IDEN);
            i++;
          }
          else{
            printf("Parametros no validos");
            exit(1);
          }
        }
        char param_salida[50];
        Parser_consumir(par, TOKEN_RPAR);
        Parser_consumir(par, TOKEN_LPAR);
        if(par->token_actual.tipo == TOKEN_IDEN){
          strcpy(param_salida, par->token_actual.valor.frase);
          Parser_consumir(par, TOKEN_IDEN);
        } 
        Parser_consumir(par, TOKEN_RPAR);
        Nodo* bloque = Parser_bloque(par);
        nodo = Crear_nodo_func_def(nombre, param, param_salida, i, bloque);
        break;}
    case TOKEN_FINISH:{
        Parser_consumir(par, TOKEN_FINISH);
        Parser_consumir(par, TOKEN_PUNCOMA);
        nodo = Crear_nodo_finish();
        break;}
    case TOKEN_IDEN:{
        nodo = Parser_factor(par);
        Parser_consumir(par, TOKEN_PUNCOMA);
        break;}
    default:
      printf("Error de sintaxis: token inesperado o instrucción no válida\n");
      Print_token(par->token_actual.tipo);
      exit(1);
  }
  return nodo;
}
