#include "../../include/lexer/tok.h"

tok_t tok_create(tok_type_t type, const char *value, const char *file)
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
  ufree(tok->value);
  tok->file = NULL;
}

static char *stringify_token_type(tok_t *tok)
{
  switch (tok->type)
  {
  case TOK_TYPE_ADD:
    return "add";
  case TOK_TYPE_SUB:
    return "subtract";
  case TOK_TYPE_MUL:
    return "multiply";
  case TOK_TYPE_DIV:
    return "divide";
  case TOK_TYPE_POW:
    return "exponentiate";
  case TOK_TYPE_LPAR:
    return "left parentheses";
  case TOK_TYPE_RPAR:
    return "right parentheses";
  case TOK_TYPE_INT:
    return "integer literal";
  case TOK_TYPE_FLT:
    return "float literal";
  case TOK_TYPE_STR:
    return "string literal";
  case TOK_TYPE_EOF:
    return "end";
  default:
    return "unknown";
  }
}

void tok_print(tok_t *tok, bool newline, bool verbose, bool stringify)
{
  if (stringify)
  {
    printf("[type: %s ", stringify_token_type(tok));
  }
  else
  {
    printf("[type: %d ", tok->type);
  }
  if (verbose)
  {
    printf("line: %d column: %d file: %s ", tok->line, tok->column, tok->file);
  }
  printf("value: %s]", tok->value);
  fflush(stdin);
  if (newline)
  {
    printf("\n");
  }
}

void tok_copy(tok_t *dest, tok_t *src, bool delete_src)
{
  dest->type = src->type;
  dest->line = src->line;
  dest->column = src->column;
  dest->file = src->file;
  if (!dest->file || !dest->value)
  {
    tok_delete(dest);
    tok_delete(src);
    error_pos_t pos = {__FILE__, __func__, __LINE__};
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

void tok_list_print(tok_list_t *list, bool verbose, bool stringify)
{
  for (int i = 0; i < list->count; ++i)
  {
    tok_print(list->toks[i], true, verbose, stringify);
  }
}

// NOTE: delete_src deletes the tokens in the src if 1.
void tok_list_copy(tok_list_t *dest, tok_list_t *src, bool delete_src)
{
  dest->count = src->count;
  for (int i = 0; i < src->count; ++i)
  {
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    dest->toks[i] = ualloc(&pos, sizeof(tok_t));
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
    list->toks[i] = NULL;
  }
  list = NULL;
}