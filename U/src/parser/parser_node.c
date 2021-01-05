#include "../../include/parser/parser_node.h"

static parser_node_t **root = NULL;

static void parser_node_shallow_delete(parser_node_t *par_node)
{
  par_node->child_count = 0;
  par_node->bro_count = 0;
  tok_delete(par_node->data);
}

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




/*void parser_parse(parser_t *par, parser_node_

/*void parser_parse(parser_t *par, parser_node_t *mov)
{
  if (par->tok_cur->type == TOK_TYPE_INT || par->tok_cur->type == TOK_TYPE_FLT)
  {
    tok_t *cur = par->tok_cur;
    parser_advance(par);
    if (par->tok_cur->type == TOK_TYPE_INT || par->tok_cur->type == TOK_TYPE_FLT)
    {
      parser_err_invalid_syntax eis = {
          {par->tok_cur,
           "ParserErrInvalidSyntax",
           "Expected operator or none after numbern but got number"}};
      parser_destroy(par);
      parser_node_kill_both(mov);
      parser_err_raise(&eis.base);
    }
    if (par->tok_cur->type == TOK_TYPE_ADD || par->tok_cur->type == TOK_TYPE_SUB)
    {
      tok_t *curcur = par->tok_cur;
      parser_advance(par);
      if (par->tok_cur->type == TOK_TYPE_ADD ||
          par->tok_cur->type == TOK_TYPE_SUB ||
          par->tok_cur->type == TOK_TYPE_MUL ||
          par->tok_cur->type == TOK_TYPE_DIV)
      {
        parser_err_invalid_syntax eis = {{par->tok_cur, "ParserErrInvalidSyntax", "Expected number or '(' after operator"}};
        parser_destroy(par);
        parser_node_kill_both(mov);
        parser_err_raise(&eis.base);
      }
      if (par->tok_cur->type == TOK_TYPE_INT || par->tok_cur->type == TOK_TYPE_FLT)
      {
        mov->type = NODE_BINARY_OP;
        mov->child_count += 3;
        mov->child = malloc(sizeof(parser_node_t) * 3);
        (mov->child + 0)->type = NODE_NUMBER;
        (mov->child + 0)->data = cur;
        (mov->child + 0)->bro_count = 3;
        (mov->child + 0)->bro = (mov->child + sizeof(parser_node_t));

        (mov->child + sizeof(parser_node_t))->type = NODE_DUMMY;
        (mov->child + sizeof(parser_node_t))->data = curcur;
        (mov->child + sizeof(parser_node_t))->bro_count = 3;
        (mov->child + sizeof(parser_node_t))->bro = (mov->child + sizeof(parser_node_t) * 2);

        (mov->child + sizeof(parser_node_t) * 2)->type = NODE_NUMBER;
        (mov->child + sizeof(parser_node_t) * 2)->data = par->tok_cur;
        (mov->child + sizeof(parser_node_t) * 2)->bro_count = 3;
        (mov->child + sizeof(parser_node_t) * 2)->bro = NULL; // No bros to the right.
      }
      else if (par->tok_cur->type == TOK_TYPE_LPAR)
      {
        parser_node_t *temp = mov->bro + mov->bro_count * sizeof(parser_node_t);
        temp = malloc(sizeof(parser_node_t));
        if (!temp)
        {
          parser_node_kill_both(mov);
          parser_destroy(par);
          error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
          error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
        }
        parser_parse(par, mov->bro + mov->bro_count++ * sizeof(parser_node_t));
      }
    }
  }
}*/