#ifndef __LEXER_LEX_H__
#define __LEXER_LEX_H__

#include <ctype.h>

#include "lex_err.h"
#include "tok.h"

struct Lexer {
  char *cur;        // Cursor or current character in analysis.
  lexer_pos_t pos;  // Information about position when performing lexical analysis.
  int size;         // Size of text in bytes.
  char *text;       // Source buffer.
};

extern lexer_t lex_create(const char *path);
extern lexer_t lex_create_from_string(const char *str);
extern void lex_destroy(lexer_t *lex);
extern void lex_advance(lexer_t *lex);

extern tok_list_t *lex_make_toks(lexer_t *lex);

#endif
