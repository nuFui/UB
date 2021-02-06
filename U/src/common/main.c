#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#include "../../include/common/compiler.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    error_raise(error_fatal, &ERROR_POSITION, "Too few arguments");
    exit(EXIT_FAILURE);
  }
  // TODO: Argument parser.
  if (!strcmp(argv[1], "run")) {
    if (argc > 2) {
      if (!strcmp(argv[2], "-f")) {
        // For files.
        if (!access(argv[3], F_OK)) {
          parser_register_t *reg = ualloc(&ERROR_POSITION, 0);
          reg->count = 0;
          run(argv[3], lex_create, &reg);
        } else {
          error_raise(error_fatal, &ERROR_POSITION, "Cannot access option '%f'", argv[3]);
          exit(EXIT_FAILURE);
        }
      } else if (!strcmp(argv[2], "-s")) {
        // For strings.
        parser_register_t *reg = ualloc(&ERROR_POSITION, 0);
        reg->count = 0;
        run(argv[3], lex_create_from_string, &reg);
      } else {
        error_raise(error_fatal, &ERROR_POSITION, "Invalid option '%s'", argv[2]);
        exit(EXIT_FAILURE);
      }
    }
  } else if (!strcmp(argv[1], "toks")) {
    if (argc > 2) {
      if (!strcmp(argv[2], "-f")) {
        if (!access(argv[3], F_OK)) {
          toks(argv[3], lex_create);
        } else {
          error_raise(error_fatal, &ERROR_POSITION, "Cannot access '%s'", argv[3]);
          exit(EXIT_FAILURE);
        }
      } else if (!strcmp(argv[2], "-s")) {
        toks(argv[3], lex_create_from_string);
      } else {
        error_raise(error_fatal, &ERROR_POSITION, "Invalid option '%s'", argv[2]);
        exit(EXIT_FAILURE);
      }
    }
  } else if (!strcmp(argv[1], "repl")) {
    parser_register_t *reg = ualloc(&ERROR_POSITION, 0);
    reg->count = 0;
    repl(reg);
  } else if (!strcmp(argv[1], "help")) {
    help();
  } else {
    error_raise(error_fatal, &ERROR_POSITION, "Invalid command '%s'", argv[1]);
    // Invalid first argument.
    exit(EXIT_FAILURE);
  }
  return 0;
}