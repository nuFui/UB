#include "../../include/lexer/lex_helper.h"

// Reads file contents info a null-terminated string on the heap.
// Can raise heap-allocation error if fail.
int lex_helper_read_file(const char *path, char **buffer, int *size) {
  FILE *file = fopen(path, "r");
  if (!file) {
    fclose(file);
    error_raise(error_fatal, &ERROR_POSITION, "Could not open file '%s'");
    return 1;
  }
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *buffer = ualloc(&ERROR_POSITION, sizeof(char) * (*size));
  fread(*buffer, sizeof(char), *size, file);
  int status = fclose(file);
  if (status == EOF) {
    ufree(*buffer);
    ufree(file);
    error_raise(&error_fatal, &ERROR_POSITION, "Could not close file '%s'", path);
  }
  (*buffer)[(*size)] = '\0';
  return 0;
}