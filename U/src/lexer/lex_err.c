#include "../../include/lexer/lex_err.h"

void lex_err_print(lex_err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  fprintf(stderr, "%s[err]: file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          err->pos.file,
          err->pos.line,
          err->pos.column,
          (char*)err->name);
  vfprintf(stderr, err->details, args);
  va_end(args);
  fprintf(stderr, ".%s\n", RESET);
}

void lex_err_vprint(lex_err_base_t *err, va_list details)
{
  fprintf(stderr, "%s[err]:  file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          err->pos.file,
          err->pos.line,
          err->pos.column,
          (char*)err->name);
  vfprintf(stderr, err->details, details);
  fprintf(stderr, ".%s\n", RESET);
}

void lex_err_raise(lex_err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  lex_err_vprint(err, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

// NOTE: Does not delete src pointer if delete_src = 1.
void lex_err_copy(lex_err_base_t *dest, lex_err_base_t *src, int delete_src)
{
  lex_pos_copy(&dest->pos, &src->pos);
  dest->details = strdup(src->details);
  if (delete_src)
  {
    free((char*)src->name);
    free(src->details);
    src->name = NULL;
    src->details = NULL;
  }
}