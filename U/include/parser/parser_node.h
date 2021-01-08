#ifndef __PARSER_PARSER_NODE_H__
#define __PARSER_PARSER_NODE_H__

#include <math.h>

#include "parser.h"

typedef struct NodeBinary {
  tok_t *op;
  struct NodeBinary *left;
  struct NodeBinary *right;
} node_binary_t;

/*
If right is number but op and left are NULL => this is a number node.
If none are NULL => this is a binary operation node.
*/

extern node_binary_t **root;

extern void node_binary_tree_root_init();
extern void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov);
extern void node_binary_tree_delete(node_binary_t *mov);
extern void node_binary_tree_print();

#endif