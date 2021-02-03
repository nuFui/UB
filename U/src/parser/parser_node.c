#include "../../include/parser/parser_node.h"

node_binary_t **root = NULL;
static int scope = 0;

void node_binary_tree_root_init()
{
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  root = ualloc(&pos, sizeof(node_binary_t *));
  *root = ualloc(&pos, sizeof(node_binary_t));
}

void node_binary_tree_root_deinit()
{
  ufree(*root);
  ufree(root);
}

// Finds smallest operation in the current scope.
static int find_next_op(parser_t *par, int from, int to)
{
  int scp = scope;
  int smallest_index = -1;
  tok_type_t smallest = TOK_TYPE_DUMMY_MAX;
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
    case TOK_TYPE_ADD:
    case TOK_TYPE_SUB:
    case TOK_TYPE_DIV:
    case TOK_TYPE_MUL:
    case TOK_TYPE_POW:
    case TOK_TYPE_ASGN:
      if (scp == scope)
      {
        // If minus, find rightmost one.
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
      break;
    default:
      break;
    }
    ++from;
  }
  return smallest_index;
}

// Constructs binary tree of operations.
// How expression 2 * 4 looks like:
//                 BINOP
//              /   |   \
//        BINOP     *    BINOP
//      /   |  \       /   |   \
//  NULL   2   NULL  NULL  4  NULL

void node_binary_tree(int from, int to, parser_t *par, node_binary_t *mov)
{
  mov->op = NULL;
  mov->left = NULL;
  mov->right = NULL;

  if (to == from)
  {
    // Unary minus or plus => insert '0' to the left.
    if (par->tok_list->toks[from]->type == TOK_TYPE_SUB || par->tok_list->toks[from]->type == TOK_TYPE_ADD)
    {
      error_pos_t pos = {__FILE__, __func__, __LINE__};
      mov->op = ualloc(&pos, sizeof(tok_t));
      mov->op->type = TOK_TYPE_INT;
      mov->op->line = -1;
      mov->op->column = -1;
      mov->op->file = par->tok_list->toks[from]->file;
      mov->op->value = "0";
      return;
    }
  }

  if (to - from == 1)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_INT || 
        par->tok_list->toks[from]->type == TOK_TYPE_FLT ||
        par->tok_list->toks[from]->type == TOK_TYPE_STR ||
        par->tok_list->toks[from]->type == TOK_TYPE_IDF)
    {
      mov->op = par->tok_list->toks[from];
      return;
    }
  }

  int i = find_next_op(par, from, to);
  if (i != -1)
  {
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    mov->op = par->tok_list->toks[i];
    mov->left = ualloc(&pos, sizeof(node_binary_t));
    mov->right = ualloc(&pos, sizeof(node_binary_t));
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
  ufree(mov->op);
  node_binary_tree_delete(mov->left);
  node_binary_tree_delete(mov->right);
  ufree(mov->left);
  ufree(mov->right);
}

void node_binary_tree_print(node_binary_t *mov)
{
  if (!mov)
    return;
  tok_print(mov->op, true, true, true);
  node_binary_tree_print(mov->left);
  node_binary_tree_print(mov->right);
}