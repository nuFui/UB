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

void tok_print(tok_t *tok, int newline)
{
  printf("[type: %d value: %s]", tok->type, tok->value);
  if (newline)
  {
    printf("\n");
  }
}

void tok_copy(tok_t *dest, tok_t *src, int delete_src)
{
  dest->type = src->type;
  dest->value = strdup(src->value);
  if (delete_src)
  {
    tok_delete(src);
  }
}

void tok_list_print(tok_list_t *list)
{
  for (int i = 0; i < list->count; ++i)
  {
    tok_print(list->toks[i], 1);
  }
}

// NOTE: delete_src deletes the tokens in the src if 1.
void tok_list_copy(tok_list_t *dest, tok_list_t *src, int delete_src)
{
  dest->count = src->count;
  for (int i = 0; i < src->count; ++i)
  {
    dest->toks[i] = malloc(sizeof(tok_t));
    tok_copy(dest->toks[i], src->toks[i], delete_src);
  }
  if (delete_src)
  {
    tok_list_delete(src);
  }
}

void tok_list_delete(tok_list_t *list)
{
  for (int i = 0; i < list->count; ++i)
  {
    tok_delete(list->toks[i]);
  }
}