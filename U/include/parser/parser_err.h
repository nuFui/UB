#ifndef __PARSER_ERR_H__
#define __PARSER_ERR_H__

#include <stdarg.h>

#include "../common/constants.h"
#include "../lexer/tok.h"

/* Implicit decls required by this translation unit. */
typedef struct Parser parser_t;
void parser_destroy(parser_t *par);
typedef struct ParserRegister parser_register_t;
void parser_register_destroy(parser_register_t *reg);
/* Endof implicit decls */

typedef struct
{
  tok_t *t;
  const char *name;  // name of error
  char *details;     // Details of error
} parser_err_base_t;

extern void parser_err_print(parser_err_base_t *err, ...);
extern void parser_err_vprint(parser_err_base_t *err, va_list details);
extern void parser_err_raise(parser_err_base_t *err, parser_t *par, ...);
extern void parser_err_handle(int count, ...);
extern void parser_register_err_raise(parser_err_base_t *err, parser_register_t *reg, ...);
extern void parser_err_copy(parser_err_base_t *dest, parser_err_base_t *src, bool delete_src);

typedef struct
{
  parser_err_base_t base;
} parser_err_invalid_syntax;

#endif