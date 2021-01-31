#ifndef __LEXER_LEX_HELPER_H__
#define __LEXER_LEX_HELPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/error.h"
#include "../common/ualloc.h"

#define TEST_ERRFATAL 0

extern int lex_helper_read_file(const char *path, char **buffer, int *size);

#endif