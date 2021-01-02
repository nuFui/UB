#ifndef __PARSER_ERR_H__
#define __PARSER_ERR_H__

#include <stdarg.h>

#include "../common/constants.h"

typedef struct ParserErrBase
{
  tok_t *t;
  const char *name;
  char *details;
} parser_err_base_t;

extern void parser_err_print(parser_err_base_t *err, ...);
extern void parser_err_vprint(parser_err_base_t *err, va_list details);
extern void parser_err_raise(parser_err_base_t *err, ...);
extern void parser_err_copy(parser_err_base_t *dest, parser_err_base_t *src, int delete_src);

typedef struct ParserErrInvalidSyntax
{
  parser_err_base_t base;
} parser_err_invalid_syntax;

#endif