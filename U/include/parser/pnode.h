#include "parser.h"

typedef struct NodeBinary {
  tok_t *op;
  struct NodeBinary *left;
  struct NodeBinary *right;
} node_binary_t;

/*
If right is number but op and left are NULL => this is a number node.
If right is number and op is token but left is NULL => this is a unary operation node.
If none are NULL => this is a binary operation node.
*/

extern node_binary_t **root;

extern void node_binary_tree_root_init();
extern void node_binary_tree(uint32_t from, uint32_t to, parser_t *par, node_binary_t *mov);
extern void node_binary_tree_delete(node_binary_t *mov);
extern void node_binary_tree_print();