#include "../../include/parser/parser.h"

static void check_tokens(parser_t *par)
{
  for (int i = 0; i < par->tok_list->count - 1; ++i)
  {
    switch (par->tok_list->toks[i]->type)
    {
    case TOK_TYPE_ADD:
    case TOK_TYPE_SUB:
    case TOK_TYPE_DIV:
    case TOK_TYPE_MUL:
    case TOK_TYPE_POW:
      switch (par->tok_list->toks[i + 1]->type)
      {
      case TOK_TYPE_ADD:
      case TOK_TYPE_SUB:
      case TOK_TYPE_DIV:
      case TOK_TYPE_MUL:
      case TOK_TYPE_POW:
      case TOK_TYPE_RPAR:
      case TOK_TYPE_EOF:;
        parser_err_invalid_syntax peis = {{par->tok_list->toks[i + 1],
                                           "ErrInvalidSyntax",
                                           "Found '%s' after '%s', expected '(', literal or identifier"}};

        parser_err_raise(&peis.base, par, par->tok_list->toks[i + 1]->type, par->tok_list->toks[i]->type);
      }
      break;
    case TOK_TYPE_INT:
    case TOK_TYPE_FLT:
      switch (par->tok_list->toks[i + 1]->type)
      {
      case TOK_TYPE_INT:
      case TOK_TYPE_FLT:
      case TOK_TYPE_LPAR:;
        parser_err_invalid_syntax peis = {{par->tok_list->toks[i + 1], "ErrInvalidSyntax", "Found '%s' after '%s', expected ')', operator or eof"}};
        parser_err_raise(&peis.base, par, par->tok_list->toks[i + 1]->type, par->tok_list->toks[i]->type);
      }
      break;
    case TOK_TYPE_LPAR:
      switch (par->tok_list->toks[i + 1]->type)
      {
        /* Can be followed by unary + or minus.
      case TOK_TYPE_ADD:
      case TOK_TYPE_SUB:
      */
      case TOK_TYPE_MUL:
      case TOK_TYPE_DIV:
      case TOK_TYPE_POW:
      case TOK_TYPE_EOF:;
        parser_err_invalid_syntax peis = {{par->tok_list->toks[i + 1], "ErrInvalidSyntax", "Found '%s' after '%s', expected ')', literal or identifier"}};
        parser_err_raise(&peis.base, par, par->tok_list->toks[i + 1]->type, par->tok_list->toks[i]->type);
      }
      break;
    case TOK_TYPE_RPAR:
      switch (par->tok_list->toks[i + 1]->type)
      {
      case TOK_TYPE_LPAR:
      case TOK_TYPE_INT:
      case TOK_TYPE_FLT:;
        parser_err_invalid_syntax peis = {{par->tok_list->toks[i + 1], "ErrInvalidSyntax", "Found '%s' after '%s', expected operator, ')' or eof"}};
        parser_err_raise(&peis.base, par, par->tok_list->toks[i + 1]->type, par->tok_list->toks[i]->type);
      }
    }
  }
}

parser_t parser_create(tok_list_t *list)
{
  parser_t par;
  par.tok_index = 0;
  par.tok_list = list;
  par.tok_cur = list->toks[par.tok_index];
  check_tokens(&par);
  return par;
}

// Clears current token and token list of parser.
void parser_destroy(parser_t *par)
{
  tok_delete(par->tok_cur);
  par->tok_cur = NULL;
  tok_list_delete(par->tok_list);
  par->tok_list = NULL;
}

// Sets current token to the next one. If there is no next one then current token is NULL.
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