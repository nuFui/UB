#include "../include/lex.h"
#include "../include/error.h"

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
        lexer_pos_t pos = lex_pos_copy(&lex->pos);
        lex_advance(lex);
        error_illegal_char_t eic = {{pos,
                                     lex->pos,
                                     "ErrorIllegalChar",
                                     "More than one decimal dot in number"}};
        error_raise(&eic.base);
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
      lexer_pos_t pos = lex_pos_copy(&lex->pos);
      char current_char = *lex->cur;
      lex_advance(lex);
      error_illegal_char_t eic = {{pos,
                                   lex->pos,
                                   "ErrorIllegalChar",
                                   "Illegal character '%c' found"}};
      error_raise(&eic.base, current_char);
    }
  }
ret:
  return tok;
}

void lex_pos_advance(lexer_pos_t *lex_pos, char *cur)
{
  ++lex_pos->index;
  ++lex_pos->column;
  if (*cur == '\n')
  {
    ++lex_pos->line;
    lex_pos->column = 0;
  }
}

lexer_pos_t lex_pos_copy(lexer_pos_t *lex_pos)
{
  return *lex_pos;
}

lexer_t lex_create(const char *path)
{
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.pos.file = strdup(path);
  lexh_read_file(path, &lex.text, &lex.size);
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

void lex_make_toks(lexer_t *lex, tok_list_t *list)
{
  list->size = 0;
  while (lex->cur)
  {
    list->toks[list->size] = malloc(sizeof(tok_t));
    *list->toks[list->size] = lex_make_tok(lex);
    ++list->size;
  }
}