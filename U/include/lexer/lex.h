#ifndef __LEXER_LEX_H__
#define __LEXER_LEX_H__

#include <ctype.h>

#include "lex_pos.h"
#include "tok.h"

#define APPEND_EOF 1

typedef struct
{
  char *cur;       // Cursor or current character in analysis.
  lexer_pos_t pos; // Information about position when performing lexical analysis.
  uint32_t size;   // Size of text in bytes.
  char *text;      // Source buffer.
} lexer_t;

extern lexer_t lex_create(const char *path);
extern lexer_t lex_create_from_string(const char *str);
extern void lex_destroy(lexer_t *lex);
extern void lex_advance(lexer_t *lex);

extern tok_list_t lex_make_toks(lexer_t *lex);

#endif
