#ifndef __LEXER_LEX_POS_H__
#define __LEXER_LEX_POS_H__

#include "lex_helper.h"

typedef struct
{
  uint32_t index;   // Index of current with respect to text.
  uint32_t line;    // Line number
  uint32_t column;  // Column number
  const char *file; // File name
} lexer_pos_t;

extern void lex_pos_advance(lexer_pos_t *lex_pos, char *cur);
extern void lex_pos_copy(lexer_pos_t *dest, lexer_pos_t *src);

#endif