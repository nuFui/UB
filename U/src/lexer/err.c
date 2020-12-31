#include "../../include/lexer/err.h"

void err_print(err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  fprintf(stderr, "%s[err]: file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          err->start_pos.file,
          err->start_pos.line,
          err->start_pos.column,
          (char*)err->name);
  vfprintf(stderr, err->details, args);
  va_end(args);
  fprintf(stderr, ".%s\n", RESET);
}

void err_vprint(err_base_t *err, va_list details)
{
  fprintf(stderr, "%s[err]:  file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          err->start_pos.file,
          err->start_pos.line,
          err->start_pos.column,
          (char*)err->name);
  vfprintf(stderr, err->details, details);
  fprintf(stderr, ".%s\n", RESET);
}

void err_raise(err_base_t *err, ...)
{
  va_list args;
  va_start(args, err);
  err_vprint(err, args);
  va_end(args);
  exit(EXIT_FAILURE);
}