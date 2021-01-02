#ifndef __LEXER_LEX_ERROR_H__
#define __LEXER_LEX_ERROR_H__

#include <stdarg.h>

#include "lex_pos.h"

#include "../common/constants.h"

typedef struct LexErrBase
{
  lexer_pos_t pos;
  const char *name;
  char *details;
} lex_err_base_t;

extern void lex_err_print(lex_err_base_t *err, ...);
extern void lex_err_vprint(lex_err_base_t *err, va_list details);
extern void lex_err_raise(lex_err_base_t *err, ...);
extern void lex_err_copy(lex_err_base_t *dest, err_base_t *src, int delete_src);

typedef struct LexErrIllegalChar
{
  lex_err_base_t base;
} lex_err_illegal_char_t;

#endif