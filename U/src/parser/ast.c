#include "../../include/parser/ast.h"

binary_node_t **root;
static int scope = 0;

enum
{
  TREE_STATUS_SUCCESS,
  TREE_STATUS_FAILURE
};

struct TreeLevelInfo
{
  int status;
  int subtree_count;
  int **indices;
};

static struct TreeLevelInfo getTreeInfo(parser_t *par, int from, int to)
{
  struct TreeLevelInfo info;
  int scp = scope;
  info.status = TREE_STATUS_SUCCESS;
  info.subtree_count = 0;
  info.indices = malloc(sizeof(int *));
  if (!info.indices)
  {
    info.status = TREE_STATUS_FAILURE;
    return info;
  }
  int smallest_type = TOK_TYPE_DUMMY_MAX;
  for (from; from < to; ++from)
  {
    switch (par->tok_list->toks[from]->type)
    {
    case TOK_TYPE_LPAR:
      ++scp;
      break;
    case TOK_TYPE_RPAR:
      --scp;
      break;
    }
    if (scope == scp && par->tok_list->toks[from]->type < smallest_type)
    {
      smallest_type = par->tok_list->toks[from]->type;
    }
  }
  for (int i = 0; i < to; ++i)
  {
    switch (par->tok_list->toks[i]->type)
    {
    case TOK_TYPE_LPAR:
      ++scp;
      break;
    case TOK_TYPE_RPAR:
      --scp;
      break;
    }
    if (scope == scp && par->tok_list->toks[i]->type == smallest_type)
    {
      info.indices[info.subtree_count] = malloc(sizeof(int));
      if (!info.indices[info.subtree_count])
      {
        info.status = TREE_STATUS_FAILURE;
        return info;
      }
      *info.indices[info.subtree_count] = i;
      ++info.subtree_count;
    }
  }
  if (info.subtree_count)
    return info;
}

void binary_tree_init()
{
  root = malloc(sizeof(binary_node_t *));
}

void create_binary_tree(parser_t *par, binary_node_t *mov, int from, int to)
{
  mov->data = NULL;
  mov->left = NULL;
  mov->right = NULL;
  if (to - from == 1)
  {
    if (par->tok_list->toks[from]->type == TOK_TYPE_INT || par->tok_list->toks[from]->type == TOK_TYPE_FLT)
    {
      mov->data = par->tok_list->toks[from];
      return;
    }
  }
  struct TreeLevelInfo info = getTreeInfo(par, from, to);
  if (info.status == TREE_STATUS_SUCCESS)
  {
    for (int i = 0; i < to - 1; ++i)
    {
      mov->data = par->tok_list->toks[*info.indices[i]];
      mov->left = malloc(sizeof(binary_node_t));
      mov->right = malloc(sizeof(binary_node_t));
      create_binary_tree(par, mov->left, from, *info.indices[i]);
      create_binary_tree(par, mov->right, *info.indices[i], *info.indices[i + 1]);
    }
  }
}

void delete_binary_tree(binary_node_t *mov)
{
  if (!mov)
    return;
  free(mov->data);
  mov->data = NULL;
  delete_binary_tree(mov->left);
  delete_binary_tree(mov->right);
  free(mov->left);
  free(mov->right);
}