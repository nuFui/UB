#ifndef __LEXER_TOK_H__
#define __LEXER_TOK_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../common/error.h"
#include "../common/ualloc.h"

typedef enum
{
  TOK_TYPE_ADD,
  TOK_TYPE_SUB,
  TOK_TYPE_DIV,
  TOK_TYPE_MUL,
  TOK_TYPE_POW,
  TOK_TYPE_LPAR,
  TOK_TYPE_RPAR,
  TOK_TYPE_INT,
  TOK_TYPE_FLT,
  TOK_TYPE_STR,
  TOK_TYPE_EOF,
  TOK_TYPE_DUMMY_MAX
} tok_type_t;

typedef struct
{
  tok_type_t type;
  int line;
  int column;
  const char *file;
  char *value;
} tok_t;

typedef struct
{
  int count;
  tok_t *toks[];
} tok_list_t;

extern tok_t tok_create(tok_type_t type, const char *value, const char *file);
extern void tok_delete(tok_t *tok);
extern void tok_print(tok_t *tok, bool newline, bool verbose, bool stringify);
extern void tok_copy(tok_t *dest, tok_t *src, bool delete_src);

extern void tok_list_print(tok_list_t *list, bool verbose, bool stringify);
extern void tok_list_copy(tok_list_t *dest, tok_list_t *src, bool delete_src);
extern void tok_list_delete(tok_list_t *list);

#endif