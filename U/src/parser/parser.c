#include "../../include/parser/parser.h"

static uint8_t in(uint8_t a, uint8_t coll[], uint8_t coll_count)
{
  for (uint8_t i = 0; i < coll_count; ++i)
  {
    if (a == coll[i])
    {
      return 1;
    }
  }
  return 0;
}

parser_t parser_create(tok_list_t *tokens)
{
  parser_t par;
  par.tok_index = -1;
  tok_list_copy(par.tokens, tokens, 1);
  parser_advance(&par);
  return par;
}

void parser_destroy(parser_t *par)
{
  tok_list_delete(par->tokens);
}

void parser_advance(parser_t *par)
{
  ++par->tok_index;
  if (par->tok_index < par->tokens->size)
  {
    par->current_tok = par->tokens->toks[par->tok_index];
  }
}

parse_result_t parser_parse(parser_t *par)
{
  parse_result_t res = parser_expr(par);
  if (res.err)
  {
    return res;
  }
  // TODO Implement
  return res;
}

parse_result_t parser_factor(parser_t *par)
{
  parse_result_t res;
  tok_t t;
  tok_copy(&t, par->current_tok, 1);
  uint8_t ops1[2] = {TOK_TYPE_ADD, TOK_TYPE_SUB};
  uint8_t ops2[2] = {TOK_TYPE_INT, TOK_TYPE_FLT};
  if (in(t.type, ops1, 2))
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    parse_result_t facres = parser_factor(par);
    parse_result_t *fac = parse_result_register(&res, &facres);
    if (res.err)
    {
      return res;
    }
    node_unaryop_t unop = {
        &t,
        fac->node};
    node_generic_t ngen = {
        NULL,
        NULL,
        &unop};
    return *parse_result_success(&res, &ngen);
  }
  else if (in(t.type, ops2, 2))
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    node_number_t num = {&t};
    node_generic_t ngen = {
        &num,
        NULL,
        NULL};
    return *parse_result_success(&res, &ngen);
  }
  else if (t.type == TOK_TYPE_LPAR)
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    parse_result_t exprres = parser_expr(par);
    parse_result_t *expr = parse_result_register(&res, &exprres);
    if (res.err)
    {
      return res;
    }
    if (par->current_tok->type == TOK_TYPE_RPAR)
    {
      // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
      parse_result_register(&res, NULL);
      return *parse_result_success(&res, expr->node);
    }
    else
    {
      err_invalid_syntax_t eis = {
          {par->pos.start_pos,
           par->pos.end_pos,
           "ErrInvalidSyntax",
           "Expected ')'"}};
      parse_result_t *fail = parse_result_failure(&res, &eis);
      return *fail;
    }
  }
}

parse_result_t parser_term(parser_t *par)
{
  uint8_t ops[2] = {TOK_TYPE_MUL, TOK_TYPE_DIV};
  return parser_binop(par, &parser_factor, ops, 2);
}

parse_result_t parser_expr(parser_t *par)
{
  uint8_t ops[2] = {TOK_TYPE_ADD, TOK_TYPE_SUB};
  return parser_binop(par, &parser_term, ops, 2);
}

parse_result_t parser_binop(parser_t *par, parse_result_t (*func)(parser_t *par), uint8_t types[], uint8_t types_count)
{
  parse_result_t res;
  node_binop_t bin;
  parse_result_t temp = func(par);
  parse_result_t *left = parse_result_register(&res, &temp);
  if (res.err)
  {
    return res;
  }
  while (in(par->current_tok->type, types, types_count))
  {
    tok_t *op_tok = par->current_tok;
    tok_copy(op_tok, par->current_tok, 1);
    parser_advance(par);
    parse_result_register(&res, NULL);
    parse_result_t temp = func(par)
        parse_result_t *right = parse_result_register(&res, &temp);
    if (res.err)
    {
      return res;
    }
    bin = node_binop_create(left->node, op_tok, right->node);
  }
  node_generic_t ngen = {
      NULL,
      &bin,
      NULL};
  res.node = &ngen;
  return res;
}