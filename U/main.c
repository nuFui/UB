#include "include/lexer/lex.h"
#include "include/parser/ast.h"

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
    tok_list_print(&list, 1);
    tok_list_delete(&list);
    lex_destroy(&lex);
  }
  if (!strcmp(argv[1], "tree"))
  {
    lexer_t lex = lex_create(argv[2]);
    tok_list_t list = lex_make_toks(&lex);
    tok_list_print(&list, 1);
    parser_t par = parser_create(&list);
    /*node_binary_tree_root_init();
    node_binary_tree(0, par.tok_list->count, &par, *root);
    struct EvalResult k = node_binary_tree_eval(NULL, *root);
        printf("%s = %f\n", lex.text, k.result);
    */
    binary_tree_init();
    create_binary_tree(&par, *root, 0, par.tok_list->count);
    tok_list_delete(&list);
    lex_destroy(&lex);
    parser_destroy(&par);
    delete_binary_tree(*root);
  }
}