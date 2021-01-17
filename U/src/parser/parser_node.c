#include "../../include/parser/parser_node.h"

node_binary_t **root = NULL;
static uint8_t scope = 0;

void node_binary_tree_root_init()
{
  root = malloc(sizeof(node_binary_t *));
}

/*
// Finds next operator in types[] in the current scope.
static int32_t find_next_op(parser_t *par, uint32_t from, uint32_t to, uint8_t types[], uint8_t types_count)
{
  uint8_t scp = scope;
  uint8_t smallest = TOK_TYPE_DUMMY_MAX;
  int32_t smallest_index = -1;

  while (to > from)
  {
    switch (par->tok_list->toks[to]->type)
    {
    case TOK_TYPE_LPAR:
      --scp;
      break;
    case TOK_TYPE_RPAR:
      ++scp;
      break;
    case TOK_TYPE_INT:
    case TOK_TYPE_FLT:
    case TOK_TYPE_EOF:
    case TOK_TYPE_DUMMY_MAX:
      break;
    default:
      goto cmp;
    }
    --to;
    continue;
  cmp:
    if (scp == scope)
    {
      for (int j = 0; j < types_count; ++j)
      {
        if (par->tok_list->toks[to]->type == types[j])
        {
          goto small;
        }
      }
      --to;
      continue;
    small:
      if (par->tok_list->toks[to]->type < smallest)
      {
        smallest = par->tok_list->toks[to]->type;
        smallest_index = to;
      }
    }
    --to;
  }
  return smallest_index;
}
*/

static int32_t find_next_op(parser_t *par, uint32_t from, uint32_t to, uint8_t types[], uint8_t types_count)
{
  uint8_t scp = scope;
  uint8_t smallest = TOK_TYPE_DUMMY_MAX;
  int32_t smallest_index = -1;
  while (from < to)
  {
    switch (par->tok_list->toks[from]->type)
    {
    case TOK_TYPE_LPAR:
      ++scp;
      break;
    case TOK_TYPE_RPAR:
      --scp;
      break;
    case TOK_TYPE_INT:
    case TOK_TYPE_FLT:
    case TOK_TYPE_EOF:
    case TOK_TYPE_DUMMY_MAX:
      break;
    default:
      goto cmp;
    }
    ++from;
    continue;
  cmp:
    if (scp == scope)
    {
      for (int j = 0; j < types_count; ++j)
      {
        if (par->tok_list->toks[from]->type == types[j])
        {
          goto small;
        }
      }
      ++from;
      continue;
    small:
      if (par->tok_list->toks[from]->type <= smallest && smallest == TOK_TYPE_SUB)
      {
        smallest = par->tok_list->toks[from]->type;
        smallest_index = from;
      }
      else if (par->tok_list->toks[from]->type < smallest)
      {
        smallest = par->tok_list->toks[from]->type;
        smallest_index = from;
      }
    }
    ++from;
  }
  return smallest_index;
}

// PEMDSA
static uint8_t ops[5] = {
    TOK_TYPE_ADD,
    TOK_TYPE_SUB,
    TOK_TYPE_DIV,
    TOK_TYPE_MUL,
    TOK_TYPE_POW};

// Constructs binary tree of operations.
// How expression 2 * 4 looks like:
//                 BINOP
//              /   |   \
//        BINOP     *    BINOP
//      /   |  \       /   |   \
//  NULL   2   NULL  NULL  4  NULL

/*
void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov)
{
  mov->op = NULL;
  mov->left = NULL;
  mov->right = NULL;

  if (to == from)
  {
    if (par->tok_list->toks[to]->type == TOK_TYPE_INT || par->tok_list->toks[to]->type == TOK_TYPE_FLT)
    {
      mov->op = par->tok_list->toks[to];
      return;
    }
  }

  int32_t i = find_next_op(par, from, to, ops, 5);
  if (i != -1)
  {
    mov->op = par->tok_list->toks[i];
    mov->left = malloc(sizeof(node_binary_t));
    mov->right = malloc(sizeof(node_binary_t));
    node_binary_tree(from, i - 1, par, mov->left);
    node_binary_tree(i + 1, to, par, mov->right);
    return;
  }

  if (par->tok_list->toks[to]->type == TOK_TYPE_RPAR && par->tok_list->toks[from]->type == TOK_TYPE_LPAR)
  {
    ++scope;
    node_binary_tree(from + 1, to - 1, par, mov);
    --scope;
    return;
  }
}
*/

void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov)
{
  mov->op = NULL;
  mov->left = NULL;
  mov->right = NULL;

  if (to - from == 1)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_INT || par->tok_list->toks[from]->type == TOK_TYPE_FLT)
    {
      mov->op = par->tok_list->toks[from];
      return;
    }
  }

  int32_t i = find_next_op(par, from, to, ops, 5);
  if (i != -1)
  {
    mov->op = par->tok_list->toks[i];
    mov->left = malloc(sizeof(node_binary_t));
    mov->right = malloc(sizeof(node_binary_t));
    node_binary_tree(from, i, par, mov->left);
    node_binary_tree(i + 1, to, par, mov->right);
    return;
  }

  if (par->tok_list->toks[from]->type == TOK_TYPE_LPAR && par->tok_list->toks[to - 1]->type == TOK_TYPE_RPAR)
  {
    ++scope;
    node_binary_tree(from + 1, to - 1, par, mov);
    --scope;
    return;
  }
}

void node_binary_tree_delete(node_binary_t *mov)
{
  if (!mov)
    return;
  free(mov->op);
  mov->op = NULL;
  node_binary_tree_delete(mov->left);
  node_binary_tree_delete(mov->right);
  free(mov->left);
  free(mov->right);
}

void node_binary_tree_print(node_binary_t *mov)
{
  if (!mov)
    return;
  tok_print(mov->op, 1, 1);
  node_binary_tree_print(mov->left);
  node_binary_tree_print(mov->right);
}