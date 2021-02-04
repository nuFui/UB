#ifndef __PARSER_PARSER_EVAL_H__
#define __PARSER_PARSER_EVAL_H__

#include <limits.h>
#include <math.h>

#include "parser_node.h"
#include "parser_reg.h"

typedef enum {
  EVAL_SUCCESS,
  EVAL_FAILURE,
  EVAL_DIVZERO,
} eval_result_code_t;

typedef struct
{
  tok_type_t kind;          // Current type.
  eval_result_code_t code;  // Success?
  char *result;             // Interpretation of result. actually int or float but sometimes merely str.
} eval_result_t;

extern eval_result_t node_binary_tree_eval(parser_register_t *par, node_binary_t *mov);

#endif