#include "../include/error.h"

void error_print(error_base_t *error, ...)
{
  va_list args;
  va_start(args, error);
  fprintf(stderr, "%s[err]: file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          error->start_pos.file,
          error->start_pos.line,
          error->start_pos.column,
          (char*)error->name);
  vfprintf(stderr, error->details, args);
  va_end(args);
  fprintf(stderr, ".%s\n", RESET);
}

void error_vprint(error_base_t *error, va_list details)
{
  fprintf(stderr, "%s[err]:  file: %s, line: %d, column: %d\n\terror: %s, message: ",
          RED,
          error->start_pos.file,
          error->start_pos.line,
          error->start_pos.column,
          (char*)error->name);
  vfprintf(stderr, error->details, details);
  fprintf(stderr, ".%s\n", RESET);
}

void error_raise(error_base_t *error, ...)
{
  va_list args;
  va_start(args, error);
  error_vprint(error, args);
  va_end(args);
  exit(EXIT_FAILURE);
}