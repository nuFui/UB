#include "../include/lexh.h"

uint8_t lexh_read_file(const char *path, char **buffer, uint32_t *size)
{
  FILE *file = fopen(path, "r");
  if (!file)
  {
    // TODO: Handle error.
    return 1;
  }
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *buffer = malloc(sizeof(char) * (*size));
  if (!buffer)
  {
    // TODO: Handle erorr.
    return 1;
  }
  fread(*buffer, sizeof(char), *size, file);
  fclose(file);
  (*buffer)[(*size)] = '\0';
  return 0;
}