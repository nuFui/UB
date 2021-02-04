#include "../../include/lexer/lex_pos.h"

// Keeps record of position member variables.
void lex_pos_advance(lexer_pos_t *lex_pos, char *cur) {
  ++lex_pos->index;
  ++lex_pos->column;
  if (*cur == '\n') {
    ++lex_pos->line;
    lex_pos->column = 0;
  }
}

// Duplicates src into dest.
void lex_pos_copy(lexer_pos_t *dest, lexer_pos_t *src) {
  *dest = *src;
  dest->file = strdup(src->file);
}