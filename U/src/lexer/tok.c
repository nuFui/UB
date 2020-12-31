#include "../../include/lexer/tok.h"

tok_t tok_create(uint8_t type, const char *value)
{
  tok_t t;
  t.type = type;
  t.value = strdup(value);
  return t;
}

void tok_delete(tok_t *tok)
{
  free(tok->value);
  tok->value = NULL;
}

void tok_print(tok_t *tok)
{
  printf("[tok]: type: %d, value: %s\n", tok->type, tok->value);
}

void tok_list_print(tok_list_t *list)
{
  for (int i = 0; i < list->size; ++i)
  {
    tok_print(list->toks[i]);
  }
}

void tok_list_delete(tok_list_t *list)
{
  for (int i = 0; i < list->size; ++i)
  {
    tok_delete(list->toks[i]);
  }
}