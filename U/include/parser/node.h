#ifndef __PARSER_NODE_H__
#define __PARSER_NODE_H__

#include "../lexer/tok.h"

typedef struct NodeNumber node_number_t;
typedef struct NodeBinOp node_binop_t;
typedef struct NodeUnaryOp node_unaryop_t;

typedef struct NodeGeneric
{
  node_number_t *num;
  node_binop_t *binop;
  node_unaryop_t *unop;
} node_generic_t;

struct NodeNumber
{
  tok_t *num;
};

struct NodeBinOp
{
  node_generic_t *left;
  tok_t *op;
  node_generic_t *right;
};

struct NodeUnaryOp
{
  tok_t *op;
  node_generic_t *node;
};

extern node_number_t node_number_create(tok_t *tok);
extern node_binop_t node_binop_create(node_generic_t *l, tok_t *op, node_generic_t *r);

extern void node_generic_print(node_generic_t *gnop);

extern void node_number_print(node_number_t *nn);
extern void node_binop_print(node_binop_t *nbop);
extern void node_unaryop_print(node_unaryop_t *nuop);

#endif