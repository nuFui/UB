#ifndef __LEXER_LEX_ERROR_H__
#define __LEXER_LEX_ERROR_H__

#include <stdarg.h>
#include <stdbool.h>

#include "lex_pos.h"

#include "../common/constants.h"

// Implicit decls required by this translation unit.
typedef struct Lexer lexer_t;
void lex_destroy(lexer_t *lex);

typedef struct
{
  lexer_pos_t pos;
  const char *name;
  char *details;
} lex_err_base_t;

extern void lex_err_print(lex_err_base_t *err, ...);
extern void lex_err_vprint(lex_err_base_t *err, va_list details);
extern void lex_err_raise(lex_err_base_t *err, lexer_t *lex, ...);
extern void lex_err_copy(lex_err_base_t *dest, lex_err_base_t *src, bool delete_src);

typedef struct
{
  lex_err_base_t base;
} lex_err_illegal_char_t;

#endif