#include "../../include/lexer/tok.h"

tok_t tok_create(uint8_t type, const char *value, const char *file)
{
  tok_t t;
  t.type = type;
  t.line = 1;
  t.column = 1;
  t.file = file;
  t.value = strdup(value);
  return t;
}

void tok_delete(tok_t *tok)
{
  free(tok->value);
  tok->value = NULL;
}

void tok_print(tok_t *tok, int newline, int verbose)
{
  printf("[type: %d ", tok->type);
  if (verbose)
  {
    printf("line: %d column: %d file: %s ", tok->line, tok->column, tok->file);
  }
  printf("value: %s]", tok->value);
  if (newline)
  {
    printf("\n");
  }
}

void tok_copy(tok_t *dest, tok_t *src, int delete_src)
{
  dest->type = src->type;
  dest->line = src->line;
  dest->column = src->column;
  dest->file = src->file;
  if (!dest->file || !dest->value)
  {
    tok_delete(dest);
    tok_delete(src);
    error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
  }
  if (src->value)
  {
    dest->value = strdup(src->value);
  }
  if (delete_src)
  {
    tok_delete(src);
  }
}

void tok_list_print(tok_list_t *list, int verbose)
{
  for (int i = 0; i < list->count; ++i)
  {
    tok_print(list->toks[i], 1, verbose);
  }
}

// NOTE: delete_src deletes the tokens in the src if 1.
void tok_list_copy(tok_list_t *dest, tok_list_t *src, int delete_src)
{
  dest->count = src->count;
  for (int i = 0; i < src->count; ++i)
  {
    dest->toks[i] = malloc(sizeof(tok_t));
    if (!dest->toks[i])
    {
      free(dest->toks[i]);
      tok_list_delete(dest);
      tok_list_delete(src);
      error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
      error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
    }
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