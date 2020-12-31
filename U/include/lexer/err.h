#ifndef __LEXER_ERROR_H__
#define __LEXER_ERROR_H__

#include <stdarg.h>

#include "lex.h"

#include "../common/constants.h"

typedef struct ErrBase
{
  lexer_pos_t start_pos;
  lexer_pos_t end_pos;
  const char *name;
  char *details;
} err_base_t;

extern void err_print(err_base_t *err, ...);
extern void err_vprint(err_base_t *err, va_list details);
extern void err_raise(err_base_t *err, ...);

typedef struct ErrIllegalChar
{
  err_base_t base;
} err_illegal_char_t;

#endif