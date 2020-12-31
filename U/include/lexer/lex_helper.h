#ifndef __LEXER_LEX_HELPER_H__
#define __LEXER_LEX_HELPER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../common/error.h"

#define TEST_MEMFAIL 0
#define TEST_ERRFATAL 0

extern uint8_t lex_helper_read_file(const char *path, char **buffer, uint32_t *size);

#endif