#ifndef __LEXER_LEX_H__
#define __LEXER_LEX_H__

#include <ctype.h>

#include "lex_helper.h"
#include "tok.h"

typedef struct LexPosition
{
  uint32_t index;   // Index of current with respect to text.
  uint32_t line;    // Line number
  uint32_t column;  // Column number
  const char *file; // File name
} lexer_pos_t;

extern void lex_pos_advance(lexer_pos_t *lex_pos, char *cur);
extern lexer_pos_t lex_pos_copy(lexer_pos_t *lex_pos);

typedef struct Lex
{
  char *cur;       // Cursor or current character in analysis.
  lexer_pos_t pos; // Information about position when performing lexical analysis.
  uint32_t size;   // Size of text in bytes.
  char *text;      // Source buffer.
} lexer_t;

extern lexer_t lex_create(const char *path);
extern void lex_destroy(lexer_t *lex);
extern void lex_advance(lexer_t *lex);

extern void lex_make_toks(lexer_t *lex, tok_list_t *list);

#endif
