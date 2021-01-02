#include "../../include/parser/parser_node.h"

parser_node_t parser_node_create(uint8_t type, tok_t *data)
{
  parser_node_t par_node;
  par_node.type = type;
  par_node.child_count = 0;
  par_node.bro_count = 0;
  par_node.data = data;
  par_node.child = NULL;
  par_node.bro = NULL;
  return par_node;
}

static void parser_node_shallow_delete(parser_node_t *par_node)
{
  par_node->child_count = 0;
  par_node->bro_count = 0;
  tok_delete(par_node->data);
}

void parser_node_kill_both(parser_node_t *par_node)
{
  if (!par_node)
    return;
  parser_node_shallow_delete(par_node);
  parser_node_kill_both(par_node->child);
  parser_node_kill_both(par_node->bro);
  free(par_node->child);
  free(par_node->bro);
  free(par_node);
}

PARSER_NODE_KILL(child)
PARSER_NODE_KILL(bro)