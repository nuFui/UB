#include "../../include/lexer/lex.h"
#include "../../include/lexer/err.h"

static void lex_make_number(lexer_t *lex, tok_t *tok)
{
  uint8_t dot_count = 0;
  uint32_t size = 0;
  char *f = lex->cur;
  while (lex->cur && (isdigit(*lex->cur) || *lex->cur == '.'))
  {
    if (*lex->cur == '.')
    {
      if (dot_count == 1)
      {
        lexer_pos_t pos;
        lex_pos_copy(&pos, &lex->pos);
        lex_advance(lex);
        err_illegal_char_t eic = {{pos,
                                   lex->pos,
                                   "ErrIllegalChar",
                                   "More than one decimal dot in number"}};
        err_raise(&eic.base);
      }
      ++dot_count;
    }
    ++size;
    lex_advance(lex);
  }
  tok->value = malloc(sizeof(char) * size);
  strncpy(tok->value, f, size);
  tok->value[size] = '\0';
  tok->type = dot_count ? TOK_TYPE_FLT : TOK_TYPE_INT;
}

static tok_t lex_make_tok(lexer_t *lex)
{
  tok_t tok;
  tok.value = NULL;
  while (lex->cur)
  {
    switch (*lex->cur)
    {
    case ' ':
    case '\t':
      lex_advance(lex);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':;
      lex_make_number(lex, &tok);
      goto ret;
    case '+':
      tok.type = TOK_TYPE_ADD;
      lex_advance(lex);
      goto ret;
    case '-':
      tok.type = TOK_TYPE_SUB;
      lex_advance(lex);
      goto ret;
    case '*':
      tok.type = TOK_TYPE_MUL;
      lex_advance(lex);
      goto ret;
    case '/':
      tok.type = TOK_TYPE_DIV;
      lex_advance(lex);
      goto ret;
    case '(':
      tok.type = TOK_TYPE_LPAR;
      lex_advance(lex);
      goto ret;
    case ')':
      tok.type = TOK_TYPE_RPAR;
      lex_advance(lex);
      goto ret;
    default:;
      lexer_pos_t pos;
      lex_pos_copy(&pos, &lex->pos);
      char current_char = *lex->cur;
      lex_advance(lex);
      err_illegal_char_t eic = {{pos,
                                 lex->pos,
                                 "ErrIllegalChar",
                                 "Illegal character '%c' found"}};
      err_raise(&eic.base, current_char);
    }
  }
ret:
  return tok;
}

lexer_t lex_create(const char *path)
{
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.pos.file = strdup(path);
  lex_helper_read_file(path, &lex.text, &lex.size);
  lex.cur = lex.text;
  lex_advance(&lex);
  return lex;
}

void lex_destroy(lexer_t *lex)
{
  free((char *)lex->pos.file);
  lex->pos.file = NULL;
  free(lex->text);
  lex->text = NULL;
}

void lex_advance(lexer_t *lex)
{
  lex_pos_advance(&lex->pos, lex->cur);
  if (lex->pos.index < lex->size)
  {
    lex->cur = &lex->text[lex->pos.index];
  }
  else
  {
    lex->cur = NULL;
  }
}

tok_list_t lex_make_toks(lexer_t *lex)
{
  tok_list_t list;
  list.size = 0;
  while (lex->cur)
  {
    list.toks[list.size] = malloc(sizeof(tok_t));
    if (!list.toks[list.size])
    {
      free(list.toks[list.size]);
      error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__ - 4};
      error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
    }
    *list.toks[list.size] = lex_make_tok(lex);
    ++list.size;
  }
#if APPEND_EOF
  list.toks[list.size] = malloc(sizeof(tok_t));
  if (!list.toks[list.size])
  {
    free(list.toks[list.size]);
    error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__ - 4};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
  }
  list.toks[list.size]->type = TOK_TYPE_EOF;
  list.toks[list.size]->value = NULL;
#endif
  return list;
}