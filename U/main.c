#include "include/lexer/lex.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    // Handle error.
    exit(EXIT_FAILURE);
  }
  if (!strcmp(argv[1], "lex"))
  {
    lexer_t lex = lex_create(argv[2]);
    tok_list_t list = lex_make_toks(&lex);
    tok_list_print(&list);
    tok_list_delete(&list);
    lex_destroy(&lex);
  }
}