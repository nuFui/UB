#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdarg.h>

#include "lex.h"

#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef struct ErrorBase
{
  lexer_pos_t start_pos;
  lexer_pos_t end_pos;
  const char *name;
  char *details;
} error_base_t;

extern void error_print(error_base_t *error, ...);
extern void error_vprint(error_base_t *error, va_list details);
extern void error_raise(error_base_t *error, ...);

typedef struct ErrorIllegalChar
{
  error_base_t base;
} error_illegal_char_t;

#endif