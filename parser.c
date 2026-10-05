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
      nodo = Crear_nodo_iden(nombre);
      break;}
    case TOKEN_LPAR:{
      Parser_consumir(par, TOKEN_LPAR);
      nodo = Parser_comparacion(par);
      Parser_consumir(par, TOKEN_RPAR);
      break;}
  }
  return nodo;
}

Nodo* Parser_termino(Parser* par){
  Nodo* izq = Parser_factor(par);
  while(par->token_actual.tipo == TOKEN_MUL || par->token_actual.tipo == TOKEN_DIV){
    Tipo_binario op = (par->token_actual.tipo == TOKEN_MUL ? OP_MUL : OP_DIV);
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

Nodo* Parser_instruccion(Parser* par){
  Nodo* nodo; 
  switch ((int) par->token_actual.tipo) {
    case TOKEN_VAR:{
      Parser_consumir(par, TOKEN_VAR);
      char nombre[50];
      strcpy(nombre, par->token_actual.valor.frase);
      Parser_consumir(par, TOKEN_IDEN);
      Parser_consumir(par, TOKEN_ASIGNAR);
      Nodo* exp = Parser_comparacion(par);
      Parser_consumir(par, TOKEN_PUNCOMA);
      nodo = Crear_nodo_asig(nombre, exp);
      break;}
    case TOKEN_PRINT:{
      Parser_consumir(par, TOKEN_PRINT);
      Nodo* exp = Parser_comparacion(par);
      Parser_consumir(par, TOKEN_PUNCOMA);
      nodo = Crear_nodo_print(exp);
      break;} 
    case TOKEN_IF:
    case TOKEN_WHILE:{
      int tipo_actual = (par->token_actual.tipo == TOKEN_IF ? 0 : 1);
      Parser_consumir(par, par->token_actual.tipo == TOKEN_IF ? TOKEN_IF : TOKEN_WHILE);
      Nodo* condicion = Parser_comparacion(par);
      Nodo* bloque = Parser_bloque(par);
      nodo = Crear_nodo_if_while(condicion, bloque, tipo_actual);
      break;}
  }
  return nodo;
}
