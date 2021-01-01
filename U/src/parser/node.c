#include "../../include/parser/node.h"

node_number_t node_number_create(tok_t *tok)
{
  node_number_t nn;
  tok_copy(&nn.num, tok, 1);
  return nn;
}

node_binop_t node_binop_create(node_generic_t *l, tok_t *op, node_generic_t *r)
{
  node_binop_t nbop;
  nbop.left = l;
  nbop.right = r;
  tok_copy(&nbop.op, op, 1);
  return nbop;
}

void node_generic_print(node_generic_t *gnod)
{
  if (gnod->num)
  {
    node_number_print(gnod->num);
  }
  else if (gnod->binop)
  {
    node_binop_print(gnod->binop);
  }
  else if (gnod->unop)
  {
    node_unaryop_print(gnod->unop);
  }
  else
  {
    return;
  }
}

void node_number_print(node_number_t *nn)
{
  tok_print(nn->num);
}

void node_binop_print(node_binop_t *nbop)
{
  printf("(");
  node_generic_print(nbop->left);
  printf(", ");
  tok_print(nbop->op, 0);
  printf(", ");
  node_generic_print(nbop->right);
  printf(")\n");
}

void node_unaryop_print(node_unaryop_t *nuop)
{
  printf("(");
  node_generic_print(noup->node);
  printf(")\n");
}