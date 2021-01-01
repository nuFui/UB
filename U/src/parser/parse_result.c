#include "../../include/parser/parse_result.h"

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