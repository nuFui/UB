#include "../../include/parser/pnode.h"

node_binary_t **root = NULL;
static uint8_t scope = 0;
static uint32_t last = 0;

void node_binary_tree_root_init()
{
  root = malloc(sizeof(node_binary_t *));
}

static uint32_t find_next(parser_t *par, uint8_t types[], uint8_t types_count)
{
  for (int i = last; i < par->tok_list->count; ++i)
  {
    for (int ii = 0; ii < types_count; ++ii)
    {
      if (par->tok_list->toks[i]->type == types[ii])
      {
        last = i + 1;
        return i;
      }
    }
  }
  return par->tok_list->count + 1;
}

static uint8_t taddsub[2] = {TOK_TYPE_ADD, TOK_TYPE_SUB};
static uint8_t tmuldiv[2] = {TOK_TYPE_MUL, TOK_TYPE_DIV};
static uint8_t tintflt[2] = {TOK_TYPE_INT, TOK_TYPE_FLT};

void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov)
{
  mov->op = NULL;
  mov->left = NULL;
  mov->right = NULL;
  if (to == from)
  {
    // Unary operation
    free(mov);
    mov = NULL;
    return;
  }
  if (to - from == 1)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_INT || par->tok_list->toks[from]->type == TOK_TYPE_FLT)
    {
      mov->op = par->tok_list->toks[from];
      return;
    }
  }
  uint32_t i = find_next(par, taddsub, 2);
  if (i != par->tok_list->count + 1)
  {
    mov->op = par->tok_list->toks[i];
    mov->left = malloc(sizeof(node_binary_t));
    mov->right = malloc(sizeof(node_binary_t));
    node_binary_tree(from, i, par, mov->left);
    node_binary_tree(i + 1, to, par, mov->right);
  }

  i = find_next(par, tmuldiv, 2);
  if (i != par->tok_list->count + 1)
  {
    mov->op = par->tok_list->toks[i];
    mov->left = malloc(sizeof(node_binary_t));
    mov->right = malloc(sizeof(node_binary_t));
    node_binary_tree(from, i, par, mov->left);
    node_binary_tree(i + 1, to, par, mov->right);
  }
}

void node_binary_tree_delete(node_binary_t *mov)
{
  if (!mov)
    return;
  node_binary_tree_delete(mov);
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