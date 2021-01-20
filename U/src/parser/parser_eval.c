#include "../../include/parser/parser_eval.h"

// Evaluates binary operation given left, right operand and the node->op->type.
static eval_result_t node_binary_eval(node_binary_t *node, eval_result_t left, eval_result_t right)
{
  eval_result_t res = {
      EVAL_SUCCESS,
      -1};
  switch (node->op->type)
  {
  case TOK_TYPE_ADD:
    res.result = left.result + right.result;
    break;
  case TOK_TYPE_SUB:
    res.result = left.result - right.result;
    break;
  case TOK_TYPE_MUL:
    res.result = left.result * right.result;
    break;
  case TOK_TYPE_DIV:
    if (right.result == 0)
    {
      res.code = EVAL_DIVZERO;
      return res;
    }
    res.result = left.result / right.result;
    break;
  case TOK_TYPE_POW:
    res.result = pow(left.result, right.result);
  }
  return res;
}

// Evaluates binary tree from leaves up.
eval_result_t node_binary_tree_eval(node_binary_t *mov)
{
  if (mov->op->type == TOK_TYPE_INT || mov->op->type == TOK_TYPE_FLT)
  {
    eval_result_t ret = {
        EVAL_SUCCESS,
        atof(mov->op->value)};
    return ret;
  }
  eval_result_t s = node_binary_eval(mov, node_binary_tree_eval(mov->left), node_binary_tree_eval(mov->right));
  if (s.code != EVAL_SUCCESS)
  {
    node_binary_tree_delete(*root);
    exit(EXIT_FAILURE);
  }
  return s;
}