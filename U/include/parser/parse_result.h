#ifndef __PARSER_PARSE_RESULT_H__
#define __PARSER_PARSE_RESULT_H__

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

#endif