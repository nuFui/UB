#ifndef __PARSER_AST_H__
#define __PARSER_AST_H__

#include "parser.h"

typedef struct BinaryNode
{
  tok_t *data;
  struct BinaryNode *left;
  struct BinaryNode *right;
} binary_node_t;

extern binary_node_t **root;

extern void binary_tree_init();
extern void create_binary_tree(parser_t *par, binary_node_t *mov, int from, int to);
extern void delete_binary_tree(binary_node_t *mov);

#endif