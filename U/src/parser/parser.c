#include "../../include/parser/parser.h"

static void check_tokens(parser_t *par) {
  for (int i = 0; i < par->tok_list->count; ++i) {
    switch (par->tok_list->toks[i]->type) {
      case TOK_TYPE_EOF:
        break;
      case TOK_TYPE_ADD:
      case TOK_TYPE_SUB:
      case TOK_TYPE_DIV:
      case TOK_TYPE_MUL:
      case TOK_TYPE_MOD:
      case TOK_TYPE_POW:
        switch (par->tok_list->toks[i + 1]->type) {
          case TOK_TYPE_ADD:
          case TOK_TYPE_SUB:
          case TOK_TYPE_DIV:
          case TOK_TYPE_MUL:
          case TOK_TYPE_MOD:
          case TOK_TYPE_POW:
          case TOK_TYPE_RPAR:
          case TOK_TYPE_EOF:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected '(', literal or identifier"},
                             par,
                             symbolize_token_type(par->tok_list->toks[i + 1]->type),
                             symbolize_token_type(par->tok_list->toks[i]->type));
        }
        break;
      case TOK_TYPE_INT:
      case TOK_TYPE_FLT:
      case TOK_TYPE_STR:
      case TOK_TYPE_IDF:
        switch (par->tok_list->toks[i + 1]->type) {
          case TOK_TYPE_INT:
          case TOK_TYPE_FLT:
          case TOK_TYPE_STR:
          case TOK_TYPE_IDF:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i + 1],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected ')', operator or eof"},
                             par,
                             par->tok_list->toks[i + 1]->value,
                             par->tok_list->toks[i]->value);
            break;
          case TOK_TYPE_LPAR:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i + 1],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected ')', operator or eof"},
                             par,
                             symbolize_token_type(par->tok_list->toks[i + 1]->type),
                             par->tok_list->toks[i]->value);
        }
        break;
      case TOK_TYPE_LPAR:
        switch (par->tok_list->toks[i + 1]->type) {
          case TOK_TYPE_MUL:
          case TOK_TYPE_DIV:
          case TOK_TYPE_MOD:
          case TOK_TYPE_POW:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i + 1],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected ')', literal or identifier"},
                             par,
                             symbolize_token_type(par->tok_list->toks[i + 1]->type),
                             symbolize_token_type(par->tok_list->toks[i]->type));
        }
        break;
      case TOK_TYPE_RPAR:
        switch (par->tok_list->toks[i + 1]->type) {
          case TOK_TYPE_LPAR:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i + 1],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected operator, ')' or eof"},
                             par,
                             symbolize_token_type(par->tok_list->toks[i + 1]->type),
                             symbolize_token_type(par->tok_list->toks[i]->type));
          case TOK_TYPE_INT:
          case TOK_TYPE_FLT:
          case TOK_TYPE_STR:
          case TOK_TYPE_IDF:;
            parser_err_raise(&(parser_err_base_t){par->tok_list->toks[i + 1],
                                                  "ErrInvalidSyntax",
                                                  "Found '%s' after '%s', expected operator, ')' or eof"},
                             par,
                             par->tok_list->toks[i + 1]->value,
                             symbolize_token_type(par->tok_list->toks[i]->type));
        }
        break;
      case TOK_TYPE_ASGN:
        switch (par->tok_list->toks[i + 1]->type) {
          case TOK_TYPE_ASGN:
          case TOK_TYPE_RPAR:
          case TOK_TYPE_MUL:
          case TOK_TYPE_DIV:
          case TOK_TYPE_POW:
          case TOK_TYPE_MOD:
          case TOK_TYPE_EOF:;
            parser_err_invalid_syntax eeis = {{par->tok_list->toks[i + 1],
                                               "ErrInvalidSyntax",
                                               "Found '%s' after '%s', expected identifier, literal or '('"}};
            parser_err_raise(&eeis.base,
                             par,
                             symbolize_token_type(par->tok_list->toks[i + 1]->type),
                             symbolize_token_type(par->tok_list->toks[i]->type));
        }
    }
  }
}

parser_t parser_create(tok_list_t *list) {
  parser_t par;
  par.tok_index = 0;
  par.tok_list = list;
  par.tok_cur = list->toks[par.tok_index];
  check_tokens(&par);
  return par;
}

// Clears current token and token list of parser.
void parser_destroy(parser_t *par) {
  /* Noneed beacause the list destroys the current token.
  tok_delete(par->tok_cur);
  par->tok_cur = NULL;
  */
  tok_list_delete(par->tok_list);
  par->tok_list = NULL;
}

// Sets current token to the next one. If there is no next one then current token is NULL.
void parser_advance(parser_t *par) {
  ++par->tok_index;
  if (par->tok_index < par->tok_list->count) {
    par->tok_cur = par->tok_list->toks[par->tok_index];
  } else {
    par->tok_cur = NULL;
  }
}