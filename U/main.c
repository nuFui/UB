#include "include/lexer/lex.h"
#include "include/parser/parser.h"

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
    parser_t par = parser_create(&list);
    parser_advance(&par);
    tok_list_delete(&list);
    lex_destroy(&lex);
    parser_destroy(&par);
  }
}