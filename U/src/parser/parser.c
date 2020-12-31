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

parse_result_t *parse_result_register(parse_result_t *self, parse_result_t *res)
{
  if (res->err)
  {
    self->err = malloc(sizeof(err_base_t));
    err_copy(self->err, res->err, 1);
  }
  return self;
}

parse_result_t *parse_result_success(parse_result_t *self, generic_node_t *node)
{
  self->node = node;
  return self;
}

parse_result_t *parse_result_failure(parse_result_t *self, error_base_t *err)
{
  self->err = malloc(sizeof(err_base_t));
  err_copy(self->err, err, 1);
  return self;
}

parser_t parser_create(tok_list_t *tokens)
{
  parser_t par;
  par.tok_index = -1;
  tok_list_copy(par.tokens, tokens);
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
}

parse_result_t parser_factor(parser_t *par)
{
  parse_result_t res;
  tok_t t;
  tok_copy(&t, par->current_tok);
  if (in(t.type, {TOK_TYPE_ADD, TOK_TYPE_SUB}, 2))
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    parse_result_t fac = parse_result_register(&res, parser_factor(par));
    if (res.err)
    {
      return res;
    }
    node_generic_t ngen = {
        NULL,
        NULL,
        {&t,
         &fac}};
    return parse_result_success(&res, &ngen);
  }
  else if (in(t.type), {TOK_TYPE_INT, TOK_TYPE_FLT})
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    node_generic_t ngen = {
        {&t},
        NULL,
        NULL};
    return parse_result_success(&res, &ngen)
  }
  else if (t.type == TOK_TYPE_LPAR)
  {
    // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
    parse_result_register(&res, NULL);
    parse_result_t expr = parse_result_register(&res, parser_expr(par));
    if (res.err)
    {
      return res;
    }
    if (par->current_tok.type == TOK_TYPE_RPAR)
    {
      // PARSE_RESULT_REGISTER NEEDS SECOND PARAM
      parse_result_register(&res, NULL);
      return parse_result_success(&res, expr.node);
    }
    else
    {
      err_invalid_syntax_t eis = {
          {par->current_tok.start_pos,
           par->current_tok.end_pos,
           "ErrInvalidSyntax",
           "Expected ')'"}};
      return parse_result_failure(&res, &eis);
    }
  }
}

parse_result_t parser_term(parser_t *par)
{
  return parser_binop(par, parser_factor(par), {TOK_TYPE_MUL, TOK_TYPE_DIV}, 2);
}

parse_result_t parser_expr(parser_t *par)
{
  return parser_binop(par, parser_term(par), {TOK_TYPE_ADD, TOK_TYPE_SUB}, 2);
}

parse_result_t parser_binop(parser_t *par, parse_result_t (*func)(parser_t *par), uint8_t types[], uint8_t types_count)
{
  parse_result_t res;
  parse_result_register(&res, (*func)(par));
  if (res.err)
  {
    return res;
  }
  while (in(par->current_tok.type, types, types_count))
  {
    tok_t op_tok = par->current_tok;
    tok_copy(&op_tok, par->current_tok, 1);
    parser_advance(par);
    parse_result_register(&res, (*func)(par));
    if (res.err)
    {
      return res;
    }
    node_binop_create()
  }
}