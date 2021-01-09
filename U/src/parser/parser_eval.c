#include "../../include/parser/parser_eval.h"

static struct EvalResult node_binary_eval(node_binary_t *node, struct EvalResult left, struct EvalResult right)
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
    res.result = left.result - right.result;
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

struct EvalResult node_binary_tree_eval(node_binary_t *mov)
{
  if (mov->op->type == TOK_TYPE_INT || mov->op->type == TOK_TYPE_FLT)
  {
    struct EvalResult ret;
    ret.code = EVAL_SUCCESS;
    ret.result = atof(mov->op->value);
    return ret;
  }
  struct EvalResult l = node_binary_tree_eval(mov->left);
  struct EvalResult r = node_binary_tree_eval(mov->right);
  struct EvalResult s = node_binary_eval(mov, l, r);
  if (s.code == EVAL_FAILURE)
  {
    node_binary_tree_delete(mov);
    exit(EXIT_FAILURE);
  }
  return s;
}