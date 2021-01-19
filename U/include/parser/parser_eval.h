#ifndef __PARSER_PARSER_EVAL_H__
#define __PARSER_PARSER_EVAL_H__

#include "parser_node.h"

typedef enum
{
  EVAL_SUCCESS,
  EVAL_FAILURE,
  EVAL_DIVZERO,
} eval_result_code_t;

typedef struct
{
  eval_result_code_t code;
  float result;
} eval_result_t;

extern eval_result_t node_binary_tree_eval(node_binary_t *mov);

#endif