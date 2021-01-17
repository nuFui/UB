#include "../../include/common/error.h"

// Raiser function. Exits with code 1.
void error_raise(void (*error_func)(error_pos_t *, const char *, va_list), error_pos_t *pos, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  (*error_func)(pos, fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

// Memory errors.
void error_memory(error_pos_t *pos, const char *fmt, va_list details) { ERROR_BODY(pos->file, pos->function, pos->line) }

// Critical errors.
void error_fatal(error_pos_t *pos, const char *fmt, va_list details) { ERROR_BODY(pos->file, pos->function, pos->line) }