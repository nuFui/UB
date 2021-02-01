#ifndef __COMMON_COMPILER_H__
#define __COMMON_COMPILER_H__

/*Toplevel definitions ran by switches.*/

#include "../lexer/lex.h"
#include "../parser/parser.h"
#include "../parser/parser_node.h"
#include "../parser/parser_eval.h"

void run(char *str, lexer_t (*func)(const char *str))
{
  lexer_t lex = func(str);
  tok_list_t *list = lex_make_toks(&lex);
  parser_t par = parser_create(list);
  node_binary_tree_root_init();
  node_binary_tree(0, par.tok_list->count, &par, *root);
  eval_result_t k = node_binary_tree_eval(*root);
  if (k.code == EVAL_FAILURE)
  {
    printf("Failed to evaluate.\n");
    exit(EXIT_SUCCESS);
  }
  printf("%s = %s\n", lex.text, k.result);
  lex_destroy(&lex);
  parser_destroy(&par);
  node_binary_tree_root_deinit(root);
}

void toks(char *str, lexer_t (*func)(const char *str))
{
  lexer_t lex = func(str);
  tok_list_t *list = lex_make_toks(&lex);
  tok_list_print(list, true, true);
  tok_list_delete(list);
  lex_destroy(&lex);
}

void repl()
{
  printf("COMPILER Ulang\n");
  printf("Type stormout() or CTRL+C to exit\n");
  char *str = NULL;
  ssize_t len = 0;
  int read = 0;
  do
  {
    printf("> ");
    read = getline(&str, &len, stdin); // flushes
    if (!strcmp("stormout()\n", str) || read == -1)
    {
      free(str);
      str = NULL;
      break;
    }
    if (!strcmp("\n", str))
    {
      printf("nil\n");
      continue;
    }
    str[read - 1] = '\0'; // because newline is read
    run(str, lex_create_from_string);
    free(str);
    str = NULL;
  } while (true);
}

void help()
{
  printf("U compiler navigation\n");
  printf("\tU/bin/U <command> <option> <src>\n");
  printf("\t\t<command> [run, toks, repl, help]\n");
  printf("\t\t\thelp => displays this message\n");
  printf("\t\t\trun => can only be followed by [-s, -f] (string or path to file which oughts to get run)\n");
  printf("\t\t\ttoks => can only be followed by [-s, -f] (string or path to file from which tokens are extracted\n");
  printf("\t\t\trepl => followed by nothing\n");
}

#endif