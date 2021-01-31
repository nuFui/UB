#include "../../include/lexer/lex.h"
#include "../../include/parser/parser.h"
#include "../../include/parser/parser_node.h"
#include "../../include/parser/parser_eval.h"

#include <dirent.h>
#include <errno.h>
#include <unistd.h>

void run_proc(char *argv[], lexer_t (*func)(const char *str))
{
  lexer_t lex = func(argv[3]);
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
  printf("%s = %f\n", lex.text, k.result);
  lex_destroy(&lex);
  parser_destroy(&par);
  node_binary_tree_root_deinit(root);
}

void toks_proc(char *argv[], lexer_t (*func)(const char *str))
{
  lexer_t lex = func(argv[3]);
  tok_list_t *list = lex_make_toks(&lex);
  tok_list_print(list, 1);
  tok_list_delete(list);
  lex_destroy(&lex);
}

void repl_proc(char *line, lexer_t (*func)(const char *str))
{
  lexer_t lex = func(line);
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
  printf("%s = %f\n", lex.text, k.result);
  lex_destroy(&lex);
  parser_destroy(&par);
  node_binary_tree_root_deinit(root);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    // Handle error.
    exit(EXIT_FAILURE);
  }
  // TODO: Argument parser.
  if (!strcmp(argv[1], "run"))
  {
    if (argc > 2)
    {
      if (!strcmp(argv[2], "-f"))
      {
        // For files.
        if (!access(argv[3], F_OK))
        {
          // File exists => proceed with evaluation.
          run_proc(argv, lex_create);
        }
        else
        {
          // File does not exist => exit with 1.
          exit(EXIT_FAILURE);
        }
      }
      else if (!strcmp(argv[2], "-s"))
      {
        // For strings.
        run_proc(argv, lex_create_from_string);
      }
      else
      {
        exit(EXIT_FAILURE);
      }
    }
  }
  else if (!strcmp(argv[1], "toks"))
  {
    if (argc > 2)
    {
      if (!strcmp(argv[2], "-f"))
      {
        if (!access(argv[2], F_OK))
        {
          toks_proc(argv, lex_create);
        }
        else
        {
          exit(EXIT_FAILURE);
        }
      }
      else if (!strcmp(argv[2], "-s"))
      {
        toks_proc(argv, lex_create_from_string);
      }
      else
      {
        exit(EXIT_FAILURE);
      }
    }
  }
  else if (!strcmp(argv[1], "repl"))
  {
    printf("stormout() to exit\n");
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
        printf("no-input\n");
        continue;
      }
      str[read - 1] = '\0'; // because newline is read
      repl_proc(str, lex_create_from_string);
      free(str);
      str = NULL;
    } while (true);
  }
  else if (!strcmp(argv[1], "help"))
  {
    printf("U compiler navigation\n");
    printf("\tU/bin/U <command> <option> <src>\n");
    printf("\t\t<command> [run, toks, repl, help]\n");
    printf("\t\t\thelp => displays this message\n");
    printf("\t\t\trun => can only be followed by [-s, -f] (string or path to file which oughts to get run)\n");
    printf("\t\t\ttoks => can only be followed by [-s, -f] (string or path to file from which tokens are extracted\n");
    printf("\t\t\trepl => followed by nothing\n");
  }
  else
  {
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(error_fatal, &pos, "Invalid command '%s'", argv[1]);
    // Invalid first argument.
    exit(EXIT_FAILURE);
  }
  return 0;
}