#include "../../include/parser/parser_err.h"

void parser_err_print(parser_err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  vfprintf(stderr, "%s[err]:  file: %s, line %d, column: %d\n\terror: %s, message: ",
           RED,
           err->t->file,
           err->t->line,
           err->t->column,
           (char*)err->name);
  vfprintf(stderr, err->details, args);
  va_end(args);
  fprintf(stderr, ".%s\n", RESET);
}

void parser_err_vprint(parser_err_base_t *err, va_list details)
{
  fprintf(stderr, "%s[err]:  file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          err->t->file,
          err->t->line,
          err->t->column,
          (char*)err->name);
  vfprintf(stderr, err->details, details);
  fprintf(stderr, ".%s\n", RESET);
}

void parser_err_raise(parser_err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  parser_err_vprint(err, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

void parser_err_copy(parser_err_base_t *dest, parser_err_base_t *src, int delete_src)
{
  dest->t = malloc(sizeof(tok_t));
  tok_copy(dest->t, src->t, 1);
  dest->details = strdup(src->details);
  if (delete_src)
  {
    free((char*)src->name);
    free(src->detials);
    src->name = NULL;
    src->detials = NULL;
  }
}