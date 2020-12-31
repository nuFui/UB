#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "../lexer/lex.h"
#include "../lexer/err.h"

#include "node.h"

typedef struct ParseResult
{
  error_base_t *err;
  generic_node_t *node;
} parse_result_t;

extern parse_result_t *parse_result_register(parse_result_t *self, parse_result_t *res);
extern parse_result_t *parse_result_success(parse_result_t *self, node_generic_t *node);
extern parse_result_t *parse_result_failure(parse_result_t *self, error_base_t *err);

typedef struct Parser
{
  int tok_index;
  tok_list_t *tokens;
  tok_t *current_tok;
} parser_t;

extern parser_t parser_create(tok_list_t *tokens);
extern void parser_destroy(parser_t *par);

extern void parser_advance(parser_t *par);

extern parse_result_t parser_parse(parser_t *par);
extern parse_result_t parser_factor(parser_t *par);

extern parse_result_t parser_term(parser_t *par);
extern parse_result_t parser_expr(parser_t *par);

extern parse_result_t parser_binop(parser_t *par, parse_result_t (*func)(parser_t *par), uint8_t types[], uint8_t types_count);

#endif