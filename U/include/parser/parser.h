#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "../lexer/tok.h"
#include "parser_err.h"

struct Parser {
  int tok_index;
  tok_t *tok_cur;
  tok_list_t *tok_list;
};

extern parser_t parser_create(tok_list_t *list);
extern void parser_destroy(parser_t *par);
extern void parser_advance(parser_t *par);

#endif