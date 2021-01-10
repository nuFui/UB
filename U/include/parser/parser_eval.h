#ifndef __PARSER_PARSER_EVAL_H__
#define __PARSER_PARSER_EVAL_H__

#include "parser_node.h"

enum EvalResultCode
{
  EVAL_SUCCESS,
  EVAL_FAILURE
};

struct EvalResult
{
  enum EvalResultCode code;
  float result;
};

extern struct EvalResult node_binary_tree_eval(node_binary_t *prev, node_binary_t *mov);

#endif