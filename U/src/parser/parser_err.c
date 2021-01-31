#include "../../include/parser/parser_err.h"

void parser_err_print(parser_err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  fprintf(stderr, "%s[err]:  file: %s, line %d, column: %d\n\terror: %s, message: ",
          RED,
          err->t->file,
          err->t->line,
          err->t->column,
          (char *)err->name);
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
          (char *)err->name);
  vfprintf(stderr, err->details, details);
  fprintf(stderr, ".%s\n", RESET);
}

void parser_err_raise(parser_err_base_t *err, parser_t *par, ...)
{
  va_list args;
  va_start(args, par);
  parser_err_vprint(err, args);
  va_end(args);
  parser_destroy(par);
  exit(EXIT_FAILURE);
}

void parser_err_copy(parser_err_base_t *dest, parser_err_base_t *src, bool delete_src)
{
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  dest->t = ualloc(&pos, sizeof(tok_t));
  tok_copy(dest->t, src->t, true);
  dest->details = strdup(src->details);
  if (delete_src)
  {
    ufree((char *)src->name);
    ufree(src->details);
  }
}