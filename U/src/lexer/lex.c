#include "../../include/lexer/lex.h"

#include "../../include/lexer/lex_err.h"

// Creates a number, can raise error on double dots or heap-allocation failure.
// Function cleans up lexer and token if error is triggered.
static void lex_make_number(lexer_t *lex, tok_t *tok) {
  int dot_count = 0;
  int size = 0;
  char *f = lex->cur;
  while (lex->cur && (isdigit(*lex->cur) || *lex->cur == '.')) {
    // For trailing zeros.
    if (*lex->cur == '0' && size == 0) {
      f = lex->cur + 1;
      lex_advance(lex);
      continue;
    }
    if (*lex->cur == '.') {
      if (dot_count == 1) {
        lex_err_illegal_char_t eic = {{lex->pos,
                                       "LexErrIllegalChar",
                                       "More than one decimal dot in number"}};
        lex_err_raise(&eic.base, lex);
      }
      ++dot_count;
    }
    ++size;
    lex_advance(lex);
  }
  tok->value = ualloc(&ERROR_POSITION, size);
  strncpy(tok->value, f, size);
  tok->value[size] = '\0';
  tok->type = dot_count ? TOK_TYPE_FLT : TOK_TYPE_INT;
}

static void lex_make_string(lexer_t *lex, tok_t *tok) {
  char *f = lex->cur;
  int size = 0;
  while (lex->cur && *lex->cur != '"' && *lex->cur != '\'') {
    ++size;
    lex_advance(lex);
  }
  tok->value = ualloc(&ERROR_POSITION, size);
  strncpy(tok->value, f, size);
  tok->value[size] = '\0';
  tok->type = TOK_TYPE_STR;
}

static void lex_make_identifier(lexer_t *lex, tok_t *tok) {
  char *f = lex->cur;
  int size = 0;
  while (lex->cur && (isalpha(*lex->cur) || *lex->cur == '_')) {
    ++size;
    lex_advance(lex);
  }
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  tok->value = ualloc(&pos, size);
  strncpy(tok->value, f, size);
  tok->value[size] = '\0';
  tok->type = TOK_TYPE_IDF;
}

// Creates token given lexer, can raise error if illegal character was found.
static tok_t *lex_make_tok(lexer_t *lex) {
  tok_t *tok = ualloc(&ERROR_POSITION, sizeof(tok_t));
  tok->type = -1;
  tok->line = lex->pos.line;
  tok->column = lex->pos.column;
  tok->value = NULL;
  while (lex->cur) {
    if (isalpha(*lex->cur) || *lex->cur == '_') {
      lex_make_identifier(lex, tok);
      break;
    }
    switch (*lex->cur) {
      case ' ':
      case '\t':
      case '\n':
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
      case '%':
        tok->type = TOK_TYPE_MOD;
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
      case '=':
        tok->type = TOK_TYPE_ASGN;
        lex_advance(lex);
        goto ret;
      case '"':
      case '\'':
        lex_advance(lex);
        lex_make_string(lex, tok);
        lex_advance(lex);
        goto ret;
      default:;
        lex_err_illegal_char_t eic = {{lex->pos,
                                       "ErrIllegalChar",
                                       "Illegal character '%c' found"}};
        lex_err_raise(&eic.base, lex, *lex->cur);
    }
  }
ret:
  return tok;
}

// Return new lexer object coupled with path to file it is lexing.
lexer_t lex_create(const char *path) {
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.pos.file = path; // do not free
  lex_helper_read_file(path, &lex.text, &lex.size);
  lex.cur = lex.text;
  lex_advance(&lex);
  return lex;
}

lexer_t lex_create_from_string(const char *str) {
  lexer_t lex;
  lex.pos.index = -1;
  lex.pos.column = -1;
  lex.pos.line = 1;
  lex.text = strdup(str);
  lex.pos.file = "|stdin|"; // do not free
  lex.size = strlen(lex.text);
  lex.cur = lex.text;
  lex_advance(&lex);
  return lex;
}

void lex_destroy(lexer_t *lex) {
  ufree(lex->text);
}

// Sets lexer cursor to the next character, cursor is set to NULL if former EOF is reached.
void lex_advance(lexer_t *lex) {
  lex_pos_advance(&lex->pos, lex->cur);
  if (lex->pos.index < lex->size) {
    lex->cur = &lex->text[lex->pos.index];
  } else {
    lex->cur = NULL;
  }
}

/*
#define APPEND_EOF 1
#define COUNT_EOF 0
*/

// Given lexer creates the list of tokens for lex->file.
// Can raise error if heap-allocation fails.
tok_list_t *lex_make_toks(lexer_t *lex) {
  tok_list_t *list = ualloc(&ERROR_POSITION, 0);
  list->count = 0;
  while (lex->cur) {
    list = urealloc(&ERROR_POSITION, list, sizeof(tok_list_t) + (list->count + 1) * sizeof(tok_t *));
    list->toks[list->count] = lex_make_tok(lex);
    if (!list->toks[list->count]) {
      ufree(list->toks[list->count]);
      return list;
    }
    list->toks[list->count]->file = lex->pos.file;
    ++list->count;
  }
  // #if APPEND_EOF
  list = urealloc(&ERROR_POSITION, list, sizeof(tok_list_t) + (list->count + 1) * sizeof(tok_t *));
  list->toks[list->count] = ualloc(&ERROR_POSITION, sizeof(tok_t));
  list->toks[list->count]->type = TOK_TYPE_EOF;
  /*#if COUNT_EOF && APPEND_EOF
  ++list->count;
#endif
#endif*/
  return list;
}