#include "../../include/lexer/lex.h"
#include "../../include/lexer/lex_err.h"

// Creates a number, can raise error on double dots or heap-allocation failure.
// Function cleans up lexer and token if error is triggered.
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
        lex_destroy(lex);
        lex_err_illegal_char_t eic = {{lex->pos,
                                       "LexErrIllegalChar",
                                       "More than one decimal dot in number"}};
        lex_err_raise(&eic.base);
      }
      ++dot_count;
    }
    ++size;
    lex_advance(lex);
  }
  tok->value = malloc(sizeof(char) * size);
  if (!tok->value)
  {
    lex_destroy(lex);
    tok_delete(tok);
    error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
  }
  strncpy(tok->value, f, size);
  tok->value[size] = '\0';
  tok->type = dot_count ? TOK_TYPE_FLT : TOK_TYPE_INT;
}

// Creates token given lexer, can raise error if illegal character was found.
static tok_t *lex_make_tok(lexer_t *lex)
{
  tok_t *tok = malloc(sizeof(tok_t));
  tok->type = -1;
  tok->line = lex->pos.line;
  tok->column = lex->pos.column;
  tok->value = NULL;
  while (lex->cur)
  {
    switch (*lex->cur)
    {
    case ' ':
    case '\t':
    case '\n':
      lex_advance(lex);
      if (!lex->cur)
      {
        free(tok);
        return NULL;
      }
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
      lex_make_number(lex, tok);
      goto ret;
    case '+':
      tok->type = TOK_TYPE_ADD;
      lex_advance(lex);
      goto ret;
    case '-':
      tok->type = TOK_TYPE_SUB;
      lex_advance(lex);
      goto ret;
    case '*':
      tok->type = TOK_TYPE_MUL;
      lex_advance(lex);
      goto ret;
    case '/':
      tok->type = TOK_TYPE_DIV;
      lex_advance(lex);
      goto ret;
    case '^':
      tok->type = TOK_TYPE_POW;
      lex_advance(lex);
      goto ret;
    case '(':
      tok->type = TOK_TYPE_LPAR;
      lex_advance(lex);
      goto ret;
    case ')':
      tok->type = TOK_TYPE_RPAR;
      lex_advance(lex);
      goto ret;
    default:;
      lex_err_illegal_char_t eic = {{lex->pos,
                                     "ErrIllegalChar",
                                     "Illegal character '%c' found"}};
      lex_err_raise(&eic.base, *lex->cur);
    }
  }
ret:
  return tok;
}

// Return new lexer object coupled with path to file it is lexing.
lexer_t lex_create(const char *path)
{
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.pos.file = path; // Not obtained through malloc() => dont free the file name buffer.
  lex_helper_read_file(path, &lex.text, &lex.size);
  lex.cur = lex.text;
  lex_advance(&lex);
  return lex;
}

lexer_t lex_create_from_string(const char *str)
{
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.text = strdup(str);
  lex.pos.file = "<stdin>"; // Do not malloc(), string literal handled by compiler.
  lex.size = strlen(lex.text);
  lex.cur = lex.text;
  lex_advance(&lex);
  return lex;
}

void lex_destroy(lexer_t *lex)
{
  free(lex->text);
  lex->text = NULL;
}

// Sets lexer cursor to the next character, cursor is set to NULL if former EOF is reached.
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

// Given lexer creates the list of tokens for lex->file.
// Can raise error if heap-allocation fails.
tok_list_t lex_make_toks(lexer_t *lex)
{
  tok_list_t list;
  list.count = 0;
  list.toks = malloc(sizeof(tok_t *));
  if (!list.toks)
  {
    free(list.toks);
    lex_destroy(lex);
    error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
  }
  while (lex->cur)
  {
    list.toks[list.count] = malloc(sizeof(tok_t));
    if (!list.toks[list.count])
    {
      free(list.toks[list.count]);
      lex_destroy(lex);
      error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
      error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
    }
    list.toks[list.count] = lex_make_tok(lex);
    if (!list.toks[list.count])
    {
      free(list.toks[list.count]);
      list.toks[list.count] = NULL;
      return list;
    }
    list.toks[list.count]->file = lex->pos.file;
    ++list.count;
  }
#if APPEND_EOF
  list.toks[list.count] = malloc(sizeof(tok_t));
  if (!list.toks[list.count])
  {
    free(list.toks[list.count]);
    lex_destroy(lex);
    error_pos_t pos = {__FILE__, __FUNCTION__, __LINE__};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory");
  }
  list.toks[list.count]->type = TOK_TYPE_EOF;
  list.toks[list.count]->value = NULL;
#endif
  return list;
}