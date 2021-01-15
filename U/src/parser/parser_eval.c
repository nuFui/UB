#include "../../include/parser/parser_eval.h"

// Evaluates binary operation given left, right operand and the node->op->type.
static struct EvalResult node_binary_eval(node_binary_t *node, struct EvalResult left, struct EvalResult right, int was_minus)
{
  struct EvalResult res;
  res.result = -1;
  res.code = EVAL_SUCCESS;
  switch (node->op->type)
  {
  case TOK_TYPE_ADD:
    res.result = left.result + right.result;
    break;
  case TOK_TYPE_SUB:
    res.result = left.result - (was_minus ? -right.result : right.result);
    break;
  case TOK_TYPE_MUL:
    res.result = left.result * right.result;
    break;
  case TOK_TYPE_DIV:
    if (right.result == 0)
    {
      res.code = EVAL_FAILURE;
      return res;
    }
    res.result = left.result / right.result;
    break;
  case TOK_TYPE_POW:
    res.result = pow(left.result, right.result);
    break;
  }
  return res;
}

// Evaluates binary tree from leaves up. 
struct EvalResult node_binary_tree_eval(node_binary_t *mov, int was_minus)
{
  if (mov->op->type == TOK_TYPE_INT || mov->op->type == TOK_TYPE_FLT)
  {
    struct EvalResult ret;
    ret.code = EVAL_SUCCESS;
    ret.result = atof(mov->op->value);
    printf("was_minus = %d, result = %f\n", was_minus, ret.result);
    return ret;
  }
  struct EvalResult l = node_binary_tree_eval(mov->left, was_minus);
  struct EvalResult r = node_binary_tree_eval(mov->right, was_minus);
  was_minus = mov->op->type == TOK_TYPE_SUB && *root != mov;
  struct EvalResult s = node_binary_eval(mov, l, r, was_minus);
  if (s.code == EVAL_FAILURE)
  {
    node_binary_tree_delete(*root);
    exit(EXIT_FAILURE);
  }
  return s;
}