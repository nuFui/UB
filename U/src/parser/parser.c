#include "../../include/parser/parser.h"

parser_t parser_create(tok_list_t *list)
{
  parser_t par;
  par.tok_index = 0;
  par.tok_list = list;
  par.tok_cur = list->toks[par.tok_index];
  return par;
}

void parser_destroy(parser_t *par)
{
  tok_delete(par->tok_cur);
  tok_list_delete(par->tok_list);
}

void parser_advance(parser_t *par)
{
  ++par->tok_index;
  if (par->tok_index < par->tok_list->count)
  {
    par->tok_cur = par->tok_list->toks[par->tok_index];
  }
  else
  {
    par->tok_cur = NULL;
  }
}