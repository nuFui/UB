#ifndef __LEXER_TOK_H__
#define __LEXER_TOK_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../common/error.h"

enum
{
  TOK_TYPE_INT,
  TOK_TYPE_FLT,
  TOK_TYPE_SUB,
  TOK_TYPE_ADD,
  TOK_TYPE_DIV,
  TOK_TYPE_MUL,
  TOK_TYPE_POW,
  TOK_TYPE_LPAR,
  TOK_TYPE_RPAR,
  TOK_TYPE_EOF
};

typedef struct Tok
{
  uint8_t type;
  uint32_t line;
  uint32_t column;
  const char *file;
  char *value;
} tok_t;

typedef struct TokList
{
  uint32_t count;
  tok_t **toks;
} tok_list_t;

extern tok_t tok_create(uint8_t type, const char *value, const char *file);
extern void tok_delete(tok_t *tok);
extern void tok_print(tok_t *tok, int newline, int verbose);
extern void tok_copy(tok_t *dest, tok_t *src, int delete_src);

extern void tok_list_print(tok_list_t *list, int verbose);
extern void tok_list_copy(tok_list_t *dest, tok_list_t *src, int delete_src);
extern void tok_list_delete(tok_list_t *list);

#endif