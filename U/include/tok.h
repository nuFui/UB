#ifndef __TOK_H__
#define __TOK_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

enum TokType {
  TOK_TYPE_INT,
  TOK_TYPE_FLT,
  TOK_TYPE_ADD,
  TOK_TYPE_SUB,
  TOK_TYPE_MUL,
  TOK_TYPE_DIV,
  TOK_TYPE_LPAR,
  TOK_TYPE_RPAR
};

typedef struct Tok {
  uint8_t type;
  char *value;
} tok_t;

typedef struct TokList {
  uint16_t size;
  tok_t **toks;
} tok_list_t;

extern tok_t tok_create(uint8_t type, const char *value);
extern void tok_delete(tok_t *tok);
extern void tok_print(tok_t *tok);

extern void tok_list_print(tok_list_t *list);
extern void tok_list_delete(tok_list_t *list);

#endif