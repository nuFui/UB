#ifndef __PARSER_PARSER_NODE_H__
#define __PARSER_PARSER_NODE_H__

#include "parser.h"

#define PARSER_NODE_KILL(what)                          \
  void parser_node_kill_##what(parser_node_t *par_node) \
  {                                                     \
    if (!par_node)                                      \
      return;                                           \
    parser_node_shallow_delete(par_node);               \
    tok_delete(par_node->data);                         \
    parser_node_kill_##what(par_node->what);            \
    free(par_node->what);                               \
    free(par_node);                                     \
  }

enum
{
  NODE_EXPRESSION,
  NODE_BINARY_OP,
  NODE_UNARY_OP
};

typedef struct ParserNode
{
  uint8_t type;
  uint32_t child_count;
  uint32_t bro_count;
  tok_t *data;
  struct ParserNode *child;
  struct ParserNode *bro;
} parser_node_t;

extern parser_node_t parser_node_create(uint8_t type, tok_t *data);
extern void parser_node_kill_both(parser_node_t *par_node);
extern void parser_node_kill_child(parser_node_t *par_node);
extern void parser_node_kill_bro(parser_node_t *par_node);

#endif