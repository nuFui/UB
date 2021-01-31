#include "../../include/lexer/lex_helper.h"

// Reads file contents info a null-terminated string on the heap.
// Can raise heap-allocation error if fail.
int lex_helper_read_file(const char *path, char **buffer, int *size)
{
  FILE *file = fopen(path, "r");
#if TEST_ERRFATAL
  file = NULL;
#endif
  if (!file)
  {
    fclose(file);
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(&error_fatal, &pos, "Could not open file '%s'", path);
    return 1;
  }
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *buffer = malloc(sizeof(char) * (*size));
#if TEST_MEMFAIL
  free(*buffer);
  *buffer = NULL;
#endif
  if (!*buffer)
  {
    free(*buffer);
    fclose(file);
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(&error_memory, &pos, "Could not allocate sufficient memory for '%p'", buffer);
    return 1;
  }
  fread(*buffer, sizeof(char), *size, file);
  int status = fclose(file);
  if (status == EOF)
  {
    free(*buffer);
    free(file);
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(&error_fatal, &pos, "Could not close file '%s'", path);
  }
  (*buffer)[(*size)] = '\0';
  return 0;
}