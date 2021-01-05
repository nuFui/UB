#include "../../include/parser/pnode.h"

node_binary_t **root = NULL;
static uint8_t scope = 0;

void node_binary_tree_root_init()
{
  root = malloc(sizeof(node_binary_t *));
}

static uint32_t find_add_or_sub(uint32_t from, uint32_t to, parser_t *par)
{
  for (from; from < to; ++from)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_ADD || par->tok_list->toks[from]->type == TOK_TYPE_SUB)
    {
      return from;
    }
  }
  return to + 1;
}

static uint32_t find_mul_or_div(uint32_t from, uint32_t to, parser_t *par)
{
  for (from; from < to; ++from)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_MUL || par->tok_list->toks[from]->type == TOK_TYPE_DIV)
    {
      return from;
    }
  }
  return to + 1;
}

void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov)
{
  if (from == to)
    return;

  if (par->tok_list->toks[from]->type == TOK_TYPE_SUB)
  {
    mov->op = par->tok_list->toks[from];
    mov->left = NULL;
    node_binary_tree(from + 1, to, par, mov->right);
  }

  uint32_t add_sub = find_add_or_sub(from, to, par);
  if (add_sub != to + 1)
  {
    mov->op = par->tok_list->toks[add_sub];
    node_binary_tree(0, add_sub, par, mov->left);
    node_binary_tree(add_sub + 1, par->tok_list->count, par, mov->right);
  }

  uint32_t mul_div = find_mul_or_div(from, to, par);
  if (mul_div != to + 1)
  {
    mov->op = par->tok_list->toks[mul_sub];
    node_binary_tree(0, mul_div, par, mov->left);
    node_binary_tree(mul_div + 1, par->tok_list->count, par, mov->right);
  }




  // mov = malloc(sizeof(node_binary_t));
  if (par->tok_list->toks[from]->type == TOK_TYPE_SUB)
  {
    mov->op = par->tok_list->toks[from];
    node_binary_tree(from + 1, par->tok_list->count, par, mov->right);
  }
  else if (par->tok_list->toks[from]->type == TOK_TYPE_ADD || par->tok_list->toks[from]->type == TOK_TYPE_SUB)
  {
    mov->op = par->tok_list->toks[from];
    node_binary_tree(0, from, par, mov->left);
    node_binary_tree(from + 1, par->tok_list->count, par, mov->right);
  }
  else if (par->tok_list->toks[from]->type == TOK_TYPE_MUL || par->tok_list->toks[from]->type == TOK_TYPE_DIV)
  {
    mov->op = par->tok_list->toks[from];
    node_binary_tree(0, from, par, mov->left);
    node_binary_tree(from + 1, par->tok_list->count, par, mov->right);
  }
  else if (par->tok_list->toks[from]->type == TOK_TYPE_INT || par->tok_list->toks[from]->type == TOK_TYPE_FLT)
  {
    // Put number as (op) so no further allocation is needed.
    mov->op = par->tok_list->toks[from];
  }
  node_binary_tree(from + 1, to, par, mov);
}

void node_binary_tree_delete(node_binary_t *mov)
{
  if (!mov)
    return;
  node_binary_tree_delete(mov);
  free(mov->left);
  free(mov->right);
}