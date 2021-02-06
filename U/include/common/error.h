#ifndef __COMMON_ERROR_H__
#define __COMMON_ERROR_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

typedef struct {
  const char *file;
  const char *function;
  int line;
} error_pos_t;

#define ERROR_BODY(file, function, line)                                      \
  fprintf(stderr, "%s[%s]: file: %s, function: %s, line: %d, message: ", RED, \
          __FUNCTION__, file, function, line);                                \
  vfprintf(stderr, fmt, details);                                             \
  fprintf(stderr, ".%s\n", RESET);                                            \
  exit(EXIT_FAILURE);

extern void error_raise(void (*error_func)(error_pos_t *, const char *,
                                           va_list),
                        error_pos_t *pos, const char *fmt, ...);

extern void error_memory(error_pos_t *pos, const char *fmt, va_list details);
extern void error_fatal(error_pos_t *pos, const char *fmt, va_list details);

#define ERROR_POSITION \
  (error_pos_t) { __FILE__, __func__, __LINE__ }

#endif