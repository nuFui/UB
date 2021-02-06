#ifndef __COMMON_COMPILER_H__
#define __COMMON_COMPILER_H__

/*Toplevel definitions ran by switches.*/

#include "../lexer/lex.h"
#include "../parser/parser_eval.h"
#include "time.h"

#define OUTPUT_VARIABLE_INFO 0

void run(char *str, lexer_t (*func)(const char *str), parser_register_t **reg) {
  lexer_t lex = func(str);
  tok_list_t *list = lex_make_toks(&lex);
  parser_t par = parser_create(list);
  node_binary_tree_root_init();
  node_binary_tree(0, par.tok_list->count, &par, *root);
  eval_result_t k = node_binary_tree_eval(reg, *root);
  if (k.code == EVAL_FAILURE) {
    printf("Failed to evaluate.\n");
    exit(EXIT_SUCCESS);
  }
  if (k.kind != TOK_TYPE_IDF) {
    printf("%s = %s\n", lex.text, k.result);
  }
#if OUTPUT_VARIABLE_INFO
  else {
    identifier_t *s = (*reg)->identifiers[(*reg)->count - 1];
    printf("%s = %s\n", s->name, s->value);
  }
#endif
  lex_destroy(&lex);
  parser_destroy(&par);
  node_binary_tree_root_deinit(root);
}

void toks(char *str, lexer_t (*func)(const char *str)) {
  lexer_t lex = func(str);
  tok_list_t *list = lex_make_toks(&lex);
  tok_list_print(list, true, true);
  tok_list_delete(list);
  lex_destroy(&lex);
}

void repl(parser_register_t *reg) {
  struct tm *tinf = localtime(&(time_t){time(NULL)});
  printf("Ulang - %d\nType stormout() or CTRL+C to exit\n", tinf->tm_year + 1900);
  char *str = NULL;
  ssize_t len = 0;
  int read = 0;
  do {
    printf("> ");
    read = getline(&str, &len, stdin);  // flushes
    if (!strcmp("stormout()\n", str) || read == -1) {
      free(str);
      str = NULL;
      break;
    }
    if (!strcmp("\n", str)) {
      printf("nil\n");
      continue;
    }
    if (read > 0) {
      str[read - 1] = '\0';  // because newline is read
      run(str, lex_create_from_string, &reg);
    }
    free(str);
    str = NULL;
  } while (true);
}

void help() {
  printf("U compiler navigation\n");
  printf("\tU/bin/U <command> <option> <src>\n");
  printf("\t\t<command> [run, toks, repl, help]\n");
  printf("\t\t\thelp => this message\n");
  printf("\t\t\trun => can only be followed by [-s, -f] (string or path to file which oughts to get run)\n");
  printf("\t\t\ttoks => can only be followed by [-s, -f] (string or path to file from which tokens are extracted)\n");
  printf("\t\t\trepl => followed by nothing\n");
}

#endif